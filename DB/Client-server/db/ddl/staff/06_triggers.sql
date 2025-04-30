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
