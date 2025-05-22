package model

type Laboratory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}

type LaboratoryWithWorkshops struct {
	ID        int        `db:"id"`
	Name      string     `db:"название"`
	Workshops []Workshop // вручную добавим после запроса
}
