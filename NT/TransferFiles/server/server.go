package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"server/inputParser"
)

func main() {
	inputParser.ParseInput()
	host := fmt.Sprintf("%s:%s", inputParser.GetIP(), inputParser.GetPort())
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
		go handleRequest(conn)
	}
}

func handleRequest(conn net.Conn) {
	defer conn.Close()
	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		clientMessage := scanner.Text()
		fmt.Printf("Received: %s\n", clientMessage)
		conn.Write([]byte(clientMessage + "\n"))
	}
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading:", err.Error())
	}
}
