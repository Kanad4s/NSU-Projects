package app

import (
	"client-server/internal/env"
	"client-server/internal/handler/functions"
	"client-server/internal/route"
	"log"

	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/template/html/v2"
	"github.com/jmoiron/sqlx"
)

func Start(database *sqlx.DB) {
	app := fiber.New(fiber.Config{
		Views: setupEngine(),
		ErrorHandler: func(c *fiber.Ctx, err error) error {
			referer := c.Get("Referer", "/")
			return c.Status(fiber.StatusInternalServerError).Render("error", fiber.Map{
				"Message": err.Error(),
				"BackURL": referer,
			})
		},
	})
	app.Static("/img", "../src/img")
	app.Static("/static", "../static")

	route.Setup(app, database)

	log.Fatal(app.Listen(":" + env.GetPort()))
}

func setupEngine() *html.Engine {
	engine := html.New("../views", ".html")
	engine.Reload(true)
	engine.AddFunc("FormatDate", functions.FormatDate)
	engine.AddFunc("FormatDateWithNull", functions.FormatDateWithNull)
	return engine
}
