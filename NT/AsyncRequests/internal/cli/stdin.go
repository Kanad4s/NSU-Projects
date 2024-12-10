package cli

import "fmt"

func ChoosePlace(placeCount int) (placeId int) {
	fmt.Printf("Please indicate the number of the place you are interested in\n")
scanId:
	fmt.Scanf("%d\n", &placeId)
	if placeId < 0 || placeId >= placeCount {
		fmt.Printf("Place number should be in range [0; %d]\n", placeCount - 1)
		goto scanId
	}
	return
}