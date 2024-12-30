package main

import (
	"fmt"
	"sync"

	"AsyncRequests/internal/cli"
	"AsyncRequests/internal/mistral"
	"AsyncRequests/internal/places"
	"AsyncRequests/internal/weather"
)

func main() {
	mistralKey, request := cli.Parse()

	var wg sync.WaitGroup
	if len(request) > 0 {
		wg.Add(1)
		go mistral.MistralRequest(request, &wg, mistralKey)
	}

	city := cli.ChooseCity()
	fmt.Printf("Scanning for interesting places in %s\n", city.Name)
	places := places.RequestPlaces(city)
	places.Print()
	placeId := cli.ChoosePlace(len(places.Results))
	fmt.Printf("Selected place № %d, %s\n", placeId, places.Results[placeId].Title)

	request = mistral.BuildPlaceRequest(places.Results[placeId])

	wg.Add(1)
	go mistral.MistralRequest(request, &wg, mistralKey)
	wg.Add(1)
	go weather.Request(places.Results[placeId].Coords, &wg)

	wg.Wait()
}
