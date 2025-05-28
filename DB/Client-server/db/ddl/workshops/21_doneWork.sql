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