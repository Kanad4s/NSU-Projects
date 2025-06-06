CREATE OR REPLACE FUNCTION состав_бригад(
    цех_id INTEGER DEFAULT NULL,
    участок_id INTEGER DEFAULT NULL
)
RETURNS TABLE (
    бригада_id INTEGER,
    бригадир INTEGER,
    рабочий_id INTEGER
) AS $$
BEGIN
    RETURN QUERY
    SELECT DISTINCT
        б."id" AS бригада_id,
        б."бригадир" AS бригадир,
        р."id" AS рабочий_id
    FROM "Бригады" б
    JOIN "Рабочие" р ON р."бригада" = б."id"
    JOIN "Выполненные_работы" вр ON вр."бригада" = б."id"
    JOIN "Участки_цехов" у ON у."id" = вр."участок"
    WHERE (цех_id IS NULL OR у."цех" = цех_id)
      AND (участок_id IS NULL OR у."id" = участок_id)
    ORDER BY б."id", р."id";
END;
$$ LANGUAGE plpgsql;


-- SELECT * FROM состав_бригад();
