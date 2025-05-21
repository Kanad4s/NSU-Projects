package products

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetPlanes(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var planes []model.Plane
		err := db.Select(&planes, `SELECT id, категория, размах_крыла, "кол-во_мест", макс_скорость FROM "Самолеты" ORDER BY id`)
		if err != nil {
			return err
		}

		var categories []model.PlaneCategory
		err = db.Select(&categories, `SELECT id, название FROM "Категории_самолетов"`)
		if err != nil {
			return err
		}

		categoryMap := make(map[int]string)
		for _, cat := range categories {
			categoryMap[cat.ID] = cat.Name
		}

		var models []model.ProductModel
		err = db.Select(&models, `SELECT id, название FROM "Модели_изделий"`)
		if err != nil {
			return err
		}
		modelMap := make(map[int]string)
		for _, m := range models {
			modelMap[m.ID] = m.Name
		}

		return c.Render("products/models/planes", fiber.Map{
			"Title":       "Самолёты",
			"Planes":      planes,
			"CategoryMap": categoryMap,
			"ModelMap":    modelMap,
		})
	}
}

func GetAddPlaneForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("products/models/addPlane", fiber.Map{
			"Title": "Добавить самолет",
		})
	}
}

func AddPlane(db *sqlx.DB) fiber.Handler {
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
		return c.Redirect("products/models/planes")
	}
}

func DeletePlane(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Самолеты" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("products/models/planes")
	}
}
