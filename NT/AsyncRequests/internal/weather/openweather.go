package weather

import (
	"fmt"
	"AsyncRequests/internal/places"
)
	
type Response struct {
	Lat float32 
	Lon float32
	Timezone string
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

const (
	URL = "https://api.openweathermap.org/data/3.0"
	API_KEY = "89182125333df18a51c2c0b92ed77cdc"

	EXCLUDE_CURRENT = "current"
	EXCLUDE_DAILY = "daily"
)


func Request(coords places.Coords) {
	url := buildUrl(coords.Lat, coords.Lon)
	fmt.Println(url)
}

func buildUrl(lat float32, lot float32) (url string) {
	url = URL + "/onecall?"
	url += "lat="
	url += fmt.Sprintf("%2.2f", lat)
	url += "&lot=" 
	url += fmt.Sprintf("%2.2f", lot)
	url += "&appid=" + API_KEY
	url += "&exclude=" + EXCLUDE_CURRENT
	return
}