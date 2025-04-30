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


CREATE OR REPLACE FUNCTION check_model_type_match() RETURNS trigger AS $$
DECLARE
    plane_type_id INT;
    helicopter_type_id INT;
    rocket_type_id INT;
    gliders_type_id INT;
    hang_gliders_type_id INT;
BEGIN
    -- Получаем id для каждого типа из "Виды_изделий"
    SELECT id INTO plane_type_id FROM "Виды_изделий" WHERE "название" = 'Самолеты';
    SELECT id INTO helicopter_type_id FROM "Виды_изделий" WHERE "название" = 'Вертолеты';
    SELECT id INTO rocket_type_id FROM "Виды_изделий" WHERE "название" = 'Ракеты';
    SELECT id INTO gliders_type_id FROM "Виды_изделий" WHERE "название" = 'Планеры';
    SELECT id INTO hang_gliders_type_id FROM "Виды_изделий" WHERE "название" = 'Дельтапланы';

    IF NEW."вид" = plane_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Planes" WHERE "model_type" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Planes', NEW."id";
        END IF;
    ELSIF NEW."вид" = helicopter_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Helicopters" WHERE "model_type" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Helicopters', NEW."id";
        END IF;
    ELSIF NEW."вид" = rocket_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Rockets" WHERE "model_type" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Rockets', NEW."id";
        END IF;
    ELSE
        RAISE EXCEPTION 'Неизвестное значение поля "вид": %', NEW."вид";
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_hang_gliders_unique_model ON "Модели_изделий";
CREATE TRIGGER trg_check_model_type_match
BEFORE INSERT OR UPDATE ON "Модели_изделий"
FOR EACH ROW
EXECUTE FUNCTION check_model_type_match();
