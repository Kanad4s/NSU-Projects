package products

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetHelicopters(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var helicopters []model.Helicopter
		err := db.Select(&helicopters, `
			SELECT id, "грузоподъемность", "высота_подъема", "макс_скорость" 
			FROM "Вертолеты"
			ORDER BY id
		`)
		if err != nil {
			return err
		}

		var models []model.ModelTypes
		err = db.Select(&models, `SELECT id, название FROM "Модели_изделий"`)
		if err != nil {
			return err
		}
		modelMap := make(map[int]string)
		for _, m := range models {
			modelMap[m.ID] = m.Name
		}

		return c.Render("products/models/helicopters", fiber.Map{
			"Title":       "Вертолеты",
			"Helicopters": helicopters,
			"ModelMap":    modelMap,
		})
	}
}

func GetAddHelicopterForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("products/models/addHelicopter", fiber.Map{
			"Title": "Добавить вертолет",
		})
	}
}

func AddHelicopter(db *sqlx.DB) fiber.Handler {
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
		return c.Redirect("/products/models/helicopters")
	}
}

func DeleteHelicopter(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Вертолеты" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/products/models/helicopters")
	}
}
