package model

type Workshop struct {
	ID   int    `db:"id"`
	Name string `db:"название"`
	Boss string `db:"начальник_цеха"`
}
