package workshops

import (
	"database/sql"
	"strconv"

	"client-server/internal/model"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetAreas(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var areas []model.AreaWithName

		query := `
		SELECT a.id, w."название" as "цех", p."ФИО", tw."название" AS "вид_работы"
		FROM "Участки_цехов" a
		JOIN "Цеха" w ON w.id = a."цех"
		JOIN "ИТ_персонал" m ON a."начальник" = m.id
		JOIN "Люди" p ON p.id = m.id
		JOIN "Виды_работ_на_участке" tw on tw.id = a."вид_работы"
		ORDER BY a.id;
		`

		if err := db.Select(&areas, query); err != nil {
			return err
		}

		for i := range areas {
			var masters []model.Master
			err := db.Select(&masters, `SELECT it.id, p."ФИО" 
				FROM "Участки_цехов" a 
				JOIN "Мастера_участка" m ON m."участок" = a."id"
				JOIN "ИТ_персонал" it ON m."мастер" = it.id
				JOIN "Люди" p ON p.id = it.id
				WHERE m."участок" = $1 ORDER BY id
				`, areas[i].ID)
			if err != nil {
				return err
			}
			areas[i].Masters = masters
		}

		return c.Render("workshops/areas", fiber.Map{
			"Title": "Участки цехов",
			"Areas": areas,
		})
	}
}

func GetAddAreaForm(db *sqlx.DB) fiber.Handler {
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

		return c.Render("workshops/addArea", fiber.Map{
			"Categories": categories,
			"Brigades":   brigades,
			"People":     availablePeople,
			"Title":      "Добавить рабочего",
		})
	}
}

func AddArea(db *sqlx.DB) fiber.Handler {
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

		return c.Redirect("/workshops/areas")
	}
}

func DeleteArea(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Участки_цехов" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/workshops/areas")
	}
}
