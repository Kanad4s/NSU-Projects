package proxy

import (
	"net"

	"Socks5/internal/log"
)

func Start(port int) {
	listener, err := net.ListenTCP("tcp4", &net.TCPAddr{Port: port})
	if err != nil {
		log.Log.Errorf("Listen socket creation error: %v", err)
		return
	}
	log.Log.Infof("Proxy server is listening on %v", listener.Addr())

	defer func() {
		listener.Close()
		log.Log.Debugf("Listener is closed")
	}()

	for ;; {
		client, err := listener.AcceptTCP()
		if err != nil {
			log.Log.Errorf("Client accepting error: %v", err)
			return
		}
		log.Log.Infof("Accepted client %v", client.RemoteAddr())

		go handleClient(client)
	}
}

func handleClient(conn *net.TCPConn) {
	defer func() {
		conn.Close()
		log.Log.Debugf("%v: Client connection is closed", conn.RemoteAddr())
	}()

	authReply, err := authenticate(conn)
	if err != nil {
		log.Log.Errorf("%v: %v", conn.RemoteAddr(), err)
		if err := sendAuthReply(conn, authReply); err != nil {
			log.Log.Errorf("%v: %v", conn.RemoteAddr(), err)
		}
		return
	}
	if err := sendAuthReply(conn, authReply); err != nil {
		log.Log.Errorf("%v: %v", conn.RemoteAddr(), err)
		return
	}
	log.Log.Debugf("%v: Client is authenticated", conn.RemoteAddr())

	peer, commandReply, err := connectCommand(conn)
	if err != nil {
		log.Log.Errorf("%v: %v", conn.RemoteAddr(), err)
		if err := sendCommandReply(conn, commandReply); err != nil {
			log.Log.Errorf("%v: %v", conn.RemoteAddr(), err)
		}
		return
	}
	if err := sendCommandReply(conn, commandReply); err != nil {
		log.Log.Errorf("%v: %v", conn.RemoteAddr(), err)
		return
	}
	log.Log.Debugf("%v: Proxy server is connected to peer", conn.RemoteAddr())

	// Transfer data
	transferData(conn, peer)
}