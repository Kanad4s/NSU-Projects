package products

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"
	"time"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetManufactured(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		rows, err := db.Query(`SELECT id, "ФИО", "дата_рождения", "кол-во_детей", "дата_устройства", "дата_увольнения" FROM "Люди"`)
		if err != nil {
			return err
		}
		defer rows.Close()

		var people []model.Person
		for rows.Next() {
			var p model.Person
			var birth, hire time.Time
			var dismissal sql.NullTime
			err := rows.Scan(&p.ID, &p.FIO, &birth, &p.ChildrenCount, &hire, &dismissal)
			if err != nil {
				return err
			}
			p.BirthDate = birth.Format("2006-01-02")
			p.HireDate = hire.Format("2006-01-02")
			if dismissal.Valid {
				date := dismissal.Time.Format("2006-01-02")
				p.DismissalDate = &date
			}
			people = append(people, p)
		}

		return c.Render("people", fiber.Map{
			"Title":  "Список людей",
			"People": people,
		})
	}
}

func GetAddManufacturedForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("staff/addPerson", fiber.Map{
			"Title": "Добавить человека",
		})
	}
}

func AddManufactured(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		fio := c.FormValue("fio")
		birth := c.FormValue("birth")
		children, _ := strconv.Atoi(c.FormValue("children"))
		hire := c.FormValue("hire")
		dismissal := c.FormValue("dismissal")

		_, err := db.Exec(
			`INSERT INTO "Люди" ("ФИО", "дата_рождения", "кол-во_детей", "дата_устройства", "дата_увольнения") VALUES ($1, $2, $3, $4, $5)`,
			fio, birth, children, hire, sql.NullString{String: dismissal, Valid: dismissal != ""},
		)
		if err != nil {
			return err
		}
		return c.Redirect("/staff/people")
	}
}

func DeleteManufactured(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Люди" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/staff/people")
	}
}
