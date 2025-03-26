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
	connDB := db.Connect(dbInfo)
	db.Drop(connDB)
	db.CreateTables(connDB)
	db.FillData(connDB)
	db.Close(connDB)
}
