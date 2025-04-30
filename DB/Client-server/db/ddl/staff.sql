CREATE TABLE IF NOT EXISTS "Люди" (
    "id" SERIAL PRIMARY KEY,
    "ФИО" VARCHAR(255) NOT NULL,
    "дата_рождения" DATE NOT NULL,
    "кол-во_детей" INTEGER NOT NULL CHECK ("кол-во_детей" >= 0),
    "дата_устройства" DATE NOT NULL,
    "дата_увольнения" DATE,
    UNIQUE ("ФИО", "дата_рождения"),
    CONSTRAINT "дата_увольнения_после_устройства" 
        CHECK ("дата_увольнения" IS NULL OR "дата_увольнения" > "дата_устройства"),
    CONSTRAINT "дата_рождения_до_устройства" 
        CHECK ("дата_рождения" < "дата_устройства")
);

CREATE TABLE IF NOT EXISTS "Категории_ИТ_персонала" 
(
	"id"       serial PRIMARY KEY,
	"название" varchar(255) NOT NULL UNIQUE
);
 
 COMMENT ON TABLE "Категории_ИТ_персонала"  IS 'Категории ИТ персонала';
 
 COMMENT ON COLUMN "Категории_ИТ_персонала".id IS 'ID категории';
 COMMENT ON COLUMN "Категории_ИТ_персонала"."название" IS 'название категории';

CREATE TABLE IF NOT EXISTS "ИТ_персонал" (
    "id" SERIAL PRIMARY KEY,
    "человек" INTEGER NOT NULL UNIQUE REFERENCES "Люди"("id") ON DELETE CASCADE ON UPDATE CASCADE,
    "категория" INTEGER NOT NULL REFERENCES "Категории_ИТ_персонала"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
    "уровень_допуска" CHAR NOT NULL CHECK ("уровень_допуска" IN ('A', 'B', 'C')),
    "знания_английского" varchar(255) NOT NULL CHECK ("знания_английского" IN ('A1', 'A2', 'B1', 'B2', 'C1', 'C2'))
);

CREATE TABLE IF NOT EXISTS "Категории_рабочих" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Рабочие" (
    "id" serial PRIMARY KEY,
    "человек" INTEGER NOT NULL UNIQUE REFERENCES "Люди"("id") ON DELETE CASCADE ON UPDATE CASCADE,
    "бригада" INTEGER,
    "категория" INTEGER NOT NULL REFERENCES "Категории_рабочих"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
    "физическая_форма" varchar(255) NOT NULL CHECK ("физическая_форма" IN ('отличная', 'хорошая', 'удовлетворительная', 'плохая')),
    "размер_спецодежды" INTEGER NOT NULL CHECK ("размер_спецодежды" BETWEEN 40 AND 60)
);

CREATE OR REPLACE FUNCTION check_person_unique()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_TABLE_NAME = 'Рабочие' THEN
        IF EXISTS (SELECT 1 FROM "ИТ_персонал" WHERE "человек" = NEW."человек") THEN
            RAISE EXCEPTION 'Person with id % already exists in it table', NEW."человек";
        END IF;
    END IF;
    
    IF TG_TABLE_NAME = 'ИТ_персонал' THEN
        IF EXISTS (SELECT 1 FROM "Рабочие" WHERE "человек" = NEW."человек") THEN
            RAISE EXCEPTION 'Person with id % already exists in workers table', NEW."человек";
        END IF;
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER check_worker_unique
BEFORE INSERT OR UPDATE ON "Рабочие"
FOR EACH ROW EXECUTE FUNCTION check_person_unique();

CREATE OR REPLACE TRIGGER check_it_unique
BEFORE INSERT OR UPDATE ON "ИТ_персонал"
FOR EACH ROW EXECUTE FUNCTION check_person_unique();

CREATE TABLE IF NOT EXISTS "Бригады" (
	"id" SERIAL PRIMARY KEY,
	"бригадир" INTEGER NOT NULL UNIQUE REFERENCES "Рабочие"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);
ALTER TABLE "Рабочие" ADD CONSTRAINT "Рабочие_fk2" FOREIGN KEY ("бригада") REFERENCES "Бригады"("id") ON DELETE RESTRICT ON UPDATE CASCADE;

CREATE OR REPLACE FUNCTION check_brigadir_consistency()
RETURNS trigger AS $$
DECLARE
    brigade_id INTEGER;
BEGIN
    SELECT id INTO brigade_id
    FROM "Бригады"
    WHERE "бригадир" = NEW.id;

    IF brigade_id IS NOT NULL THEN
        IF NEW.бригада IS DISTINCT FROM brigade_id THEN
            RAISE EXCEPTION 'Рабочий (id=%) является бригадиром бригады (id=%), но указан в другой бригаде (id=%).',
                NEW.id, brigade_id, NEW.бригада;
        END IF;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_check_brigadir_consistency
BEFORE INSERT OR UPDATE ON "Рабочие"
FOR EACH ROW
EXECUTE FUNCTION check_brigadir_consistency();




