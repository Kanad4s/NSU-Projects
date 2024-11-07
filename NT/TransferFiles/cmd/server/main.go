package main

import (
	ni "TransferFiles/internal/networkInteraction"
	"TransferFiles/internal/server/inputParser"
	"fmt"
	"net"
	"os"
)

const storePackage = "uploads"

func main() {
	CreateStore()
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
	doReceive, message := PrepareReceivingFile(conn)
	fmt.Println("message to send: ", message)
	if !doReceive {
		fmt.Println("do not receive file on conn: ", conn.RemoteAddr().String())
		return
	}

}

func PrepareReceivingFile(conn net.Conn) (doReceive bool, message string) {
	fileName := GetFileName(conn)
	overwrite := GetOverwrite(conn)
	isOverwrite := setIsOverwrite(overwrite)
	fmt.Println("getFileName(): ", fileName)
	fmt.Println("getOverwrite(): ", overwrite)
	fmt.Println("getOverwrite(): ", isOverwrite)
	filePath := storePackage + "/" + fileName
	fileExists, err := DoesFileExist(filePath)
	if err != nil {
		panic(err)
	}
	if fileExists && !isOverwrite {
		doReceive = false
		message = "ABCD"
	} else {
		os.OpenFile(filePath, os.O_CREATE|os.O_RDWR, 0666)
		doReceive = true
		message = "9876"
	}

	ni.SendMessage(message, conn)
	return
}

func DoesFileExist(path string) (found bool, err error) {
	if _, err := os.Stat(path); err == nil {
		if os.IsNotExist(err) {
			err = nil
		} else {
			found = true
		}
	}
	fmt.Printf("Does %s exists: %v\n", path, found)
	return
}

func setIsOverwrite(overwrite string) bool {
	return overwrite == "1"
}

func GetOverwrite(conn net.Conn) string {
	overwrite := ni.GetMessage(conn)
	fmt.Printf("get row overwrite: %v, size: %v\n", overwrite, len(overwrite))

	ni.SendMessage(ni.SuccessMsg, conn)
	return overwrite
}

func GetFileName(conn net.Conn) string {
	fileName := ni.GetMessage(conn)
	fmt.Printf("get row fileName: %v, size: %v\n", fileName, len(fileName))

	ni.SendMessage(ni.SuccessMsg, conn)
	return fileName
}

func CreateStore() {
	err := os.Mkdir(storePackage, 0755)
	if err != nil {
		fmt.Println("Error Mkdir: ", err.Error())
	}
}
