package main

import (
	"fmt"
	"sync"

	"AsyncRequests/internal/cli"
	"AsyncRequests/internal/mistral"
	"AsyncRequests/internal/places"
	"AsyncRequests/internal/weather"
)

// найти интересные места, получить информацию о них в мистрали и погоду там сейчас
func main() {
	mistralKey, request := cli.Parse()
	fmt.Println(mistralKey)
	fmt.Println(request)
	
	var wg sync.WaitGroup
	if len(request) > 0 {
		wg.Add(1)
		go mistral.MistralRequest(request, &wg, mistralKey)
	}

	fmt.Println("Scanning for interesting places")
	places := places.Request()
	placeId := cli.ChoosePlace(len(places.Results))
	fmt.Printf("Selected place № %d, %s\n", placeId, places.Results[placeId].Title)

	request = mistral.BuildPlaceRequest(places.Results[placeId].Title, places.Results[placeId].Address)

	weather.Request(places.Results[placeId].Coords)
	wg.Add(1)
	go mistral.MistralRequest(request, &wg, mistralKey)

	wg.Wait()
}