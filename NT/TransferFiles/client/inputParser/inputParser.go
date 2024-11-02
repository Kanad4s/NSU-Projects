package inputParser

import (
	"fmt"

	"github.com/alexflint/go-arg"
)

var args struct {
	IP   string `default:"localhost" help:"IP address to connect"`
	Port string `arg:"-p, --port" default:"8181" help:"port to connect"`
	File string `arg:"-f, --file, required" help:"file to transfer"`
}

func ParseInput() {
	arg.MustParse(&args)
}

func GetIP() string {
	return args.IP
}

func GetPort() string {
	return args.Port
}

func GetFile() string {
	return args.File
}

func MyFoo() {
	fmt.Println("my foo")
}
