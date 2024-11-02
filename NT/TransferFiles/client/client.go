package main

import (
	"client/inputParser"
	"fmt"
	"net"
	"os"
)

func main() {
	inputParser.ParseInput()
	host := fmt.Sprintf("%s:%s", inputParser.GetIP(), inputParser.GetPort())
	fmt.Println("File to send: " + inputParser.GetFile())
	conn, err := net.Dial("tcp", host)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}
	defer conn.Close()

	SendFileName(conn, inputParser.GetFile())

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

func SendFileName(conn net.Conn, name string) {
	conn.Write([]byte(name))
}
