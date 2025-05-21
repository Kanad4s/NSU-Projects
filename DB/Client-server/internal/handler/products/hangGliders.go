package products

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetHangGliders(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var gliders []model.HangGlider
		err := db.Select(&gliders, `
			SELECT id, "вес", "макс_скорость", "угол_веера"
			FROM "Дельтапланы"
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

		return c.Render("products/models/hangGliders", fiber.Map{
			"Title":       "Дельтапланы",
			"HangGliders": gliders,
			"ModelMap":    modelMap,
		})
	}
}

func GetAddHangGliderForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("products/models/addHangGliders", fiber.Map{
			"Title": "Добавить дельтаплан",
		})
	}
}

func AddHangGlider(db *sqlx.DB) fiber.Handler {
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
		return c.Redirect("products/models/hangGliders")
	}
}

func DeleteHangGlider(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Дельтапланы" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("products/models/hangGliders")
	}
}
