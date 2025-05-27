package model

type Workshop struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
	Boss string `db:"начальник_цеха"`
}

type WorkshopWithLabs struct {
	ID           int    `db:"id"`
	Name         string `db:"название"`
	Boss         string
	Laboratories []Laboratory
}

type Area struct {
	ID       int    `db:"id"`
	Workshop string `db:"цех"`
	Boss     string `db:"начальник"`
	Work     string `db:"вид_работы"`
}

type AreaWithName struct {
	ID           int    `db:"id"`
	WorkshopName string `db:"цех"`
	BossName     string `db:"ФИО"`
	WorkName     string `db:"вид_работы"`
	Masters      []Master
}

type Master struct {
	ID   int    `db:"id"`
	Name string `db:"ФИО"`
}
