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
