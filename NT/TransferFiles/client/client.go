package main

import (
	"bufio"
	"fmt"
	"net"
	"os"

	"github.com/alexflint/go-arg"
)

var args struct {
	IP   string `default:"localhost" help:"IP address to connect"`
	Port string `arg:"-p, --port" default:"8181" help:"port to connect"`
	File string `arg:"-f, --file, required" help:"file to transfer"`
}

func main() {
	arg.MustParse(&args)
	host := fmt.Sprintf("%s:%s", args.IP, args.Port)
	fmt.Println("File to send" + args.File)
	conn, err := net.Dial("tcp", host)
	if err != nil {
		fmt.Println("Error connecting:", err.Error())
		os.Exit(1)
	}
	defer conn.Close()

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
