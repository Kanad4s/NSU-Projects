CREATE TABLE IF NOT EXISTS "Дельтапланы" (
	"id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
	"вес" INTEGER  NOT NULL CHECK ("вес" > 0),
	"макс_скорость" INTEGER  NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000),
	"угол_веера" NUMERIC(5,2)  NOT NULL CHECK ("угол_веера" BETWEEN 0 AND 180)
);
