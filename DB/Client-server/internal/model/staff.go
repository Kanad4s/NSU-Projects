package model

type Person struct {
	ID            int
	FIO           string
	BirthDate     string
	ChildrenCount int
	HireDate      string
	DismissalDate *string
}

type Worker struct {
	ID           int
	PersonID     int
	BrigadeID    *int
	CategoryID   int
	PhysicalForm string
	UniformSize  int
}

type WorkerCategory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}

type Brigade struct {
	ID        int    `db:"id"`
	Brigadier string `db:"brigadier_name"`
}

type StaffET struct {
	ID           int    `db:"id"`
	PersonID     int    `db:"человек"`
	CategoryID   int    `db:"категория"`
	AccessLevel  string `db:"уровень_допуска"`
	EnglishLevel string `db:"знания_английского"`
}

type StaffETCategory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}
