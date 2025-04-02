CREATE TABLE IF NOT EXISTS "Цеха" (
	"id" SERIAL PRIMARY KEY,
	"начальник_цеха" INTEGER NOT NULL REFERENCES "ИТ_персонал"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS "Виды_работ_на_участке" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Участки_цехов" (
	"id" SERIAL PRIMARY KEY,
	"цех" INTEGER  NOT NULL REFERENCES "Цеха"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"начальник" INTEGER NOT NULL REFERENCES "ИТ_персонал"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"вид_работы" INTEGER NOT NULL REFERENCES "Виды_работ_на_участке"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	UNIQUE("цех", "начальник")
);

CREATE TABLE IF NOT EXISTS "Мастера_участка" (
	"участок" INTEGER NOT NULL REFERENCES "Участки_цехов"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"мастер" INTEGER NOT NULL REFERENCES "ИТ_персонал"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	PRIMARY KEY ("участок", "мастер")
);

CREATE TABLE IF NOT EXISTS "План_сборки" (
	"id" SERIAL PRIMARY KEY,
	"модель" INTEGER NOT NULL REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"работа" INTEGER NOT NULL REFERENCES "Виды_работ_на_участке"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	"этап" INTEGER NOT NULL CHECK ("этап" > 0),
	UNIQUE("модель", "работа", "этап")
);

CREATE TABLE IF NOT EXISTS "Выполненные_работы" (
	"id" SERIAL PRIMARY KEY,
	"участок" INTEGER NOT NULL REFERENCES "Участки_цехов"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	"бригада" INTEGER NOT NULL REFERENCES "Бригады"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	"изделие" INTEGER NOT NULL REFERENCES "Выпускаемые_изделия"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	"этап" INTEGER NOT NULL REFERENCES "План_сборки"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,
	"поступление" DATE NOT NULL CHECK ("поступление" <= CURRENT_DATE),
	"покидание" DATE CHECK ("покидание" >= "поступление" OR "покидание" IS NULL),
	UNIQUE("участок", "бригада", "изделие", "этап")
);

