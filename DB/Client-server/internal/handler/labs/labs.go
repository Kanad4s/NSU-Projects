package labs

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetLabs(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var labsWithWorkshops []model.LaboratoryWithWorkshops
		err := db.Select(&labsWithWorkshops, `SELECT id, "название" FROM "Лаборатории" ORDER BY id`)
		if err != nil {
			return err
		}

		for i := range labsWithWorkshops {
			var shops []model.Workshop
			err := db.Select(&shops, `SELECT id, "название", "начальник_цеха" FROM "Цеха" w 
				JOIN "Лаборатория_цех" l ON w.id = l."цех"
				WHERE "лаборатория" = $1 ORDER BY id
				`, labsWithWorkshops[i].ID)
			if err != nil {
				return err
			}
			labsWithWorkshops[i].Workshops = shops
		}

		return c.Render("labs/labs", fiber.Map{
			"Title": "Лаборатории и цеха",
			"Labs":  labsWithWorkshops,
		})
	}
}

func GetAddLabForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("labs/addLab", fiber.Map{
			"Title": "Добавить лабораторию",
		})
	}
}

func AddLab(db *sqlx.DB) fiber.Handler {
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
		return c.Redirect("/labs/labs")
	}
}

func DeleteLab(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Лаборатории" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/labs")
	}
}
