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
	Lat float32 
	Lon float32
	Timezone int
	TimezoneOffset int

}

type Current struct {
	Dt         int     `json:"dt"`
	Sunrise    int     `json:"sunrise"`
	Sunset     int     `json:"sunset"`
	Temp       float32 `json:"temp"`
	FeelsLike  float32 `json:"feels_like"`
	Pressure   float32 `json:"pressure"`
	Humidity   int     `json:"humidity"`
	DewPoint   float32 `json:"dew_point"`
	Uvi        float32 `json:"uvi"`
	Clouds     int     `json:"clouds"`
	Visibility int     `json:"visibility"`
	WindSpeed  float32 `json:"wind_speed"`
	WindDeg    int     `json:"wind_deg"`
	WindGust   float32 `json:"wind_gust"`
	Weather    Weather `json:"weather"`
}

type Weather struct {
	Id          int    `json:"id"`
	Main        string `json:"main"`
	Description string `json:"description"`
	Icon        string `json:"icon"`
}

type Alerts struct {
	SenderName  string `json:"sender_name"`
	Event       string `json:"event"`
	End         int    `json:"end"`
	Description string `json:"description"`
}

func Request(coords places.Coords, wg *sync.WaitGroup) {
	defer wg.Done()
	var response Response
	url := buildUrl(coords.Lat, coords.Lon)
	fmt.Println(url)
	resp, err := http.Get(buildUrl(coords.Lat, coords.Lon))
	if err != nil {
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		errorRet := "Error read response body: " + err.Error()
		fmt.Println(errorRet)
		return
	}
	fmt.Printf("body: %+v\n", string(body))

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
	
}