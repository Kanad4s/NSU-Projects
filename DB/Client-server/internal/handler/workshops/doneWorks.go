package workshops

import (
	"database/sql"
	"strconv"

	"client-server/internal/model"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetDoneWorks(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var works []model.DoneWorksWithNames

		query := `
			SELECT 
				вр.id,
				вр."участок",
				ц."название" AS "цех_название",
				вр."бригада",
				ми."название" AS "изделие_название",
				вр."этап",
				вру."название" AS "работа_название",
				вр."поступление",
				вр."покидание"
			FROM "Выполненные_работы" вр
			JOIN "Участки_цехов" у ON вр."участок" = у.id
			JOIN "Виды_работ_на_участке" вру ON у."вид_работы" = вру.id
			JOIN "Цеха" ц ON у."цех" = ц.id
			JOIN "Выпускаемые_изделия" ви ON вр."изделие" = ви.id
			JOIN "Модели_изделий" ми ON ви."модель" = ми.id
			ORDER BY вр.id
		`
		if err := db.Select(&works, query); err != nil {
			return err
		}

		return c.Render("workshops/doneWorks", fiber.Map{
			"Title": "Выполненные работы",
			"Works": works,
		})
	}
}

func GetAddDoneWorkForm(db *sqlx.DB) fiber.Handler {
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

		return c.Render("workshops/doneWorks", fiber.Map{
			"Categories": categories,
			"Brigades":   brigades,
			"People":     availablePeople,
			"Title":      "Добавить рабочего",
		})
	}
}

func AddDoneWork(db *sqlx.DB) fiber.Handler {
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

		return c.Redirect("/workshops/doneWorks")
	}
}

func DeleteDoneWork(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Выполненные_работы" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/workshops/doneWorks")
	}
}
