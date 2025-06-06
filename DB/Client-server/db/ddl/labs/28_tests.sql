CREATE TABLE IF NOT EXISTS "Испытания" (
	"id" SERIAL PRIMARY KEY,
	"испытание" INTEGER NOT NULL REFERENCES "Набор_испытаний"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"изделие" INTEGER NOT NULL REFERENCES "Выпускаемые_изделия"("id") ON DELETE CASCADE ON UPDATE CASCADE,
	"дата" DATE NOT NULL CHECK ("дата" <= CURRENT_DATE),
	"испытатель" INTEGER NOT NULL REFERENCES "Испытатели"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
	"оборудование" INTEGER NOT NULL REFERENCES "Испытательное_оборудование"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);


CREATE OR REPLACE FUNCTION проверить_дату_работы_испытателя()
RETURNS TRIGGER AS $$
DECLARE
    дата DATE := NEW."дата";
    дата_устройства DATE;
    дата_увольнения DATE;
BEGIN
    SELECT л."дата_устройства", л."дата_увольнения"
    INTO дата_устройства, дата_увольнения
    FROM "Люди" л
    JOIN "ИТ_персонал" ит ON ит."id" = (
        SELECT "id"
        FROM "Испытатели"
        WHERE "id" = NEW."испытатель"
    )
    WHERE л."id" = ит."человек";

    IF дата < дата_устройства THEN
        RAISE EXCEPTION 'Дата испытания (%), раньше даты устройства испытателя (%)', дата, дата_устройства;
    END IF;

    IF дата_увольнения IS NOT NULL AND дата >= дата_увольнения THEN
        RAISE EXCEPTION 'Дата испытания (%), позже даты увольнения испытателя (%)', дата, дата_увольнения;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER триггер_проверка_даты_испытателя
BEFORE INSERT OR UPDATE ON "Испытания"
FOR EACH ROW
EXECUTE FUNCTION проверить_дату_работы_испытателя();

CREATE OR REPLACE FUNCTION проверить_вид_испытания_и_оборудования()
RETURNS TRIGGER AS $$
DECLARE
    вид_из_набора INTEGER;
    вид_из_оборудования INTEGER;
BEGIN
    SELECT "вид_испытания" INTO вид_из_набора
    FROM "Набор_испытаний"
    WHERE id = NEW."испытание";

    SELECT "вид" INTO вид_из_оборудования
    FROM "Испытательное_оборудование"
    WHERE id = NEW."оборудование";

    IF вид_из_набора IS DISTINCT FROM вид_из_оборудования THEN
        RAISE EXCEPTION 'Несовпадение вида испытания (% из набора) и вида оборудования (%) в id=%', вид_из_набора, вид_из_оборудования, NEW.id;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER триггер_проверка_вида_испытания
BEFORE INSERT OR UPDATE ON "Испытания"
FOR EACH ROW
EXECUTE FUNCTION проверить_вид_испытания_и_оборудования();
