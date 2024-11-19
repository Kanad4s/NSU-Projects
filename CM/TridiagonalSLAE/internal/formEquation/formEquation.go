package formEquation

import (
	"fmt"
	"TridiagonalSLAE/internal/inputParser"
)

func FormEquation() {
	inputParser.ParseInput()
	n, eps, gamma, mode := inputParser.GetInput()
	fmt.Printf("%v, %v, %v, %v\n", n, eps, gamma, mode)

}

func FormRightPart(n int, eps float64, gamma float64, mode int) []float64 {
	arr := make([]float64, n)
	if mode <= 2 {
		if mode == 1 {
			eps = 0
		}
		for i := 0; i < n; i++ {
			arr[i] = 2 + eps
		}
	} else if mode == 3 {
		for i := 0; i < n; i++ {
			arr[i] = 2 * float64(i + 1 + 1) + gamma
		}
	}
	return arr
}