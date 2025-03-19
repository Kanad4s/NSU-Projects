package db

import (
	"fmt"
	"io"
	"os"

	"github.com/jmoiron/sqlx"
)

const (
	PassToDDL = "../db/ddl/"
)

func Drop(db *sqlx.DB) {
	db.Exec(readFile(PassToDDL + "dropAll.sql"))
}

func CreateTables(db *sqlx.DB) {
	createStaff(db)
}

func createStaff(db *sqlx.DB) {
	db.Exec(readFile(PassToDDL + "00_people.sql"))
	db.Exec(readFile(PassToDDL + "01_categoriesET.sql"))
	db.Exec(readFile(PassToDDL + "02_staffET.sql"))
	db.Exec(readFile(PassToDDL + "03_categoriesWorkers.sql"))
	db.Exec(readFile(PassToDDL + "04_workers.sql"))
	db.Exec(readFile(PassToDDL + "05_brigade.sql"))
}

func readFile(fileName string) string {
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
		if err == io.EOF {
			break
		}
		result += string(data[:n])
	}
	return result
}
