package cli

import (
	"AsyncRequests/internal/places"
	"fmt"
)

func ChooseCity() (city places.City) {
	fmt.Printf("Please select city id, where you want to find interesting places\n")
	response := places.RequestCities()
	response.Print()
	cityId := -1
scanPlace:
	fmt.Scanf("%d\n", &cityId)
	if cityId < 0 || cityId >= len(response.Cities) {
		fmt.Printf("Place number should be in range [0; %d]\n", len(response.Cities)-1)
		goto scanPlace
	}
	return response.Cities[cityId]
}

func ChoosePlace(placeCount int) (placeId int) {
	placeId = -1
	fmt.Printf("Please indicate the number of the place you are interested in\n")
scanId:
	fmt.Scanf("%d\n", &placeId)
	if placeId < 0 || placeId >= placeCount {
		fmt.Printf("Place number should be in range [0; %d]\n", placeCount-1)
		goto scanId
	}
	return
}
