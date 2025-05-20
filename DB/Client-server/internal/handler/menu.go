package handler

import (
	"github.com/gofiber/fiber/v2"
)

func GetMenu() fiber.Handler {
	return func(c *fiber.Ctx) error {
		return c.Render("index", fiber.Map{
			"Title": "Главная",
		})
	}
}
