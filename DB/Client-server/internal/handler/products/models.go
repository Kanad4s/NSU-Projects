package products

import (
	"strconv"
	"time"

	"client-server/internal/model"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetModels(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var models []model.ProductModel
		err := db.Select(&models, `SELECT id, вид, название, описание, дата_разработки FROM "Модели_изделий" ORDER BY id`)
		if err != nil {
			return err
		}

		var types []model.ModelTypes
		err = db.Select(&types, `SELECT id, название FROM "Виды_изделий" ORDER BY id`)
		if err != nil {
			return err
		}

		typesMap := make(map[int]string)
		for _, t := range types {
			typesMap[t.ID] = t.Name
		}

		return c.Render("products/models", fiber.Map{
			"Title":    "Модели изделий",
			"Models":   models,
			"TypesMap": typesMap,
		})
	}
}

func GetAddModelForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var types []struct {
			ID   int    `db:"id"`
			Name string `db:"название"`
		}
		err := db.Select(&types, `SELECT id, название FROM "Виды_изделий"`)
		if err != nil {
			return err
		}

		return c.Render("products/addModel", fiber.Map{
			"Types": types,
			"Title": "Добавить модель изделия",
		})
	}
}

func AddModel(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		typeID, _ := strconv.Atoi(c.FormValue("type_id"))
		name := c.FormValue("name")
		desc := c.FormValue("description")
		dateStr := c.FormValue("development_date")

		devDate, err := time.Parse("2006-01-02", dateStr)
		if err != nil {
			return fiber.NewError(fiber.StatusBadRequest, "Неверный формат даты")
		}

		_, err = db.Exec(`
			INSERT INTO "Модели_изделий" ("вид", "название", "описание", "дата_разработки")
			VALUES ($1, $2, $3, $4)
		`, typeID, name, desc, devDate)
		if err != nil {
			return err
		}

		return c.Redirect("/products/models")
	}
}

func DeleteModel(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Модели_изделий" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/products/models")
	}
}
