package main

import (
	"bufio"
	"client/inputParser"
	"fmt"
	"net"
	"os"
	"strings"
)

const endSymbol = '\a'
const successMsg = "success"

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

	if !TestSend(conn) {
		os.Exit(1)
	}
	// if !PrepareSendFile(conn) {
	// 	os.Exit(0)
	// }

}

func TestSend(conn net.Conn) bool {
	for i := 0; i < 2; i++ {
		text := inputParser.GetFile()
		if i == 1 {
			text = "0"
			if inputParser.IsOverwrite() {
				text = "1"
			}
		}
		conn.Write([]byte(text + string(endSymbol)))

		// получаем ответ от сервера
		response, err := bufio.NewReader(conn).ReadString(endSymbol)
		if err != nil {
			fmt.Println("Error reading:", err.Error())
			os.Exit(1)
		}
		fmt.Printf("Server says: %s\n", response)
	}
	response, err := bufio.NewReader(conn).ReadString(endSymbol)
	if err != nil {
		fmt.Println("Error reading:", err.Error())
		os.Exit(1)
	}
	fmt.Printf("Server says: %s\n", response)

	// doSend := DoSendFile(conn)
	doSend := true
	return doSend
}

func PrepareSendFile(conn net.Conn) bool {
	SendFileName(conn, inputParser.GetFile())
	SendOverwrite(conn, inputParser.IsOverwrite())
	fmt.Println("send done")
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
	store := make([]byte, 0, 4096)

	response := GetMessage(conn)
	// response, err := bufio.NewReader(conn).ReadString(endSymbol)
	// if err != nil {
	// 	fmt.Println("Error reading:", err.Error())
	// 	os.Exit(1)
	// }
	fmt.Println("response: ", response)
	fmt.Printf("get message: %v, size: %v\n", store, len(store))
	message := "zaglushka"
	fmt.Println("get message: ", message)

	PrintMessage(message)
	return message == "9876\n"
}

func SendFileName(conn net.Conn, name string) {
	// writer := bufio.NewWriter(conn)
	// n, err := writer.Write([]byte(name + "\n"))
	// n, err := conn.Write([]byte(name + string(endSymbol)))
	SendMessage(name, conn)
	response := GetMessage(conn)
	if response != successMsg {
		os.Exit(1)
	}
	// if err != nil {
	// 	fmt.Println("error send FileName:", err.Error())
	// 	os.Exit(1)
	// }
}

func SendOverwrite(conn net.Conn, isOverwrite bool) {
	sendValue := "0"
	if isOverwrite {
		sendValue = "1"
	}
	// writer := bufio.NewWriter(conn)
	// n, err := writer.Write([]byte(sendValue + "\n"))
	// n, err := conn.Write([]byte(sendValue + string(endSymbol)))
	SendMessage(sendValue, conn)
	response := GetMessage(conn)
	if response != successMsg {
		os.Exit(1)
	}
	// fmt.Println("overwrite write(): ", n)
	// if err != nil {
	// 	fmt.Println("error send Overwrite:", err.Error())
	// 	os.Exit(1)
	// }
}

func GetMessage(conn net.Conn) (msg string) {
	msg, err := bufio.NewReader(conn).ReadString(endSymbol)
	if err != nil {
		fmt.Println("Error reading fileName: ", err.Error())
	}
	endSymbolIndex := strings.LastIndex(msg, string(endSymbol))
	if endSymbolIndex == -1 {
		endSymbolIndex = len(msg) - 1
	}
	msg = msg[:endSymbolIndex]
	return
}

func SendMessage(msg string, conn net.Conn) {
	_, err := conn.Write([]byte(msg + string(endSymbol)))
	if err != nil {
		println("Error sendResponse: ", err.Error())
	}
}
