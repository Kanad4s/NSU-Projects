CREATE TABLE IF NOT EXISTS "Виды_испытательного_оборудования" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Лаборатории" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);


CREATE TABLE IF NOT EXISTS "Лаборатория_цех" (
	"лаборатория" INTEGER REFERENCES "Лаборатории"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"цех" INTEGER REFERENCES "Цеха"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	PRIMARY KEY ("лаборатория", "цех")
);

CREATE TABLE IF NOT EXISTS "Испытатели" (
	"id" INTEGER UNIQUE REFERENCES "ИТ_персонал"("id")  ON DELETE RESTRICT ON UPDATE CASCADE,
	"лаборатория" INTEGER REFERENCES "Лаборатории"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS "Набор_испытаний" (
	"id" SERIAL PRIMARY KEY,
	"модель" INTEGER NOT NULL REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"вид_испытания" INTEGER NOT NULL REFERENCES "Виды_испытательного_оборудования"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"описание" TEXT,
	UNIQUE ("модель", "вид_испытания")
);

CREATE TABLE IF NOT EXISTS "Испытательное_оборудование" (
	"id" SERIAL PRIMARY KEY,
	"вид" INTEGER NOT NULL REFERENCES "Виды_испытательного_оборудования"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"название" VARCHAR(255) NOT NULL,
	"дата_производства" DATE NOT NULL,
	"лаборатория" INTEGER NOT NULL REFERENCES "Лаборатории"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS "Испытания" (
	"id" SERIAL PRIMARY KEY,
	"испытание" INTEGER NOT NULL REFERENCES "Набор_испытаний"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"изделие" INTEGER NOT NULL REFERENCES "Выпускаемые_изделия"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"дата" DATE NOT NULL CHECK ("дата" <= CURRENT_DATE),
	"испытатель" INTEGER NOT NULL REFERENCES "Испытатели"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"оборудование" INTEGER NOT NULL REFERENCES "Испытательное_оборудование"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);

