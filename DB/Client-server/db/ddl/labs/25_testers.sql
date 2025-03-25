CREATE TABLE IF NOT EXISTS "Испытатели" (
	"id" INTEGER REFERENCES "ИТ_персонал"("id")  ON DELETE RESTRICT ON UPDATE CASCADE,
	"лаборатория" INTEGER REFERENCES "Лаборатории"("id") ON DELETE CASCADE ON UPDATE CASCADE,
    PRIMARY KEY ("сотрудник", "лаборатория")
);