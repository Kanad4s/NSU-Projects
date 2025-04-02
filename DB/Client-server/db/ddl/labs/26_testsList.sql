CREATE TABLE IF NOT EXISTS "Набор_испытаний" (
	"id" SERIAL PRIMARY KEY,
	"модель" INTEGER NOT NULL REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"вид_испытания" INTEGER NOT NULL REFERENCES "Виды_испытаний"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"описание" TEXT,
	UNIQUE ("модель", "вид_испытания")
);