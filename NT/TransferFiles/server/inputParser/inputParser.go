package inputParser

import (
	"github.com/alexflint/go-arg"
)

var args struct {
	IP   string `default:"localhost" help:"IP address to accept connections"`
	Port string `arg:"-p, --port" default:"8181" help:"port to accept connections"`
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
