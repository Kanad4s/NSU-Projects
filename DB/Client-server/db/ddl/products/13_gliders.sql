CREATE TABLE IF NOT EXISTS "Планеры" (
	"id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
	"вес" INTEGER  NOT NULL CHECK ("вес" > 0),
	"размах_крыла" INTEGER NOT NULL  CHECK ("размах_крыла" > 0),
	"макс_скорость" INTEGER  NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000),
	"аэро_качество" NUMERIC(5,2) NOT NULL CHECK ("аэро_качество" > 0 )
);
