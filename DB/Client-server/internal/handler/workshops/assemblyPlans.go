package workshops

import (
	"database/sql"
	"strconv"

	"client-server/internal/model"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetAssemblyPlans(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var plans []model.AssemblyPlanWithNames

		query := `
		SELECT 
				пс.id, 
				м."название" AS "модель_название", 
				вр."название" AS "работа_название", 
				пс."этап"
			FROM "План_сборки" пс
			JOIN "Модели_изделий" м ON пс."модель" = м.id
			JOIN "Виды_работ_на_участке" вр ON пс."работа" = вр.id
			ORDER BY пс.id
		`
		if err := db.Select(&plans, query); err != nil {
			return err
		}

		return c.Render("workshops/assemblyPlans", fiber.Map{
			"Title": "Планы сборки",
			"Plans": plans,
		})
	}
}

func GetAddAssemblyPlanForm(db *sqlx.DB) fiber.Handler {
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

		return c.Render("workshops/addAssemblyPlan", fiber.Map{
			"Categories": categories,
			"Brigades":   brigades,
			"People":     availablePeople,
			"Title":      "Добавить рабочего",
		})
	}
}

func AddAssemblyPlan(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		personID, _ := strconv.Atoi(c.FormValue("person_id"))
		brigadeStr := c.FormValue("brigade_id")
		categoryID, _ := strconv.Atoi(c.FormValue("category_id"))
		physForm := c.FormValue("phys_form")
		size, _ := strconv.Atoi(c.FormValue("size"))

		var brigade sql.NullInt64
		if brigadeStr != "" {
			if bid, err := strconv.Atoi(brigadeStr); err == nil {
				brigade = sql.NullInt64{Int64: int64(bid), Valid: true}
			}
		}

		_, err := db.Exec(`INSERT INTO "Рабочие" ("человек", "бригада", "категория", "физическая_форма", "размер_спецодежды") VALUES ($1, $2, $3, $4, $5)`,
			personID, brigade, categoryID, physForm, size)
		if err != nil {
			return err
		}

		return c.Redirect("/workshops/assemblyPlans")
	}
}

func DeleteAssemblyPlan(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "План_сборки" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/workshops/assemblyPlans")
	}
}
