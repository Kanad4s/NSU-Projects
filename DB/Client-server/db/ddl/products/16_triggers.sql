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
