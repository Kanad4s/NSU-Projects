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
	db.Exec(readQuery(PassToDDL + "dropAll.sql"))
}

func CreateTables(db *sqlx.DB) {
	createStaff(db)
	createProducts(db)
	createWorkshops(db)
	createLabs(db)
	createQuerries(db)
}

func createStaff(db *sqlx.DB) {
	db.Exec(readQuery(PassToDDL + "staff.sql"))
}

func createProducts(db *sqlx.DB) {
	db.Exec(readQuery(PassToDDL + "products.sql"))
}

func createWorkshops(db *sqlx.DB) {
	db.Exec(readQuery(PassToDDL + "workshops.sql"))
}

func createLabs(db *sqlx.DB) {
	db.Exec(readQuery(PassToDDL + "labs.sql"))
}

func createQuerries(db *sqlx.DB) {
	db.Exec(readQuery(PassToDDL + "query.sql"))
}

func readQuery(fileName string) string {
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
