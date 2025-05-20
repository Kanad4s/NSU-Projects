package handler

import (
	"client-server/internal/model"
	"fmt"
	"strconv"

	"github.com/gofiber/fiber/v2"
	"github.com/jmoiron/sqlx"
)

func GetStaffET(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		rows, err := db.Query(`SELECT id, "человек", "категория", "уровень_допуска", "знания_английского" FROM "ИТ_персонал"`)
		if err != nil {
			return err
		}
		defer rows.Close()

		var staff []model.StaffET
		for rows.Next() {
			var s model.StaffET
			err := rows.Scan(&s.ID, &s.PersonID, &s.CategoryID, &s.AccessLevel, &s.EnglishLevel)
			if err != nil {
				return err
			}
			staff = append(staff, s)
		}

		fmt.Println("People count:", len(staff))
		fmt.Println(staff)
		return c.Render("staff/staffET", fiber.Map{
			"Title":   "Список ИТ персонала",
			"StaffET": staff,
		}, "staffETLayout")
	}
}

func DeleteStaffET(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		id := c.Params("id")
		_, err := db.Exec(`DELETE FROM "ИТ_персонал" WHERE id = $1`, id)
		if err != nil {
			return err
		}
		return c.Redirect("/staff/staffET")
	}
}

func GetAddStaffETForm(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		var people []model.Person
		if err := db.Select(&people, `SELECT id, "ФИО" FROM "Люди" WHERE id NOT IN (SELECT "человек" FROM "ИТ_персонал")`); err != nil {
			return err
		}

		var categories []model.StaffETCategory
		if err := db.Select(&categories, `SELECT id, "название" FROM "Категории_ИТ_персонала"`); err != nil {
			return err
		}

		return c.Render("staff/addStaffET", fiber.Map{
			"Title":      "Добавить ИТ персонал",
			"People":     people,
			"Categories": categories,
		})
	}
}

func AddStaffET(db *sqlx.DB) fiber.Handler {
	return func(c *fiber.Ctx) error {
		personID, err := strconv.Atoi(c.FormValue("person_id"))
		if err != nil {
			return err
		}

		categoryID, err := strconv.Atoi(c.FormValue("category_id"))
		if err != nil {
			return err
		}

		accessLevel := c.FormValue("access_level")
		englishLevel := c.FormValue("english_level")

		_, err = db.Exec(`
			INSERT INTO "ИТ_персонал" ("человек", "категория", "уровень_допуска", "знания_английского")
			VALUES ($1, $2, $3, $4)
		`, personID, categoryID, accessLevel, englishLevel)
		if err != nil {
			return err
		}

		return c.Redirect("/staff/staffET")
	}
}
