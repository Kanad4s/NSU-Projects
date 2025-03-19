package db

import (
	"fmt"
	"log/slog"

	"github.com/jmoiron/sqlx"
	_ "github.com/lib/pq"
)

const (
	host     = "localhost"
	port     = 5432
	user     = "kanad4s"
	password = ""
	dbname   = "postgres"
)

type Response struct {
	Id     int    `db:"w_id"`
	Name   string `db:"w_name"`
	Salary int    `db:"w_salary"`
}

type DbInfo struct {
	Host    string
	Port    int
	Name    string
	Sslmode string
}

func Connect(dbInfo DbInfo) (db *sqlx.DB) {
	info := fmt.Sprintf("host=%s port=%d dbname=%s sslmode=%s",
		dbInfo.Host, dbInfo.Port, dbInfo.Name, dbInfo.Sslmode)
	db, err := sqlx.Connect("postgres", info)
	if err != nil {
		slog.Error(err.Error())
		panic(err)
	}
	// defer db.Close()
	slog.Info("DB connected:", slog.String("dbinfo", info))
	return
}

func Query(request string, db *sqlx.DB) {
	rows, err := db.Query(request)
	if err != nil {
		slog.Error(err.Error())
		panic(err)
	}

	fmt.Println(rows)

	response := make([]Response, 0, 5)
	if err := sqlx.StructScan(rows, &response); err != nil {
		slog.Error(err.Error())
	}

	slog.Info("response of: " + request + " parsed")
	fmt.Println(response)
}

func Select(request string, db *sqlx.DB) {
	response := make([]Response, 0, 5)
	err := db.Select(&response, request)
	if err != nil {
		slog.Error(err.Error())
		panic(err)
	}
	slog.Info("response of: " + request + " parsed")
	fmt.Printf("%v\n", response)
}

func Close(db *sqlx.DB) {
	db.Close()
}
