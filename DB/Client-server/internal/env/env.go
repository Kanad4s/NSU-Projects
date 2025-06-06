package env

import (
	"log"
	"os"

	"github.com/joho/godotenv"
)

const envFile = ".env"

func init() {
	err := godotenv.Load("../" + envFile)
	if err != nil {
		log.Println("No .env file found")
	}
}

func GetPort() string {
	port := os.Getenv("PORT")
	if port == "" {
		port = "3003"
	}

	return port
}
