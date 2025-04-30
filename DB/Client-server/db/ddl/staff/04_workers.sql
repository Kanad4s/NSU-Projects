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