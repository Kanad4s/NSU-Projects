package model

import "time"

type ProductModel struct {
	ID              int       `db:"id"`
	TypeID          int       `db:"вид"`
	Name            string    `db:"название"`
	Description     string    `db:"описание"`
	DevelopmentDate time.Time `db:"дата_разработки"`
}

type ModelTypes struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}
