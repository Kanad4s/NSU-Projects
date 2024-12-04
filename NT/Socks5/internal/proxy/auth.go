package proxy

import (
	"fmt"
	"io"
	"net"
)

func authenticate(conn *net.TCPConn) (byte, error) {
	version := make([]byte, 1)
	_, err := io.ReadFull(conn, version)
	if err != nil {
		return SOCKS_AUTH_NO_ACCEPTABLE_METHODS, NewErrAuthRequestParsing("No socks version")
	}
	if version[0] != SOCKS_VERSION {
		return SOCKS_AUTH_NO_ACCEPTABLE_METHODS, NewErrAuthRequestParsing(fmt.Sprintf(
			"Socks version %v is expected, but not %v", SOCKS_VERSION, version[0]))
	}

	methodCount := make([]byte, 1)
	_, err = io.ReadFull(conn, methodCount)
	if err != nil {
		return SOCKS_AUTH_NO_ACCEPTABLE_METHODS, NewErrAuthRequestParsing("No authentication method count")
	}

	methods := make([]byte, methodCount[0])
	actualMethodCount, err := io.ReadFull(conn, methods)
	if err != nil {
		return SOCKS_AUTH_NO_ACCEPTABLE_METHODS, NewErrAuthRequestParsing(fmt.Sprintf(
			"Not enough authentication methods: Expected %v, received %v", methodCount, actualMethodCount))
	}

	for _, methods := range methods {
		if methods == SOCKS_AUTH_NO_REQUIRED {
			// success
			return SOCKS_AUTH_NO_REQUIRED, nil
		}
	}

	return SOCKS_AUTH_NO_ACCEPTABLE_METHODS, NewErrAuthRequestParsing(fmt.Sprintf(
		"Unsupported auth methods, %v method supported", []byte{SOCKS_AUTH_NO_REQUIRED}))
}

func sendAuthReply(client *net.TCPConn, method byte) error {
	replyMsg := []byte{SOCKS_VERSION, method}

	_, err := client.Write(replyMsg)
	if err != nil {
		return NewErrAuthReplySending(err.Error())
	}
	return nil
}