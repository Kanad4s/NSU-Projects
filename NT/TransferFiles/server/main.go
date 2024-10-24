package main

import (
	"bufio"
	"fmt"
	"net"
	"os"

	"github.com/alexflint/go-arg"
)

var args struct {
	IP   string `default:"localhost" help:"IP address to accept connections"`
	Port string `arg:"-p, --port" default:"8181" help:"port to accept connections"`
}

func main() {
	arg.MustParse(&args)
	host := fmt.Sprintf("%s:%s", args.IP, args.Port)
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
