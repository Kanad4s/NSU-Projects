CREATE OR REPLACE FUNCTION перечень_участков(
    цех_id INTEGER DEFAULT NULL
)
RETURNS TABLE (
    id_участка INTEGER,
    начальник_id INTEGER,
    начальник_ФИО TEXT,
    цех INTEGER,
    количество_участков BIGINT
) AS $$
BEGIN
    RETURN QUERY
    SELECT
        у."id",
        у."начальник",
        п."ФИО"::TEXT,
        у."цех",
        COUNT(*) OVER (PARTITION BY у."цех") AS количество_участков
    FROM "Участки_цехов" у
    JOIN "ИТ_персонал" ит ON ит."id" = у."начальник"
    JOIN "Люди" п ON п."id" = ит."id"
    WHERE (цех_id IS NULL OR у."цех" = цех_id);
END;
$$ LANGUAGE plpgsql;





-- SELECT * FROM перечень_участков(2);