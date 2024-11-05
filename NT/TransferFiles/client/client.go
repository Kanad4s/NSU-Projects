package main

import (
	"bufio"
	"client/inputParser"
	"fmt"
	"net"
	"os"
)

func main() {
	inputParser.ParseInput()
	fmt.Println("isOverwrite: ", inputParser.IsOverwrite())
	host := fmt.Sprintf("%s:%s", inputParser.GetIP(), inputParser.GetPort())
	fmt.Println("File to send: " + inputParser.GetFile())
	conn, err := net.Dial("tcp", host)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}
	defer conn.Close()

	if !PrepareSendFile(conn) {
		os.Exit(0)
	}

}

func PrepareSendFile(conn net.Conn) bool {
	SendFileName(conn, inputParser.GetFile())
	SendOverwrite(conn, inputParser.IsOverwrite())
	doSend := DoSendFile(conn)
	return doSend
}

func PrintMessage(message string) {
	if message == "9876" {
		fmt.Println("Sending is accepted")
	} else if message == "ABCD" {
		fmt.Println("Sending is unaccepted")
	}
}

func DoSendFile(conn net.Conn) bool {
	scanner := bufio.NewScanner(conn)
	scanner.Scan()
	message := scanner.Text()
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading message: ", err.Error())
	}
	fmt.Println("get message: ", message)

	PrintMessage(message)
	return message == "9876"
}

func SendFileName(conn net.Conn, name string) {
	n, err := conn.Write([]byte(name))
	fmt.Println("write(): ", n)
	if err != nil {
		fmt.Println("error send FileName:", err.Error())
		os.Exit(1)
	}
}

func SendOverwrite(conn net.Conn, isOverwrite bool) {
	sendValue := "0"
	if isOverwrite {
		sendValue = "1"
	}
	n, err := conn.Write([]byte(sendValue))
	fmt.Println("write(): ", n)
	if err != nil {
		fmt.Println("error send Overwrite:", err.Error())
		os.Exit(1)
	}
}
