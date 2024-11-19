package main

import (
	"TridiagonalSLAE/internal/formEquation"
	"TridiagonalSLAE/internal/solver"
	"fmt"
)

func main() {
	leftPart, rightPart := formEquation.FormEquation()
	solver.PrintPart(leftPart, len(rightPart), true)
	fmt.Println(rightPart)
	solution := solver.Solve(leftPart, rightPart)
	fmt.Println("solution:")
	solver.PrintPart(solution, len(solution), false)
}