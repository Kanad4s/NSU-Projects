package workshops

import (
	"strconv"

	"client-server/internal/model"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetWorkshops(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var workshopsWithLabs []model.WorkshopWithLabs

		query := `
		SELECT d.id, d."название", p."ФИО" AS Boss
		FROM "Цеха" d
		JOIN "ИТ_персонал" m ON d."начальник_цеха" = m.id
		JOIN "Люди" p ON p.id = m.id
		ORDER BY d.id;
		`

		if err := db.Select(&workshopsWithLabs, query); err != nil {
			return err
		}

		for i := range workshopsWithLabs {
			var labs []model.Laboratory
			err := db.Select(&labs, `SELECT l.id, l."название" FROM "Лаборатории" l 
				JOIN "Лаборатория_цех" lc ON l.id = lc."лаборатория"
				WHERE lc."цех" = $1 ORDER BY id
				`, workshopsWithLabs[i].ID)
			if err != nil {
				return err
			}
			workshopsWithLabs[i].Laboratories = labs
		}

		return c.Render("workshops/workshops", fiber.Map{
			"Title":     "Цеха",
			"Workshops": workshopsWithLabs,
		})
	}
}

func GetAddWorkshopForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var categories []model.WorkerCategory
		if err := db.Select(&categories, `SELECT id, "название" FROM "Категории_рабочих"`); err != nil {
			return err
		}

		var brigades []model.Brigade
		query := `
            SELECT b.id, p.ФИО AS brigadier_name
            FROM "Бригады" b
            JOIN "Рабочие" w ON w.id = b.бригадир
            JOIN "Люди" p ON p.id = w.id
            ORDER BY b.id;
        `
		if err := db.Select(&brigades, query); err != nil {
			return err
		}

		var availablePeople []model.Person
		err := db.Select(&availablePeople, `
			SELECT id, ФИО
			FROM Люди
			WHERE дата_увольнения IS NULL AND id NOT IN (
				SELECT человек FROM ИТ_персонал
				UNION
				SELECT человек FROM Рабочие
			) 
		`)
		if err != nil {
			return err
		}

		return c.Render("workshops/addWorkshop", fiber.Map{
			"Categories": categories,
			"Brigades":   brigades,
			"People":     availablePeople,
			"Title":      "Добавить рабочего",
		})
	}
}

func AddWorkshop(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		name := c.FormValue("name")
		managerID, err := strconv.Atoi(c.FormValue("manager"))
		if err != nil {
			return err
		}

		_, err = db.Exec(
			`INSERT INTO "Цеха" ("название", "начальник_цеха") VALUES ($1, $2)`,
			name, managerID,
		)
		if err != nil {
			return err
		}

		return c.Redirect("/workshops/workshops")
	}
}

func DeleteWorkshop(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Цеха" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/workshops/workshops")
	}
}
