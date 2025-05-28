package model

import "time"

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

type Equipment struct {
	ID             int       `db:"id"`
	TestType       int       `db:"вид"`
	Name           string    `db:"название"`
	ProductionDate time.Time `db:"дата_производства"`
	Laboratory     int       `db:"лаборатория"`
}

type EquipmentWithDetails struct {
	ID             int       `db:"id"`
	Name           string    `db:"название"`
	ProductionDate time.Time `db:"дата_производства"`
	TestTypeName   string    `db:"вид_испытания"`
	LaboratoryName string    `db:"название_лаборатории"`
}

type TestType struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}

type Test struct {
	ID          int    `db:"id"`
	Model       int    `db:"модель"`
	TestType    int    `db:"вид_испытания"`
	Description string `db:"описание"`
}

type TestWithNames struct {
	ID           int `db:"id"`
	Model        int `db:"модель"`
	ModelName    string
	TestType     int `db:"вид_испытания"`
	TestTypeName string
	Description  string `db:"описание"`
}

type PerformedTest struct {
	ID        int       `db:"id"`
	Test      int       `db:"испытание"`
	Product   int       `db:"изделие"`
	Date      time.Time `db:"дата"`
	Tester    int       `db:"испытатель"`
	Equipment int       `db:"оборудование"`
}

type PerformedTestWithNames struct {
	ID            int `db:"id"`
	TestName      string
	ProductName   string
	Date          time.Time `db:"дата"`
	TesterName    string
	EquipmentName string
}
