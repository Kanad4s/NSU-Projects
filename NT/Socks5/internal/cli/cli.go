package cli

import (
	"fmt"
	"os"

	"github.com/alexflint/go-arg"
)

type args struct {
	Port      int `arg:"-p, --port, required" help:"multicast group port to connect"`
}

func (args) Description() string {
	return "this program does magic"
}

func Parse() int {
	var args args
	arg.MustParse(&args)

	validatePort(args.Port)

	return args.Port
}

func validatePort(port int) {
	if port < 0 || port > 65535 {
		fmt.Println(port, "is not valid port")
		os.Exit(1)
	}
}