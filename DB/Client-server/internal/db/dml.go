package db

import (
	"github.com/jmoiron/sqlx"
	_ "github.com/lib/pq"
)

const (
	PassToData = "../db/dml/"
)

func FillData(db *sqlx.DB) {
	fillStaff(db)
	fillProducts(db)
	fillWorkshops(db)
	fillLabs(db)
}

func fillStaff(db *sqlx.DB) {
	db.Exec(readQuery(PassToData + "staff.sql"))
}

func fillProducts(db *sqlx.DB) {
	db.Exec(readQuery(PassToData + "products.sql"))
}

func fillWorkshops(db *sqlx.DB) {
	db.Exec(readQuery(PassToData + "workshops.sql"))
}

func fillLabs(db *sqlx.DB) {
	db.Exec(readQuery(PassToData + "labs.sql"))
}
