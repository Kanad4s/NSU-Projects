package labs

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetEquipments(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var equipment []model.EquipmentWithDetails

		query := `
		SELECT e.id, e."название", e."дата_производства",
		       t."название" AS "вид_испытания",
		       l."название" AS "название_лаборатории"
		FROM "Испытательное_оборудование" e
		JOIN "Виды_испытаний" t ON e."вид" = t.id
		JOIN "Лаборатории" l ON e."лаборатория" = l.id
		ORDER BY e.id;
		`

		err := db.Select(&equipment, query)
		if err != nil {
			return err
		}

		return c.Render("labs/equipment", fiber.Map{
			"Title":     "Испытательное оборудование",
			"Equipment": equipment,
		})
	}
}

func GetAddEquipmentForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("labs/addEquipment", fiber.Map{
			"Title": "Добавить испытательное оборудование",
		})
	}
}

func AddEquipment(db *sqlx.DB) fiber.Handler {
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
		return c.Redirect("/labs/equipment")
	}
}

func DeleteEquipment(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Испытательное_оборудование" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/equipment")
	}
}
