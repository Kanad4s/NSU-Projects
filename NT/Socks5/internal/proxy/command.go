package proxy

import (
	"Socks5/internal/log"
	"encoding/binary"
	"fmt"
	"io"
	"net"
	"strconv"
	"strings"
)

func connectCommand(conn *net.TCPConn) (*net.TCPConn, byte, error) {
	version := make([]byte, 1)
	_, err := io.ReadFull(conn, version)
	if err != nil {
		return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, NewErrCommandRequestParsing("No socks version")
	}
	if version[0] != SOCKS_VERSION {
		return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, NewErrCommandRequestParsing(fmt.Sprintf(
			"Socks version %v is expected, but not %v", SOCKS_VERSION, version[0]))
	}

	command := make([]byte, 1)
	_, err = io.ReadFull(conn, command)
	if err != nil {
		return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, NewErrCommandRequestParsing("No command")
	}
	if command[0] != SOCKS_CMD_CONNECT {
		return nil, SOCKS_REPLY_COMMAND_NOT_SUPPORTED, NewErrCommandRequestParsing(fmt.Sprintf(
			"Unsupported command %v, %v command supported", command[0], SOCKS_CMD_CONNECT))
	}

	reservedByte := make([]byte, 1)
	_, err = io.ReadFull(conn, reservedByte)
	if err != nil {
		return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, NewErrCommandRequestParsing("No reserved byte")
	}
	if reservedByte[0] != SOCKS_RESERVED {
		return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, NewErrCommandRequestParsing(fmt.Sprintf(
			"Reserved byte must be set to %v, but not %v", SOCKS_RESERVED, reservedByte[0]))
	}

	addressType := make([]byte, 1)
	_, err = io.ReadFull(conn, addressType)
	if err != nil {
		return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, NewErrCommandRequestParsing("No address type")
	}
	switch addressType[0] {
	case SOCKS_ATYP_IPV4:
		ipv4, port, err := readIpv4AndPort(conn)
		if err != nil {
			return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, err
		}
		return ipv4Connect(ipv4, port)
	case SOCKS_ATYP_DOMAINNAME:
		domainName, port, err := readDomainNameAndPort(conn)
		if err != nil {
			return nil, SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET, err
		}
		return domainNameConnect(domainName, port, conn)
	default:
		return nil, SOCKS_REPLY_ADDRESS_TYPE_NOT_SUPPORTED, NewErrCommandRequestParsing(fmt.Sprintf(
			"Unsupported address type %v, %v is supported", addressType[0],
			[]byte{SOCKS_ATYP_IPV4, SOCKS_ATYP_DOMAINNAME}))
	}
}

func readIpv4AndPort(client *net.TCPConn) (net.IP, int, error) {
	ip := make([]byte, 4)
	_, err := io.ReadFull(client, ip)
	if err != nil {
		return nil, -1, NewErrCommandRequestParsing("No ipv4 address")
	}

	portBytes := make([]byte, 2)
	_, err = io.ReadFull(client, portBytes)
	if err != nil {
		return nil, -1, NewErrCommandRequestParsing("No port")
	}
	port := int(binary.BigEndian.Uint16(portBytes))

	return ip, port, nil
}

func ipv4Connect(ipv4 net.IP, port int) (*net.TCPConn, byte, error) {
	// Connect to peer
	tcpAddr := &net.TCPAddr{
		IP:   ipv4,
		Port: port,
	}

	peer, err := net.DialTCP("tcp4", nil, tcpAddr)
	if err != nil {
		if opErr, ok := err.(*net.OpError); ok {
			if opErr.Temporary() {
				return nil, SOCKS_REPLY_TTL_EXPIRED, NewErrPeerConnectionCreating(err.Error())
			}
			if opErr.Err.Error() == "network is unreachable" {
				return nil, SOCKS_REPLY_NETWORK_UNREACHABLE, NewErrPeerConnectionCreating(err.Error())
			}
			if opErr.Err.Error() == "no route to host" {
				return nil, SOCKS_REPLY_HOST_UNREACHABLE, NewErrPeerConnectionCreating(err.Error())
			}
			if opErr.Err.Error() == "connection refused" {
				return nil, SOCKS_REPLY_CONNECTION_REFUSED, NewErrPeerConnectionCreating(err.Error())
			}
		}
		return nil, SOCKS_REPLY_GENERAL_SOCKS_SERVER_FAILURE, NewErrPeerConnectionCreating(err.Error())
	}

	return peer, SOCKS_REPLY_SUCCEEDED, nil
}

func readDomainNameAndPort(client *net.TCPConn) (string, int, error) {
	domainNameSize := make([]byte, 1)
	_, err := io.ReadFull(client, domainNameSize)
	if err != nil {
		return "", -1, NewErrCommandRequestParsing("No domain name size")
	}

	domainName := make([]byte, domainNameSize[0])
	_, err = io.ReadFull(client, domainName)
	if err != nil {
		return "", -1, NewErrCommandRequestParsing("No domain name")
	}

	portBytes := make([]byte, 2)
	_, err = io.ReadFull(client, portBytes)
	if err != nil {
		return "", -1, NewErrCommandRequestParsing("No port")
	}
	port := int(binary.BigEndian.Uint16(portBytes))

	return string(domainName), port, nil
}

func domainNameConnect(domainName string, port int, client *net.TCPConn) (*net.TCPConn, byte, error) {
	ips, err := net.LookupIP(domainName)
	if err != nil {
		return nil, SOCKS_REPLY_HOST_UNREACHABLE, NewErrDNSResolving(err.Error())
	}
	log.Log.Infof("%v: Domain name %v is resolved to %v", client.RemoteAddr(), domainName, ips)

	for _, ip := range ips {
		if ipv4 := ip.To4(); ipv4 != nil {
			peer, reply, err := ipv4Connect(ipv4, port)
			if err == nil {
				// success 
				return peer, reply, nil
			}
		}
	}

	return nil, SOCKS_REPLY_HOST_UNREACHABLE,
		NewErrDNSResolving("No hosts with IPv6 addresses or working IPv4 addresses")
}

func sendCommandReply(client *net.TCPConn, reply byte) error {
	addr := strings.Split(client.LocalAddr().String(), ".")
	replyMsg := []byte{
		SOCKS_VERSION, reply, SOCKS_RESERVED, SOCKS_ATYP_IPV4,
	}

	for i := range addr {
		addrPart, _ := strconv.Atoi(addr[i])
		if i == 3 {
			addrPart, _ = strconv.Atoi(strings.Split(addr[i], ":")[0])
		}
		replyMsg = append(replyMsg, byte(addrPart))
	}

	port, _ := strconv.Atoi(strings.Split(addr[3], ":")[1])
	firstPartPort := port / 100
	secondPartPort := port % 100
	replyMsg = append(replyMsg, byte(firstPartPort))
	replyMsg = append(replyMsg, byte(secondPartPort))

	_, err := client.Write(replyMsg)
	if err != nil {
		return NewErrCommandReplySending(err.Error())
	}
	return nil
}
