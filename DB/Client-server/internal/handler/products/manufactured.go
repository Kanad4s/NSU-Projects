package products

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetManufactured(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var products []model.ProducedProduct
		err := db.Select(&products, `
			SELECT id, "модель", "начало_производства", "дата_выпуска"
			FROM "Выпускаемые_изделия"
			ORDER BY id
		`)
		if err != nil {
			return err
		}

		var models []model.ModelTypes
		err = db.Select(&models, `SELECT id, "название" FROM "Модели_изделий"`)
		if err != nil {
			return err
		}
		modelMap := make(map[int]string)
		for _, m := range models {
			modelMap[m.ID] = m.Name
		}

		return c.Render("products/manufactured", fiber.Map{
			"Title":    "Выпускаемые изделия",
			"Products": products,
			"ModelMap": modelMap,
		})
	}
}

func GetAddManufacturedForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("products/addManufactured", fiber.Map{
			"Title": "Добавить изделие",
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
		return c.Redirect("/products/manufactured")
	}
}

func DeleteManufactured(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Выпускаемые_изделия" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/products/manufactured")
	}
}
