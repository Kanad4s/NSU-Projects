package model

type Laboratory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}

type LaboratoryWithWorkshops struct {
	ID        int    `db:"id"`
	Name      string `db:"название"`
	Workshops []Workshop
}

type Tester struct {
	ID           int `db:"id"`
	LaboratoryID int `db:"лаборатория"`
}

type TesterWithLab struct {
	ID           int     `db:"id"`
	Name         string  `db:"ФИО"`
	LaboratoryID int     `db:"лаборатория"`
	LabName      *string `db:"название_лаборатории"`
}
