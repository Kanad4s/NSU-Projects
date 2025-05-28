package labs

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetTesters(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var testers []model.TesterWithLab

		query := `
		SELECT it.id, p."ФИО", t."лаборатория", l."название" as "название_лаборатории"
		FROM "Испытатели" t
		JOIN "ИТ_персонал" it ON t.id = it.id
		JOIN "Люди" p on it."человек" = p.id
		LEFT JOIN "Лаборатории" l ON t."лаборатория" = l.id
		ORDER BY it.id
		`

		err := db.Select(&testers, query)
		if err != nil {
			return err
		}

		return c.Render("labs/testers", fiber.Map{
			"Title":   "Испытатели",
			"Testers": testers,
		})
	}
}

func GetAddTesterForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("labs/addTester", fiber.Map{
			"Title": "Добавить испытателя",
		})
	}
}

func AddTester(db *sqlx.DB) fiber.Handler {
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
		return c.Redirect("/labs/testers")
	}
}

func DeleteTester(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Испытатели" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/testers")
	}
}
