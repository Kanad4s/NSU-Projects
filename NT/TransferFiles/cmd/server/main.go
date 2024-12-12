package main

import (
	"TransferFiles/internal/server"
	"fmt"
	"net"
	"os"
)

func main() {
	server.CreateStore()
	server.ParseInput()
	host := fmt.Sprintf("%s:%s", server.GetIP(), server.GetPort())
	listener, err := net.Listen("tcp", host)
	if err != nil {
		fmt.Println("Error net.Listen():", err.Error())
		os.Exit(1)
	}
	defer listener.Close()
	fmt.Println("Server is listening on " + host)

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("Error accepting:", err.Error())
			os.Exit(1)
		}
		fmt.Println("Connected with", conn.RemoteAddr().String())
		go server.HandleRequest(conn)
	}
}

