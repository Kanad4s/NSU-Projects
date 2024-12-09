package mistral

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"net/http"
)

const (
	URL = "https://api.mistral.ai/v1/chat/completions"
	MISTRAL_MODEL = "mistral-small-latest"
	MISTRAL_HTTP_CONTENT = "application/json"

	ROLE_USER = "user"
	ROLE_ADMIN = "admin"

	HTTP_METHOD_POST = "POST"
	HTTP_AUTHORIZATION_SCHEME = "Bearer"
	HTTP_CONTENT_TYPE = "Content-Type"
	HTTP_AUTHORIZATION = "Authorization"
)

type Request struct {
	Model string `json:"model"`
	Messages []Message `json:"messages"`
}

type Message struct {
	Role    string `json:"role"`
	Content string `json:"content"`
}

type Response struct {
	ID      string   `json:"id"`
	Object  string   `json:"object"`
	Model   string   `json:"model"`
	Usage   Usage    `json:"usage"`
	Created int64    `json:"created"`
	Choices []Choice `json:"choices"`
}

type Usage struct {
	PromptTokens     int `json:"prompt_tokens"`
	CompletionTokens int `json:"completion_tokens"`
	TotalTokens      int `json:"total_tokens"`
}

type Choice struct {
	Message struct {
		Role    string `json:"role"`
		Content string `json:"content"`
	} `json:"message"`
}

func SendRequest(msg string, mistralKey string) (Response, error ) {
	req := createRequest(msg, mistralKey)
	log.Printf("Request HTTP: %+v\n", req)

	client := &http.Client{}
	response, err := client.Do(req)
	if err != nil {
		log.Fatalf("Error sending POST request: %v", err.Error())
	}
	defer response.Body.Close()
	var resp Response
	if response.StatusCode != http.StatusOK {
		log.Printf("Response status is not OK: %s\n", response.Status)
		return resp, errors.New("Response status is not valid")
	}

	err = json.NewDecoder(response.Body).Decode(&resp)
	if err != nil {
		log.Printf("Error decoding response: %v\n", err.Error())
		return resp, errors.New("error decoding response")
	}
	return resp, nil
}

func createRequest(msg string, mistralKey string) (reqHTTP *http.Request) {
	var request Request
	request.Model = MISTRAL_MODEL
	request.Messages = append(request.Messages, Message{Role: ROLE_USER, Content: msg})
	log.Printf("request: %+v\n", request)
	jsonData, err := json.Marshal(request)
	if err != nil {
		log.Fatalf("ERROR json.Marshal(): %v", err.Error())
	}
	reqHTTP, err = http.NewRequest(HTTP_METHOD_POST, URL, bytes.NewBuffer(jsonData))
	if err != nil {
		log.Fatalf("ERROR http.NewRequest(): %v", err.Error())
	}
	reqHTTP.Header.Set(HTTP_CONTENT_TYPE, MISTRAL_HTTP_CONTENT)
	reqHTTP.Header.Set(HTTP_AUTHORIZATION, HTTP_AUTHORIZATION_SCHEME + " " + mistralKey)
	return
}

func (response Response) Print() {
	for i, val := range response.Choices {
		fmt.Printf("Response %d, \n%s\n", i, val)
	}
}


