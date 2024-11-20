package main

import (
	"TransferFiles/internal/client/inputParser"
	ni "TransferFiles/internal/networkInteraction"
	"fmt"
	"io"
	"net"
	"os"
	"strconv"
	"strings"
)

func main() {
	host := GetHost()
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

func SendFile(fileName string, conn net.Conn) bool {
	file, err := os.OpenFile(fileName, os.O_RDONLY, os.ModePerm)
	if err!= nil {
		fmt.Println("Error open file: ", err.Error())
		os.Exit(1)
	}
	defer file.Close()
	buffer := ni.GetBuffer(file)
	fmt.Println("Send start")
	for {
		read, err := file.Read(buffer)
		if err != nil {
			if err == io.EOF {
				break
			}
			fmt.Printf("File reading error: %v\n", err.Error())
			return false
		}
		fmt.Printf("buffer: %v\n", string(buffer[:read]))
		_, err = conn.Write(buffer[:read])
		if err != nil {
			fmt.Printf("File data sending error: %v\n", err.Error())
			return false
		}
	}
	fmt.Println("Send finish")
	return true
}

func PrepareSendFile(conn net.Conn) bool {
	SendFileName(conn, inputParser.GetFile())
	SendOverwrite(conn, inputParser.IsOverwrite())
	SendFileSize(conn, inputParser.GetFile())
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

func SeparateFileName(name string) string {
	index := strings.LastIndex(name, "/")
	if index == -1 {
		return name
	} else {
		return name[index + 1:]
	}
}

func SendFileName(conn net.Conn, name string) {
	sepName := SeparateFileName(name)
	fmt.Println(sepName)
	ni.SendMessage(sepName, conn)
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

func SendFileSize(conn net.Conn, fileName string) {
	file, err := os.OpenFile(fileName, os.O_RDONLY, os.ModePerm)
	if err!= nil {
		fmt.Println("Error open file: ", err.Error())
		os.Exit(1)
	}
	defer file.Close()
	fi, err := file.Stat()
	if err!= nil {
		fmt.Println("Error get file info: ", err.Error())
		os.Exit(1)
	}
	sizeString := strconv.Itoa(int(fi.Size()))
	fmt.Printf("File %v size: %v, %v\n", fileName, fi.Size(), sizeString)
	ni.SendMessage(sizeString, conn)
	response := ni.GetMessage(conn)
	if response != ni.SuccessMsg {
		os.Exit(1)
	}
}

func GetHost() string {
	inputParser.ParseInput()
	fmt.Println("isOverwrite: ", inputParser.IsOverwrite())
	host := fmt.Sprintf("%s:%s", inputParser.GetIP(), inputParser.GetPort())
	return host
}
