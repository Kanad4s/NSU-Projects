package main

import (
	"client-server/internal/app"
	"client-server/internal/db"
)

func main() {
	dbInfo := db.DbInfo{
		Host:    "localhost",
		Port:    5432,
		Name:    "postgres",
		Sslmode: "disable",
	}
	database := db.Connect(dbInfo)
	defer db.Close(database)
	db.Drop(database)
	db.CreateTables(database)
	db.FillData(database)

	app.Start(database)
}
