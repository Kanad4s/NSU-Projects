package main

import (
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

	PrepareSendFile(conn)
	// consoleScanner := bufio.NewScanner(os.Stdin)
	// for consoleScanner.Scan() {
	// 	text := consoleScanner.Text()
	// 	conn.Write([]byte(text + "\n"))

	// 	response, err := bufio.NewReader(conn).ReadString('\n')
	// 	if err != nil {
	// 		fmt.Println("Error reading:", err.Error())
	// 		os.Exit(1)
	// 	}
	// 	fmt.Println("Response: " + response)
	// }

	// if err := consoleScanner.Err(); err != nil {
	// 	fmt.Println("Error reading from console:", err.Error())
	// }
}

func PrepareSendFile(conn net.Conn) {
	SendFileName(conn, inputParser.GetFile())
	SendOverwrite(conn, inputParser.IsOverwrite())
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
