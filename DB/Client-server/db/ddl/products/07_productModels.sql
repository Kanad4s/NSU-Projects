CREATE TABLE IF NOT EXISTS "Модели_изделий" (
	"id" SERIAL PRIMARY KEY,
	"вид" INTEGER NOT NULL REFERENCES "Виды_изделий"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"название" VARCHAR(255) NOT NULL,
	"описание" VARCHAR(255) NOT NULL,
	"дата_разработки" DATE NOT NULL,
	UNIQUE("вид", "название")
);