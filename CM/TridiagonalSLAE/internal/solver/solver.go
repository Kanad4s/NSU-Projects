package solver

import "fmt"

func Solve(leftPart []float64, rightPart []float64) []float64{
	n := len(rightPart)
	alpha := make([]float64, n)
	beta := make([]float64, n)
	alpha[0] = 1 / leftPart[0]
	beta[0] = rightPart[0] / leftPart[0]
	for i := 1; i < n; i++ {
		denominator := leftPart[i * n + i] - alpha[i - 1]
		alpha[i] = 1 / denominator
		beta[i] = (rightPart[i] + 1 * beta[i - 1]) / denominator
	}
	x := make([]float64, n)
	x[n - 1] = beta[n - 1]
	for i := n - 2; i >= 0; i-- {
		x[i] = alpha[i] * x[i + 1] + beta[i]
	}
	return x
}

func PrintPart(part []float64, n int, isMatrix bool) {
	if isMatrix {
		for i := 0; i < n; i++ {
			for j := 0; j < n; j++ {
				fmt.Printf("%1.1f ", part[i * n + j])
			}
			fmt.Printf(" \n")
		}
	} else {
		for i := 0; i < n; i++ {
			fmt.Printf("%1.10f \n", part[i])
		}
	}
}