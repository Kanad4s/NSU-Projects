package route

import (
	"client-server/internal/handler"
	// "database/sql"
	"github.com/jmoiron/sqlx"

	"github.com/gofiber/fiber/v2"
)

func Setup(app *fiber.App, db *sqlx.DB) {
	setupStaff(app, db)
	app.Get("/", handler.GetPeople(db))
}

func setupStaff(app *fiber.App, db *sqlx.DB) {
	app.Get("/staff/people", handler.GetPeople(db))
	app.Get("/staff/people/add", handler.GetAddPersonForm(db))
	app.Post("/staff/people/add", handler.AddPerson(db))
	app.Get("/staff/delete/:id", handler.DeletePerson(db))

	app.Get("/staff/workers", handler.GetWorkers(db))
	app.Get("/staff/workers/add", handler.GetAddWorkerForm(db))
	app.Post("/staff/workers/add", handler.AddWorker(db))
	app.Get("/staff/workers/delete/:id", handler.DeleteWorker(db))
}
