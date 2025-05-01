CREATE OR REPLACE FUNCTION check_model_type_match() RETURNS trigger AS $$
DECLARE
    plane_type_id INT;
    helicopter_type_id INT;
    rocket_type_id INT;
    gliders_type_id INT;
    hang_gliders_type_id INT;
BEGIN
    SELECT id INTO plane_type_id FROM "Виды_изделий" WHERE "название" = 'Самолеты';
    SELECT id INTO helicopter_type_id FROM "Виды_изделий" WHERE "название" = 'Вертолеты';
    SELECT id INTO rocket_type_id FROM "Виды_изделий" WHERE "название" = 'Ракеты';
    SELECT id INTO gliders_type_id FROM "Виды_изделий" WHERE "название" = 'Планеры';
    SELECT id INTO hang_gliders_type_id FROM "Виды_изделий" WHERE "название" = 'Дельтапланы';

    IF NEW."вид" = plane_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Самолеты" WHERE "id" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Самолеты', NEW."id";
        END IF;
    ELSIF NEW."вид" = helicopter_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Вертолеты" WHERE "id" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Вертолеты', NEW."id";
        END IF;
    ELSIF NEW."вид" = rocket_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Ракеты" WHERE "id" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Ракеты', NEW."id";
        END IF;
    ELSIF NEW."вид" = gliders_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Планеры" WHERE "id" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Планеры', NEW."id";
        END IF;
    ELSIF NEW."вид" = hang_gliders_type_id THEN
        IF NOT EXISTS (
            SELECT 1 FROM "Дельтапланы" WHERE "id" = NEW."id"
        ) THEN
            RAISE EXCEPTION 'Модель с id % должна быть связана с таблицей Дельтапланы', NEW."id";
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
