CREATE TABLE IF NOT EXISTS "План_сборки" (
	"id" SERIAL PRIMARY KEY,
	"модель" INTEGER NOT NULL REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"работа" INTEGER NOT NULL REFERENCES "Виды_работ_на_участке"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	"этап" INTEGER NOT NULL CHECK ("этап" > 0),
	UNIQUE("модель", "работа", "этап")
);