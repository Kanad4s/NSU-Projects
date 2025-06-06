CREATE OR REPLACE FUNCTION перечень_работ_изделия(
    изделие_id INTEGER
)
RETURNS TABLE (
    этап INTEGER,
    вид_работы TEXT
) AS $$
BEGIN
    RETURN QUERY
    SELECT
        п."этап",
        в."название"::TEXT
    FROM "Выпускаемые_изделия" ви
    JOIN "План_сборки" п ON ви."модель" = п."модель"
    JOIN "Виды_работ_на_участке" в ON в."id" = п."работа"
    WHERE ви."id" = изделие_id
    ORDER BY п."этап";
END;
$$ LANGUAGE plpgsql;

-- SELECT * FROM перечень_работ_изделия(42);