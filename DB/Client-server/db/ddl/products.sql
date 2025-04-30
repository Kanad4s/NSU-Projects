CREATE TABLE IF NOT EXISTS "Виды_изделий" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Модели_изделий" (
	"id" SERIAL PRIMARY KEY,
	"вид" INTEGER NOT NULL REFERENCES "Виды_изделий"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"название" VARCHAR(255) NOT NULL,
	"описание" VARCHAR(255) NOT NULL,
	"дата_разработки" DATE NOT NULL,
	UNIQUE("вид", "название")
);

CREATE TABLE IF NOT EXISTS "Категории_самолетов" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Самолеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "категория" INTEGER NOT NULL REFERENCES "Категории_самолетов"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
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
    "категория" INTEGER NOT NULL REFERENCES "Категории_ракет"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
    "калибр" INTEGER NOT NULL CHECK ("калибр" BETWEEN 1 AND 1000000),
    "мощность" NUMERIC(10, 2) NOT NULL CHECK ("мощность" > 0),
    "дальность" INTEGER NOT NULL CHECK ("дальность" > 0),
    "вес" NUMERIC(10, 2) NOT NULL CHECK ("вес" > 0),
    "макс_скорость" INTEGER NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000)
);

CREATE TABLE IF NOT EXISTS "Вертолеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
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


CREATE TABLE IF NOT EXISTS "Выпускаемые_изделия" (
    "id" SERIAL PRIMARY KEY,
    "модель" INTEGER NOT NULL REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "начало_производства" DATE NOT NULL CHECK ("начало_производства" <= CURRENT_DATE),
    "дата_выпуска" DATE CHECK ("дата_выпуска" >= "начало_производства" OR "дата_выпуска" IS NULL)
);

CREATE OR REPLACE FUNCTION check_unique_model_type()
RETURNS TRIGGER AS $$
DECLARE
    cnt INTEGER;
BEGIN
    SELECT COUNT(*) INTO cnt FROM (
        SELECT id FROM "Самолеты" WHERE id = NEW.id AND TG_TABLE_NAME <> 'Самолеты'
        UNION ALL
        SELECT id FROM "Вертолеты" WHERE id = NEW.id AND TG_TABLE_NAME <> 'Вертолеты'
        UNION ALL
        SELECT id FROM "Ракеты" WHERE id = NEW.id AND TG_TABLE_NAME <> 'Ракеты'
        UNION ALL
        SELECT id FROM "Планеры" WHERE id = NEW.id AND TG_TABLE_NAME <> 'Планеры'
        UNION ALL
        SELECT id FROM "Дельтапланы" WHERE id = NEW.id AND TG_TABLE_NAME <> 'Дельтапланы'
    ) AS all_refs;

    IF cnt > 0 THEN
        RAISE EXCEPTION 'Модель с id % уже зарегистрирована в другой таблице изделий.', NEW.id;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_planes_unique_model ON "Самолеты";
CREATE TRIGGER trg_planes_unique_model
BEFORE INSERT OR UPDATE ON "Самолеты"
FOR EACH ROW
EXECUTE FUNCTION check_unique_model_type();

DROP TRIGGER IF EXISTS trg_helicopters_unique_model ON "Вертолеты";
CREATE TRIGGER trg_helicopters_unique_model
BEFORE INSERT OR UPDATE ON "Вертолеты"
FOR EACH ROW
EXECUTE FUNCTION check_unique_model_type();

DROP TRIGGER IF EXISTS trg_rockets_unique_model ON "Ракеты";
CREATE TRIGGER trg_rockets_unique_model
BEFORE INSERT OR UPDATE ON "Ракеты"
FOR EACH ROW
EXECUTE FUNCTION check_unique_model_type();

DROP TRIGGER IF EXISTS trg_gliders_unique_model ON "Планеры";
CREATE TRIGGER trg_gliders_unique_model
BEFORE INSERT OR UPDATE ON "Планеры"
FOR EACH ROW
EXECUTE FUNCTION check_unique_model_type();

DROP TRIGGER IF EXISTS trg_hang_gliders_unique_model ON "Дельтапланы";
CREATE TRIGGER trg_hang_gliders_unique_model
BEFORE INSERT OR UPDATE ON "Дельтапланы"
FOR EACH ROW
EXECUTE FUNCTION check_unique_model_type();



