package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"server/inputParser"
	"strings"
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
	if !PrepareReceivingFile(conn) {
		// conn.Write([]byte())
		return
	}
}

func PrepareReceivingFile(conn net.Conn) (receive bool) {
	fileName := GetFileName(conn)
	overwrite := GetOverwrite(conn)
	if len(overwrite) == 0 {
		overwrite = fileName[len(fileName)-1:]
		fileName = fileName[:len(fileName)-1]
	}
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
		receive = false
	} else {
		os.OpenFile(filePath, os.O_CREATE|os.O_RDWR, 0666)
		receive = true
	}
	return
}

func DoesFileExist(path string) (found bool, err error) {
	if _, err := os.Stat(path); err == nil {
		if os.IsNotExist(err) {
			err = nil
		}
	} else {
		found = true
	}

	return
}

func setIsOverwrite(overwrite string) bool {
	return overwrite == "1"
}

func GetOverwrite(conn net.Conn) string {
	scanner := bufio.NewScanner(conn)
	scanner.Scan()
	overwrite := scanner.Text()
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading Overwrite: ", err.Error())
	}
	fmt.Printf("get row overwrite: %v, size: %v\n", overwrite, len(overwrite))
	return overwrite
}

func GetFileName(conn net.Conn) string {
	scanner := bufio.NewScanner(conn)
	scanner.Scan()
	fileName := scanner.Text()
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading fileName: ", err.Error())
	}
	fmt.Printf("get row fileName: %v, size: %v\n", fileName, len(fileName))
	return fileName
}

func CreateStore() {
	err := os.Mkdir(storePackage, 0666)
	if err != nil {
		fmt.Println("Error Mkdir: ", err.Error())
	}
}
