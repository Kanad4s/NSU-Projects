package labs

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetTestsList(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var testsList []model.TestWithNames

		query := `
            SELECT 
                tl.id,
                tl.модель,
                m."название" AS ModelName,
                tl.вид_испытания,
                v."название" AS TestTypeName,
                tl.описание
            FROM "Набор_испытаний" tl
            JOIN "Модели_изделий" m ON tl.модель = m.id
            JOIN "Виды_испытаний" v ON tl.вид_испытания = v.id
            ORDER BY tl.id
        `

		err := db.Select(&testsList, query)
		if err != nil {
			return err
		}

		return c.Render("labs/testsList", fiber.Map{
			"Title":     "Набор_испытаний",
			"TestsList": testsList,
		})
	}
}

func GetAddTestListForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("labs/addTestsList", fiber.Map{
			"Title": "Добавить испытателя",
		})
	}
}

func AddTestList(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		fio := c.FormValue("fio")
		birth := c.FormValue("birth")
		children, _ := strconv.Atoi(c.FormValue("children"))
		hire := c.FormValue("hire")
		dismissal := c.FormValue("dismissal")

		_, err := db.Exec(
			`INSERT INTO "Испытания" ("ФИО", "дата_рождения", "кол-во_детей", "дата_устройства", "дата_увольнения") VALUES ($1, $2, $3, $4, $5)`,
			fio, birth, children, hire, sql.NullString{String: dismissal, Valid: dismissal != ""},
		)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/tests")
	}
}

func DeleteTestList(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Испытания" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/tests")
	}
}
