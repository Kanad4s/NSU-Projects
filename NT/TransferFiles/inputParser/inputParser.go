package inputParser

import (
	"fmt"
	"github.com/alexflint/go-arg"
)

var args struct {
	Port string `arg:"-p, --port" default:"8181" help:"port to accept connections"`	
}

func test() string {
	return "abcdef"
}

func getPort(input &args) string {
	arg.MustParse(input)
	return args.Port
}