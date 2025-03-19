package db

import (
	"fmt"
	"github.com/jmoiron/sqlx"
	"io"
	"os"
)

const (
	PassToDDL = "../db/ddl/"
)

func Drop(db *sqlx.DB) {
	query := ReadFile("Client-server/db/ddl/dropAll.sql")
	Query(query, db)
}

func ReadFile(fileName string) string {
	file, err := os.Open(fileName)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	defer file.Close()

	data := make([]byte, 256)
	result := ""
	for {
		n, err := file.Read(data)
		if err == io.EOF { // если конец файла
			break // выходим из цикла
		}
		result += string(data[:n])
	}
	return result
}
