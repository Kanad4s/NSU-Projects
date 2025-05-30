CREATE TABLE IF NOT EXISTS "Участки_цехов" (
	"id" SERIAL PRIMARY KEY,
	"цех" INTEGER  NOT NULL REFERENCES "Цеха"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"начальник" INTEGER NOT NULL REFERENCES "ИТ_персонал"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"вид_работы" INTEGER NOT NULL REFERENCES "Виды_работ_на_участке"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	UNIQUE("цех", "начальник")
);