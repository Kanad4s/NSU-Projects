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
		go HandleRequest(conn)
	}
}

func HandleRequest(conn net.Conn) {
	defer conn.Close()
	fileName := GetFileName(conn)
	fmt.Println("getFileName(): ", fileName)
	os.Mkdir("uploads", 0777)
}

func GetFileName(conn net.Conn) string {
	scanner := bufio.NewScanner(conn)
	scanner.Scan()
	fileName := scanner.Text()
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading fileName: ", err.Error())
	}
	return fileName
}