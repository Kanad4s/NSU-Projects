CREATE TABLE IF NOT EXISTS "Лаборатория_цех" (
	"лаборатория" INTEGER REFERENCES "Лаборатории"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"цех" INTEGER REFERENCES "Цеха"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	PRIMARY KEY ("лаборатория", "цех")
);