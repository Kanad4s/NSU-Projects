package main

import (
	"bufio"
	"fmt"
	"net"
	"os"

	"github.com/alexflint/go-arg"
)

var args struct {
	Port string `arg:"-p, --port" default:"8181" help:"port to accept connections"`
}

func main() {
	arg.MustParse(&args)
	port := args.Port
	host := fmt.Sprintf("%s%s", "localhost:", port)
	conn, err := net.Dial("tcp", host)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}
	// defer conn.Close()

	consoleScanner := bufio.NewScanner(os.Stdin)
	for consoleScanner.Scan() {
		text := consoleScanner.Text()
		conn.Write([]byte(text + "\n"))

		response, err := bufio.NewReader(conn).ReadString('\n')
		if err != nil {
			fmt.Println("Error reading:", err.Error())
			os.Exit(1)
		}
		fmt.Println("Response: " + response)
	}

	if err := consoleScanner.Err(); err != nil {
		fmt.Println("Error reading from console:", err.Error())
	}
}
