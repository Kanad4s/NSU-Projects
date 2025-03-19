package main

import (
	"client-server/internal/db"
)

func main() {
	dbInfo := db.DbInfo{
		Host:    "localhost",
		Port:    5432,
		Name:    "postgres",
		Sslmode: "disable",
	}
	db1 := db.Connect(dbInfo)
	db.Query("select * from professions", db1)
	// db.Query("", db1)
	str := db.ReadFile(db.PassToDDL + "00_people.sql")
	db.Query(str, db1)
	db.Query("select * from Люди", db1)
	db.Close(db1)
}
