package main

import (
	"TransferFiles/internal/client"
	"fmt"
	"net"
	"os"
)

func main() {
	host := client.GetHost()
	conn, err := net.Dial("tcp", host)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())	
		os.Exit(1)
	}
	defer conn.Close()

	if !client.PrepareSendFile(conn) {
		os.Exit(0)
	}

	client.SendFile(client.GetFile(), conn)
	client.ParseControlMsg(conn)
}
