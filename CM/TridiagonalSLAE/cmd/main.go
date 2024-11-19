package main

import (
	"TridiagonalSLAE/internal/formEquation"
	"fmt"
)

func main() {
	leftPart, rightPart := formEquation.FormEquation()
	fmt.Println(leftPart)
	fmt.Println(rightPart)
}