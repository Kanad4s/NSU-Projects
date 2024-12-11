package places

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"log"
	"net/http"
	"strconv"
)

const (
	URL = "https://kudago.com/public-api/v1.4/places"

	FIELD_LOCATION = "location"
	FIELD_ID = "id"
	FIELD_ADDRESS = "address"
	FIELD_TITLE = "title"
	FIELD_COORDS = "coords"

	FIELD_LOCATION_NSK = "nsk"

	HTTP_METHOD_POST = "POST"
	HTTP_AUTHORIZATION_SCHEME = "Bearer"
	HTTP_CONTENT_TYPE = "Content-Type"
	HTTP_AUTHORIZATION = "Authorization"
)

type Response struct {
	Count    int     `json:"count"`
	Next     string  `json:"next"`
	Previous string  `json:"previous"`
	Results  []Field `json:"results"`
}

type Field struct {
	Id       int    `json:"id"`
	Title    string `json:"title"`
	Address  string `json:"address"`
	Location string `json:"location"`
	Coords   Coords `json:"coords"`
}

type Coords struct {
	Lat float32 `json:"lat"`
	Lon float32 `json:"lon"`
}

func Request() (response Response) {
	response, err := sendRequest()
	if err != nil {
		log.Printf("Error sending request: %v\n", err.Error())
		return 
	}
	response.Print()
	return
}

func sendRequest() (Response, error) {
	var response Response
	resp, err := http.Get(buildUrl())
	if err != nil {
	}
	defer resp.Body.Close()
	// fmt.Println(resp)

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		errorRet := "Error read response body: " + err.Error()
		return response, errors.New(errorRet)
	}
	fmt.Printf("body: %+v\n", string(body))

	if resp.StatusCode != http.StatusOK {
		errorRet := "error response status code: " + strconv.Itoa(resp.StatusCode)
		return response, errors.New(errorRet)
	}

	err = json.Unmarshal(body, &response)
	if err != nil {
		errorRet := "Error parse JSON: " + err.Error()
		return response, errors.New(errorRet)
	}

	// fmt.Printf("Parsed json: \n%v\n", response)
	return response, nil
}

func buildUrl() (url string) {
	url = URL
	url += "/?"
	url = addUrlFields(url, FIELD_ID, FIELD_ADDRESS, FIELD_TITLE, FIELD_LOCATION, FIELD_COORDS)
	url = addRequestField(url, FIELD_LOCATION, FIELD_LOCATION_NSK)
	return
}

func addUrlFields(url string, fields ...string) string {
	if len(fields) != 0 {
		url += "&fields="
	}
	for i, field := range fields {
		url += field
		if i < len(fields) - 1 {
			url += ","
		}
	}
	return url
}

func addRequestField(url string, fieldName string, fieldValue string) string {
	return url + "&" + fieldName + "=" + fieldValue
}

func (response Response) Print() {
	for i, val := range response.Results {
		fmt.Printf("Place %d:\n\tType: %v\n\tLocation: %v\n\tCoords:%f, %f\n", i, val.Title, val.Address, val.Coords.Lat, val.Coords.Lon)
	}
}







