package staff

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"
	"time"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetPeople(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		rows, err := db.Query(`
		SELECT 
			id, 
			"ФИО", 
			"дата_рождения", 
			"кол-во_детей", 
			"дата_устройства", 
			"дата_увольнения" 
		FROM "Люди" 
		ORDER BY id`)
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

func GetAddPersonForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("staff/people/addPerson", fiber.Map{
			"Title": "Добавить человека",
		})
	}
}

func AddPerson(db *sqlx.DB) fiber.Handler {
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

func DeletePerson(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Люди" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/staff/people")
	}
}

func GetEditPersonForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")

		var p model.Person
		var birth, hire time.Time
		var dismissal sql.NullTime

		err := db.QueryRow(`SELECT id, "ФИО", "дата_рождения", "кол-во_детей", "дата_устройства", "дата_увольнения" FROM "Люди" WHERE id = $1`, id).
			Scan(&p.ID, &p.FIO, &birth, &p.ChildrenCount, &hire, &dismissal)
		if err != nil {
			return err
		}

		p.BirthDate = birth.Format("2006-01-02")
		p.HireDate = hire.Format("2006-01-02")
		if dismissal.Valid {
			d := dismissal.Time.Format("2006-01-02")
			p.DismissalDate = &d
		}

		return c.Render("staff/people/editPerson", fiber.Map{
			"Title":  "Редактировать человека",
			"Person": p,
		})
	}
}

func EditPerson(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		fio := c.FormValue("fio")
		birth := c.FormValue("birth")
		children, _ := strconv.Atoi(c.FormValue("children"))
		hire := c.FormValue("hire")
		dismissal := c.FormValue("dismissal")

		_, err := db.Exec(
			`UPDATE "Люди" SET "ФИО"=$1, "дата_рождения"=$2, "кол-во_детей"=$3, "дата_устройства"=$4, "дата_увольнения"=$5 WHERE id=$6`,
			fio, birth, children, hire, sql.NullString{String: dismissal, Valid: dismissal != ""}, id,
		)
		if err != nil {
			return err
		}

		return c.Redirect("/staff/people")
	}
}
