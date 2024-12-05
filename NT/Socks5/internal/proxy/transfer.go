package proxy

import (
	"Socks5/internal/log"
	"io"
	"net"
	"sync"
)

func transferData(conn *net.TCPConn, peer *net.TCPConn) {
	var wg sync.WaitGroup
	wg.Add(2)

	go copyData(conn, peer, &wg)
	go copyData(peer, conn, &wg)

	wg.Wait()
}

func copyData(dest *net.TCPConn, src *net.TCPConn, wg *sync.WaitGroup) {
	defer wg.Done()
	defer dest.Close()

	bytes, err := io.Copy(dest, src)

	if err != nil {
		log.Log.Errorf("%v: Reading error: %v", dest.RemoteAddr(), err)
	} else {
		log.Log.Infof("%v: Reading success, read %v bytes", dest.RemoteAddr(), bytes)
	}
}