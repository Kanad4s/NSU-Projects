package main

import (
	"client-server/internal/db"
	"client-server/internal/handler/functions"
	"log"

	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/template/html/v2"

	"client-server/internal/route"
)

func main() {
	dbInfo := db.DbInfo{
		Host:    "localhost",
		Port:    5432,
		Name:    "postgres",
		Sslmode: "disable",
	}
	database := db.Connect(dbInfo)
	defer db.Close(database)
	db.Drop(database)
	db.CreateTables(database)
	db.FillData(database)

	engine := html.New("../views", ".html")
	engine.Reload(true)
	engine.AddFunc("FormatDate", functions.FormatDate)
	engine.AddFunc("FormatDateWithNull", functions.FormatDateWithNull)
	app := fiber.New(fiber.Config{
		Views: engine,
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

	log.Fatal(app.Listen(":3003"))
}
