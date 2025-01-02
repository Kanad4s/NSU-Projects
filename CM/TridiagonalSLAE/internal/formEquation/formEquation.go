package formEquation

import (
	"fmt"
	"TridiagonalSLAE/internal/inputParser"
)

func FormEquation() (leftPart []float64, rightPart []float64){
	inputParser.ParseInput()
	n, eps, gamma, mode := inputParser.GetInput()
	fmt.Printf("%v, %v, %v, %v\n", n, eps, gamma, mode)
	leftPart = FormLeftPart(n, gamma, mode)
	rightPart = FormRightPart(n, eps, gamma, mode)
	return
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

func FormLeftPart(n int, gamma float64, mode int) []float64 {
	arr := make([]float64, n * n)
	if mode == 3 {
		for i := 0; i < n; i++ {
			for j := 0; j < n; j++ {
				if i == j {
					arr[i * n + j] = float64(2 * (i + 1)) + gamma
				} else if (j == i + 1) || (j == i - 1){
					arr[i * n + j] = -1
				} else {
					arr[i * n + j] = 0
				}
			}
		}
	} else {
		for i := 0; i < n; i++ {
			for j := 0; j < n; j++ {
				if i == j {
					arr[i * n + j] = 2
				} else if (j == i + 1) || (j == i - 1){
					arr[i * n + j] = -1
				} else {
					arr[i * n + j] = 0
				}
			}
		}
	}
	return arr
}