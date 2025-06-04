CREATE OR REPLACE FUNCTION состав_бригад2(
    цех_id INTEGER DEFAULT NULL,
    участок_id INTEGER DEFAULT NULL
)
RETURNS TABLE (
    участок INTEGER,
    мастер INTEGER
) AS $$
BEGIN
    RETURN QUERY
    SELECT DISTINCT
        у."id" AS участок,
        б."id" AS мастер
    FROM "ИТ_персонал" б
    JOIN "Мастера_участка" р ON р."мастер" = б."id"
    JOIN "Участки_цехов" у ON у."id" = р."участок"
    WHERE (цех_id IS NULL OR у."цех" = цех_id)
      AND (участок_id IS NULL OR у."id" = участок_id)
    ORDER BY б."id", у."id";
END;
$$ LANGUAGE plpgsql;


-- SELECT * FROM состав_бригад2();
