package labs

import (
	"client-server/internal/model"
	"database/sql"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetPerformedTests(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var performedTests []model.PerformedTestWithNames

		query := `
            SELECT 
                t.id,
                tt."название" AS TestName,
                p.id AS ProductName,
                t.дата,
                people."ФИО" AS TesterName,
                e."название" AS EquipmentName
            FROM "Испытания" t
            JOIN "Набор_испытаний" ts ON t.испытание = ts.id
			JOIN "Виды_испытаний" tt ON tt.id = ts."вид_испытания"
            JOIN "Выпускаемые_изделия" p ON t.изделие = p.id
            JOIN "Испытатели" s ON t.испытатель = s.id
            JOIN "Люди" people ON people.id = s.id
            JOIN "Испытательное_оборудование" e ON t.оборудование = e.id
            ORDER BY t.id
        `

		if err := db.Select(&performedTests, query); err != nil {
			return err
		}

		return c.Render("labs/tests", fiber.Map{
			"Title":          "Проведённые испытания",
			"PerformedTests": performedTests,
		})
	}
}

func GetAddPerformedTestForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("labs/addTest", fiber.Map{
			"Title": "Добавить испытателя",
		})
	}
}

func AddPerformedTest(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		fio := c.FormValue("fio")
		birth := c.FormValue("birth")
		children, _ := strconv.Atoi(c.FormValue("children"))
		hire := c.FormValue("hire")
		dismissal := c.FormValue("dismissal")

		_, err := db.Exec(
			`INSERT INTO "Испытания" ("ФИО", "дата_рождения", "кол-во_детей", "дата_устройства", "дата_увольнения") VALUES ($1, $2, $3, $4, $5)`,
			fio, birth, children, hire, sql.NullString{String: dismissal, Valid: dismissal != ""},
		)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/tests")
	}
}

func DeletePerformedTest(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "Испытания" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/labs/tests")
	}
}
