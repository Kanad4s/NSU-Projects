package model

import "time"

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

type AssemblyPlan struct {
	ID    int `db:"id"`
	Model int `db:"модель"`
	Work  int `db:"работа"`
	Stage int `db:"этап"`
}

type AssemblyPlanWithNames struct {
	ID        int    `db:"id"`
	ModelName string `db:"модель_название"`
	WorkName  string `db:"работа_название"`
	Stage     int    `db:"этап"`
}

type DoneWorks struct {
	ID            int        `db:"id"`
	Area          int        `db:"участок"`
	Brigade       int        `db:"бригада"`
	Product       int        `db:"изделие"`
	Stage         int        `db:"этап"`
	ArrivalDate   time.Time  `db:"поступление"`
	DepartureDate *time.Time `db:"покидание"` // Nullable
}

type DoneWorksWithNames struct {
	ID            int        `db:"id"`
	Area          int        `db:"участок"`
	Workshop      string     `db:"цех_название"`
	Brigade       int        `db:"бригада"`
	ProductName   string     `db:"изделие_название"`
	Stage         int        `db:"этап"`
	WorkName      string     `db:"работа_название"`
	ArrivalDate   time.Time  `db:"поступление"`
	DepartureDate *time.Time `db:"покидание"` // Nullable
}
