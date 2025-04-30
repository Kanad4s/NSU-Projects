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


