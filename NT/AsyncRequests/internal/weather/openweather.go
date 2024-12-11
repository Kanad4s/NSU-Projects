package weather

import (
	"AsyncRequests/internal/places"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"strconv"
	"sync"
)

const (
	URL = "https://api.openweathermap.org/data/2.5/weather"
	API_KEY = "795a48eb5a6349b996542681139cc878"

	EXCLUDE_CURRENT = "current"
	EXCLUDE_DAILY = "daily"

	UNITS_METRIC = "metric"
)
	
type Response struct {
	Coord          Coord   `json:"coord"`
	Weather        []Weather `json:"weather"`
	Base           string  `json:"base"`
	Main           Main    `json:"main"`
	Visibility     int     `json:"visibility"`
	Wind 		   Wind    `json:"wind"`
	Snow           Snow    `json:"snow"`
	Clouds         Clouds  `json:"clouds"`
	Dt             int     `json:"dt"`
	Sys            Sys     `json:"current"`
	Timezone       int     `json:"timezone"`
	Id             int     `json:"id"`
	Name           string  `json:"name"`
	Cod            int     `json:"cod"`
}

type Coord struct {
	Lon            float32 `json:"lon"`
	Lat            float32 `json:"lat"`
}

type Main struct {
	Temp       float32 `json:"temp"`
	FeelsLike  float32 `json:"feels_like"`
	TempMin    float32 `json:"temp_min"`
	TempMax    float32 `json:"temp_max"`
	Pressure   float32 `json:"pressure"`
	Humidity   int     `json:"humidity"`
	SeaLevel   int     `json:"sea_level"`
	GrndLevel  int     `json:"grnd_level"`
}

type Wind struct {
	Speed  float32 `json:"speed"`
	Deg    int     `json:"deg"`
	Gust   float32 `json:"gust"`
}

type Snow struct {
	Hour float32 `json:"1h"` 
}

type Clouds struct {
	All int `json:"all"` 
}

type Sys struct {
	Type       int     `json:"type"`
	Id         int     `json:"id"`
	Country    string  `json:"country"`
	Sunrise    int     `json:"sunrise"`
	Sunset     int     `json:"sunset"`
}

type Weather struct {
	Id          int    `json:"id"`
	Main        string `json:"main"`
	Description string `json:"description"`
	Icon        string `json:"icon"`
}

func Request(coords places.Coords, wg *sync.WaitGroup) {
	defer wg.Done()
	var response Response
	url := buildUrl(coords.Lat, coords.Lon)
	fmt.Println(url)
	resp, err := http.Get(buildUrl(coords.Lat, coords.Lon))
	if err != nil {
	}
	fmt.Println(resp)
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		errorRet := "Error read response body: " + err.Error()
		fmt.Println(errorRet)
		return
	}
	// fmt.Printf("body: %+v\n", string(body))

	if resp.StatusCode != http.StatusOK {
		errorRet := "error response status code: " + strconv.Itoa(resp.StatusCode)
		fmt.Println(errorRet)
		return
	}

	err = json.Unmarshal(body, &response)
	if err != nil {
		errorRet := "Error parse JSON: " + err.Error()
		fmt.Println(errorRet)
		return
	}

	response.print()
}

func buildUrl(lat float32, lon float32) (url string) {
	url = URL + "?"
	url += "lat="
	url += fmt.Sprintf("%2.2f", lat)
	url += "&lon=" 
	url += fmt.Sprintf("%2.2f", lon)
	url += "&exclude=" + EXCLUDE_CURRENT
	url += "&units=" + UNITS_METRIC
	url += "&appid=" + API_KEY
	return
}

func (response Response) print() {
	fmt.Printf("WEATHER:\n\tТемпература: %.2f, ощущается как %.2f\n\tВетер: %.2f м/с\n\tКрасочное описание дня: %s",
	response.Main.Temp, response.Main.FeelsLike, response.Wind.Speed, response.Weather[0].Description)
}