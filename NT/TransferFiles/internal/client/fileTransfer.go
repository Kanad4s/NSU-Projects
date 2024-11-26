package client

import (
	ni "TransferFiles/internal/networkInteraction"
	"fmt"
	"os"
	"strconv"
	"strings"
	"io"
	"net"
)

func SendFile(fileName string, conn net.Conn) {
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
			return
		}
		// fmt.Printf("buffer: %v\n", string(buffer[:read]))
		_, err = conn.Write(buffer[:read])
		if err != nil {
			fmt.Printf("File data sending error: %v\n", err.Error())
			return
		}
	}
	fmt.Println("Send finish")
}

func ParseControlMsg(conn net.Conn) {
	msg := ni.GetMessage(conn)
	fmt.Println("Sending result: " + msg)
}

func PrepareSendFile(conn net.Conn) bool {
	SendFileName(conn, GetFile())
	SendOverwrite(conn, IsOverwrite())
	SendFileSize(conn, GetFile())
	// fmt.Println("send parameters done")
	doSend := DoSendFile(conn)
	return doSend
}

func PrintMessage(message string) {
	if message == ni.TransferFileAcceptedMsg {
		fmt.Println("File sending is accepted")
	} else if message == ni.TransferFileDeclineMsg {
		fmt.Println("File sending is unaccepted")
	}
}

func DoSendFile(conn net.Conn) bool {
	response := ni.GetMessage(conn)
	PrintMessage(response)
	return response == ni.TransferFileAcceptedMsg
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
	fmt.Printf("File \"%v\" size: %v\n", fileName, sizeString)
	ni.SendMessage(sizeString, conn)
	response := ni.GetMessage(conn)
	if response != ni.SuccessMsg {
		os.Exit(1)
	}
}

func GetHost() string {
	ParseInput()
	// fmt.Println("isOverwrite: ", IsOverwrite())
	host := fmt.Sprintf("%s:%s", GetIP(), GetPort())
	return host
}
