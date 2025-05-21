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

type Plane struct {
	ID         int     `db:"id"`
	CategoryID int     `db:"категория"`
	WingSpan   float64 `db:"размах_крыла"`
	SeatCount  int     `db:"кол-во_мест"`
	MaxSpeed   int     `db:"макс_скорость"`
}

type PlaneCategory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}

type Rocket struct {
	ID         int     `db:"id"`
	CategoryID int     `db:"категория"`
	Caliber    int     `db:"калибр"`
	Power      float64 `db:"мощность"`
	Range      int     `db:"дальность"`
	Weight     float64 `db:"вес"`
	MaxSpeed   int     `db:"макс_скорость"`
}

type RocketCategory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}
