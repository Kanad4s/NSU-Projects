CREATE TABLE IF NOT EXISTS "Виды_изделий" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Модели_изделий" (
	"id" SERIAL PRIMARY KEY,
	"вид" INTEGER NOT NULL REFERENCES "Виды_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
	"название" VARCHAR(255) NOT NULL,
	"описание" VARCHAR(255) NOT NULL,
	"дата_разработки" DATE NOT NULL
);

CREATE TABLE IF NOT EXISTS "Категории_самолетов" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Самолеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "категория" INTEGER NOT NULL REFERENCES "Категории_самолетов"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "размах_крыла" NUMERIC(6, 2) NOT NULL CHECK ("размах_крыла" > 0),
    "кол-во_мест" INTEGER NOT NULL CHECK ("кол-во_мест" > 0),
    "макс_скорость" INTEGER NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000)
);

CREATE TABLE IF NOT EXISTS "Категории_ракет" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Ракеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "категория" INTEGER NOT NULL REFERENCES "Категории_ракет"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "мощность" NUMERIC(10, 2) NOT NULL CHECK ("мощность" > 0),
    "дальность" INTEGER NOT NULL CHECK ("дальность" > 0),
    "вес" NUMERIC(10, 2) NOT NULL CHECK ("вес" > 0)
);

CREATE TABLE IF NOT EXISTS "Вертолеты" (
    "id"INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "грузоподъемность" NUMERIC(10, 2) NOT NULL CHECK ("грузоподъемность" > 0),
    "высота_подъема" INTEGER NOT NULL CHECK ("высота_подъема" BETWEEN 0 AND 100000),
    "макс_скорость" INTEGER NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000)
);

CREATE TABLE IF NOT EXISTS "Планеры" (
	"id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
	"вес" INTEGER  NOT NULL CHECK ("вес" > 0),
	"размах_крыла" INTEGER NOT NULL  CHECK ("размах_крыла" > 0),
	"макс_скорость" INTEGER  NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000),
	"аэро_качество" NUMERIC(5,2) NOT NULL CHECK ("аэро_качество" > 0 )
);

CREATE TABLE IF NOT EXISTS "Дельтапланы" (
	"id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
	"вес" INTEGER  NOT NULL CHECK ("вес" > 0),
	"макс_скорость" INTEGER  NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000),
	"угол_веера" NUMERIC(5,2)  NOT NULL CHECK ("угол_веера" BETWEEN 0 AND 180)
);





