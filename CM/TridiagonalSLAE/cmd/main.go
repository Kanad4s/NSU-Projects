package main

import (
	"fmt"
	"TridiagonalSLAE/internal/inputParser"
)

func main() {
	inputParser.ParseInput()
	n, eps, gamma, mode := inputParser.GetInput()
	fmt.Printf("%v, %v, %v, %v\n", n, eps, gamma, mode)
}