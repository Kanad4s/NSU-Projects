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
	URL_PLACES = "https://kudago.com/public-api/v1.4/places"
	URL_CITIES = "https://kudago.com/public-api/v1.4/locations"

	FIELD_LOCATION = "location"
	FIELD_ID       = "id"
	FIELD_ADDRESS  = "address"
	FIELD_TITLE    = "title"
	FIELD_COORDS   = "coords"

	FIELD_LANG    = "lang"
	FIELD_LANG_EN = "en"

	FIELD_LOCATION_NSK = "nsk"

	HTTP_METHOD_POST          = "POST"
	HTTP_AUTHORIZATION_SCHEME = "Bearer"
	HTTP_CONTENT_TYPE         = "Content-Type"
	HTTP_AUTHORIZATION        = "Authorization"
)

type ResponsePlaces struct {
	Count    int     `json:"count"`
	Next     string  `json:"next"`
	Previous string  `json:"previous"`
	Results  []Field `json:"results"`
}

type ResponseCities struct {
	Cities []City
}

type City struct {
	Slug string `json:"slug"`
	Name string `json:"name"`
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

func RequestPlaces(city City) (response ResponsePlaces) {
	body, err := sendRequest(buildPlacesUrl(city))
	if err != nil {
		log.Printf("Error sending request: %v\n", err.Error())
		return
	}
	response, err = parseResponsePlaces(body)
	if err != nil {
		log.Printf("Error parse request: %v\n", err.Error())
		return
	}
	return
}

func RequestCities() (response ResponseCities) {
	body, err := sendRequest(buildCitiesUrl())
	if err != nil {
		log.Printf("Error sending request: %v\n", err.Error())
		return
	}
	response, err = parseResponseCities(body)
	if err != nil {
		log.Printf("Error parse request: %v\n", err.Error())
		return
	}
	return
}

func sendRequest(url string) ([]byte, error) {
	resp, err := http.Get(url)
	if err != nil {
	}
	defer resp.Body.Close()
	// fmt.Println(resp)
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		errorRet := "Error read response body: " + err.Error()
		return body, errors.New(errorRet)
	}
	// fmt.Printf("body: %+v\n", string(body))

	if resp.StatusCode != http.StatusOK {
		errorRet := "error response status code: " + strconv.Itoa(resp.StatusCode)
		return body, errors.New(errorRet)
	}

	return body, nil
}

func parseResponseCities(body []byte) (ResponseCities, error) {
	var response ResponseCities
	err := json.Unmarshal(body, &response.Cities)
	if err != nil {
		errorRet := "Error parse JSON: " + err.Error()
		return response, errors.New(errorRet)
	}

	// fmt.Printf("Parsed json: \n%v\n", response)
	return response, nil
}

func parseResponsePlaces(body []byte) (ResponsePlaces, error) {
	var response ResponsePlaces
	err := json.Unmarshal(body, &response)
	if err != nil {
		errorRet := "Error parse JSON: " + err.Error()
		return response, errors.New(errorRet)
	}

	// fmt.Printf("Parsed json: \n%v\n", response)
	return response, nil
}

func buildPlacesUrl(city City) (url string) {
	url = URL_PLACES
	url += "/?"
	url = addUrlFields(url, FIELD_ID, FIELD_ADDRESS, FIELD_TITLE, FIELD_LOCATION, FIELD_COORDS)
	url = addRequestField(url, FIELD_LOCATION, city.Slug)
	return
}

func buildCitiesUrl() (url string) {
	url = URL_CITIES
	url += "/?"
	url = addRequestField(url, FIELD_LANG, FIELD_LANG_EN)
	return
}

func addUrlFields(url string, fields ...string) string {
	if len(fields) != 0 {
		url += "&fields="
	}
	for i, field := range fields {
		url += field
		if i < len(fields)-1 {
			url += ","
		}
	}
	return url
}

func addRequestField(url string, fieldName string, fieldValue string) string {
	return url + "&" + fieldName + "=" + fieldValue
}

func (response ResponsePlaces) Print() {
	for i, val := range response.Results {
		fmt.Printf("Place %d:\n\tType: %v\n\tLocation: %v\n\tCoords:%f, %f\n", i, val.Title, val.Address, val.Coords.Lat, val.Coords.Lon)
	}
}

func (response ResponseCities) Print() {
	for i, val := range response.Cities {
		fmt.Printf("id: %d, name: %s\n", i, val.Name)
	}
}
