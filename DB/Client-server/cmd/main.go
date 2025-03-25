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
	db.Drop(db1)
	db.CreateTables(db1)
	db.Close(db1)
}
