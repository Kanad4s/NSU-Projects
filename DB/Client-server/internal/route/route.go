package route

import (
	"client-server/internal/handler/labs"
	"client-server/internal/handler/products"
	"client-server/internal/handler/staff"
	"client-server/internal/handler/workshops"

	"github.com/jmoiron/sqlx"

	"github.com/gofiber/fiber/v2"
)

func Setup(app *fiber.App, db *sqlx.DB) {
	app.Get("/", staff.GetMenu())
	setupStaff(app, db)
	setupProducts(app, db)
	setupLabs(app, db)
	setupWorkshops(app, db)
}

func setupStaff(app *fiber.App, db *sqlx.DB) {
	app.Get("/staff/people", staff.GetPeople(db))
	app.Get("/staff/people/add", staff.GetAddPersonForm(db))
	app.Post("/staff/people/add", staff.AddPerson(db))
	app.Get("/staff/people/delete/:id", staff.DeletePerson(db))
	app.Get("/staff/people/edit/:id", staff.GetEditPersonForm(db))
	app.Post("/staff/people/edit/:id", staff.EditPerson(db))

	app.Get("/staff/workers", staff.GetWorkers(db))
	app.Get("/staff/workers/add", staff.GetAddWorkerForm(db))
	app.Post("/staff/workers/add", staff.AddWorker(db))
	app.Get("/staff/workers/delete/:id", staff.DeleteWorker(db))

	app.Get("/staff/staffET", staff.GetStaffET(db))
	app.Get("/staff/staffET/add", staff.GetAddStaffETForm(db))
	app.Post("/staff/staffET/add", staff.AddStaffET(db))
	app.Get("/staff/staffET/delete/:id", staff.DeleteStaffET(db))
}

func setupProducts(app *fiber.App, db *sqlx.DB) {
	app.Get("/products/models", products.GetModels(db))
	app.Get("/products/models/add", products.GetAddModelForm(db))
	app.Post("/products/models/add", products.AddModel(db))
	app.Get("/products/models/delete/:id", products.DeleteModel(db))

	app.Get("/products/manufactured", products.GetManufactured(db))
	app.Get("/products/manufactured/add", products.GetAddManufacturedForm(db))
	app.Post("/products/manufactured/add", products.AddManufactured(db))
	app.Get("/products/manufactured/delete/:id", products.DeleteManufactured(db))

	app.Get("/products/models/planes", products.GetPlanes(db))
	app.Get("/products/models/planes/add", products.GetAddPlaneForm(db))
	app.Post("/products/models/planes/add", products.AddPlane(db))
	app.Get("/products/models/planes/delete/:id", products.DeletePlane(db))

	app.Get("/products/models/rockets", products.GetRockets(db))
	app.Get("/products/models/rockets/add", products.GetAddRocketForm(db))
	app.Post("/products/models/rockets/add", products.AddRocket(db))
	app.Get("/products/models/rockets/delete/:id", products.DeleteRocket(db))

	app.Get("/products/models/helicopters", products.GetHelicopters(db))
	app.Get("/products/models/helicopters/add", products.GetAddHelicopterForm(db))
	app.Post("/products/models/helicopters/add", products.AddHelicopter(db))
	app.Get("/products/models/helicopters/delete/:id", products.DeleteHelicopter(db))

	app.Get("/products/models/gliders", products.GetGliders(db))
	app.Get("/products/models/gliders/add", products.GetAddGliderForm(db))
	app.Post("/products/models/gliders/add", products.AddGlider(db))
	app.Get("/products/models/gliders/delete/:id", products.DeleteGlider(db))

	app.Get("/products/models/hangGliders", products.GetHangGliders(db))
	app.Get("/products/models/hangGliders/add", products.GetAddHangGliderForm(db))
	app.Post("/products/models/hangGliders/add", products.AddHangGlider(db))
	app.Get("/products/models/hangGliders/delete/:id", products.DeleteHangGlider(db))
}

func setupLabs(app *fiber.App, db *sqlx.DB) {
	app.Get("/labs/labs", labs.GetLabs(db))
	app.Get("/labs/labs/add", labs.GetAddLabForm(db))
	app.Post("/labs/labs/add", labs.AddLab(db))
	app.Get("/labs/labs/delete/:id", labs.DeleteLab(db))

	app.Get("/labs/testers", labs.GetTesters(db))
	app.Get("/labs/testers/add", labs.GetAddTesterForm(db))
	app.Post("/labs/testers/add", labs.AddTester(db))
	app.Get("/labs/testers/delete/:id", labs.DeleteTester(db))

	app.Get("/labs/equipment", labs.GetEquipments(db))
	app.Get("/labs/equipment/add", labs.GetAddEquipmentForm(db))
	app.Post("/labs/equipment/add", labs.AddEquipment(db))
	app.Get("/labs/equipment/delete/:id", labs.DeleteEquipment(db))

	app.Get("/labs/tests", labs.GetPerformedTests(db))
	app.Get("/labs/tests/add", labs.GetAddPerformedTestForm(db))
	app.Post("/labs/tests/add", labs.AddPerformedTest(db))
	app.Get("/labs/tests/delete/:id", labs.DeletePerformedTest(db))

	app.Get("/labs/testsList", labs.GetTestsList(db))
	app.Get("/labs/testsList/add", labs.GetAddTestListForm(db))
	app.Post("/labs/testsList/add", labs.AddTestList(db))
	app.Get("/labs/testsList/delete/:id", labs.DeleteTestList(db))
}

func setupWorkshops(app *fiber.App, db *sqlx.DB) {
	app.Get("/workshops/workshops", workshops.GetWorkshops(db))
	app.Get("/workshops/workshops/add", workshops.GetAddWorkshopForm(db))
	app.Post("/workshops/workshops/add", workshops.AddWorkshop(db))
	app.Get("/workshops/workshops/delete/:id", workshops.DeleteWorkshop(db))

	app.Get("/workshops/areas", workshops.GetAreas(db))
	app.Get("/workshops/areas/add", workshops.GetAddAreaForm(db))
	app.Post("/workshops/areas/add", workshops.AddArea(db))
	app.Get("/workshops/areas/delete/:id", workshops.DeleteArea(db))

	app.Get("/workshops/assemblyPlans", workshops.GetAssemblyPlans(db))
	app.Get("/workshops/assemblyPlans/add", workshops.GetAddAssemblyPlanForm(db))
	app.Post("/workshops/assemblyPlans/add", workshops.AddAssemblyPlan(db))
	app.Get("/workshops/assemblyPlans/delete/:id", workshops.DeleteAssemblyPlan(db))

	app.Get("/workshops/doneWorks", workshops.GetDoneWorks(db))
	app.Get("/workshops/doneWorks/add", workshops.GetAddDoneWorkForm(db))
	app.Post("/workshops/doneWorks/add", workshops.AddDoneWork(db))
	app.Get("/workshops/doneWorks/delete/:id", workshops.DeleteDoneWork(db))
}
