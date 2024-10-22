package main

import (
	"fmt"
	arg "github.com/alexflint/go-arg"
)

var args struct {
	Port string `arg:"-p, --port" default:"8181" help:"port to accept connections"`	
}

func main() {
	arg.MustParse(&args)
	fmt.Println(args.Port)
}