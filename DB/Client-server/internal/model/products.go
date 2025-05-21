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

type Planes struct {
	ID         int     `db:"id"`
	CategoryID int     `db:"категория"`
	WingSpan   float64 `db:"размах_крыла"`
	SeatCount  int     `db:"кол-во_мест"`
	MaxSpeed   int     `db:"макс_скорость"`
}

type PlanesCategory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}
