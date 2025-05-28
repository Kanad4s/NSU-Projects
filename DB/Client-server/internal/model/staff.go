package model

type Person struct {
	ID            int     `db:"id"`
	FIO           string  `db:"ФИО"`
	BirthDate     string  `db:"дата_рождения"`
	ChildrenCount int     `db:"кол-во_детей"`
	HireDate      string  `db:"дата_устройства"`
	DismissalDate *string `db:"дата_увольнения"`
}

type Worker struct {
	ID           int    `db:"id"`
	PersonID     int    `db:"человек"`
	BrigadeID    *int   `db:"бригада"`
	CategoryID   int    `db:"категория"`
	PhysicalForm string `db:"физическая_форма"`
	UniformSize  int    `db:"размер_спецодежды"`
}

type WorkerCategory struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
}

type Brigade struct {
	ID        int    `db:"id"`
	Brigadier string `db:"бригадир"`
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
