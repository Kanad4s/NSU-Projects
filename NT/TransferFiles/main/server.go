package main

import (
	"fmt"
	arg "github.com/alexflint/go-arg"
// 	_ "github.com/alexflint/go-scalar"
)

var args struct {
	Port string 	
}

func main() {
	arg.MustParse(&args)
	fmt.Println(args.Port)
}