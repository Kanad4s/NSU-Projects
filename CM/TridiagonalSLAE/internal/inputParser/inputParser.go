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
	if (args.Eps != "" && args.Gamma != "") {
		p.Fail("Must provide either --Eps or --Gamma")
	}
}

func GetInput() (n int, eps float64, gamma float64, mode int) {
	n, err := strconv.Atoi(args.N)
	if err != nil {
		fmt.Println("Error converting N to int")
	}
	mode = 1
	if (args.Eps != "" || args.Gamma != "") {
		if args.Eps != "" {
			eps, err = strconv.ParseFloat(args.Eps, 64)
			if err != nil {
				fmt.Println("Error converting Eps to float64")
			}
			mode = 2
		}
		if args.Gamma != "" {
			gamma, err = strconv.ParseFloat(args.Gamma, 64)
			if err != nil {
				fmt.Println("Error converting Gamma to float64")
			}
			mode = 3
		}
	}
	return
}
