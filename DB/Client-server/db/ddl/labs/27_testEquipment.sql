CREATE TABLE IF NOT EXISTS "Испытательное_оборудование" (
	"id" SERIAL PRIMARY KEY,
	"вид" INTEGER NOT NULL REFERENCES "Виды_испытательного_оборудования"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"название" VARCHAR(255) NOT NULL,
	"дата_производства" DATE NOT NULL,
	"лаборатория" INTEGER NOT NULL REFERENCES "Лаборатории"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);