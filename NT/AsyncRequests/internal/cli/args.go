package cli

import (
	"github.com/alexflint/go-arg"
)

type args struct {
	Request  string `arg:"-r, --request, required" help:"request to Mistral"`
	Key      string `arg:"-k, --key" default:"JiPEVBapCAYltDkWq6jvJfEIlvtvbtFD" help:"Mistral Key API"`
}

func (args) Description() string {
	return "this program does magic"
}

func Parse() (key string, request string) {
	var args args
	arg.MustParse(&args)

	return args.Key, args.Request
}