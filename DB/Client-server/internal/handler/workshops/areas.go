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
		rows, err := db.Query(`SELECT id, человек, бригада, категория, физическая_форма, размер_спецодежды FROM "Рабочие" order by id`)
		if err != nil {
			return err
		}
		defer rows.Close()

		var workers []model.Worker
		for rows.Next() {
			var w model.Worker
			var brigade sql.NullInt64
			err := rows.Scan(&w.ID, &w.PersonID, &brigade, &w.CategoryID, &w.PhysicalForm, &w.UniformSize)
			if err != nil {
				return err
			}
			if brigade.Valid {
				b := int(brigade.Int64)
				w.BrigadeID = &b
			}
			workers = append(workers, w)
		}

		var categories []model.StaffETCategory
		err = db.Select(&categories, "SELECT id, название FROM Категории_рабочих")
		if err != nil {
			return err
		}

		categoryMap := make(map[int]string)
		for _, cat := range categories {
			categoryMap[cat.ID] = cat.Name
		}

		var people []model.Person
		err = db.Select(&people, `
			SELECT p.id, p.ФИО
			FROM Люди p
			JOIN Рабочие w ON  w.человек = p.id
		`)
		if err != nil {
			return err
		}

		peopleMap := make(map[int]string)
		for _, p := range people {
			peopleMap[p.ID] = p.FIO
		}

		return c.Render("staff/workers", fiber.Map{
			"Title":       "Рабочие",
			"Workers":     workers,
			"PeopleMap":   peopleMap,
			"CategoryMap": categoryMap,
		}, "workersLayout")
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

		return c.Redirect("/staff/workers")
	}
}

func DeleteArea(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Рабочие" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/staff/workers")
	}
}
