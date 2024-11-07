package main

import (
	"TransferFiles/internal/client/inputParser"
	"fmt"
	"io"
	"net"
	"os"
	ni "TransferFiles/internal/networkInteraction"
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

	SendFile(inputParser.GetFile(), conn)
}

func SendFile(fileName string, conn net.Conn) {
	file, err := os.OpenFile(fileName, os.O_RDONLY, os.ModePerm)
	if err!= nil {
		fmt.Println("Error open file: ", err.Error())
		os.Exit(1)
	}
	defer file.Close()

	_, err = io.Copy(file, conn)
	if err != nil {
		fmt.Println("Error sending file: ", err.Error())
		os.Exit(1)
	}

}

func PrepareSendFile(conn net.Conn) bool {
	SendFileName(conn, inputParser.GetFile())
	SendOverwrite(conn, inputParser.IsOverwrite())
	fmt.Println("send parameters done")
	doSend := DoSendFile(conn)
	return doSend
}

func PrintMessage(message string) {
	if message == "9876" {
		fmt.Println("File sending is accepted")
	} else if message == "ABCD" {
		fmt.Println("File sending is unaccepted")
	}
}

func DoSendFile(conn net.Conn) bool {
	response := ni.GetMessage(conn)
	PrintMessage(response)
	return response == "9876"
}

func SendFileName(conn net.Conn, name string) {
	ni.SendMessage(name, conn)
	response := ni.GetMessage(conn)
	if response != ni.SuccessMsg {
		os.Exit(1)
	}
}

func SendOverwrite(conn net.Conn, isOverwrite bool) {
	sendValue := "0"
	if isOverwrite {
		sendValue = "1"
	}
	ni.SendMessage(sendValue, conn)
	response := ni.GetMessage(conn)
	if response != ni.SuccessMsg {
		os.Exit(1)
	}
}
