package inputParser

import (
	"fmt"
	"strconv"

	"github.com/alexflint/go-arg"
)

var args struct {
	N   string   `arg:"-n, required"    help:"Matrix size"`
	Eps string   `arg:"-e, --eps"   help:"Epsilon"`
	Gamma string `arg:"-g, --gamma" help:"file to transfer"`
}

func ParseInput() {
	p := arg.MustParse(&args)
	n, _ := strconv.Atoi(args.N)
	if (n <= 0) {
		p.Fail("Matrix size must be positive")
	}
}

func GetInput() (n int, eps float64, gamma float64) {
	n, err := strconv.Atoi(args.N)
	if err != nil {
		fmt.Println("Error converting N to int")
	}
	eps, err = strconv.ParseFloat(args.Eps, 64)
	if err != nil {
		fmt.Println("Error converting Eps to int")
	}
	gamma, err = strconv.ParseFloat(args.Gamma, 64)
	if err != nil {
		fmt.Println("Error converting Gamma to int")
	}
	return
}
