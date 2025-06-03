CREATE OR REPLACE FUNCTION перечень_изделий(
    p_цех_id INTEGER DEFAULT NULL,
    p_категория_изделия TEXT DEFAULT NULL
)
RETURNS TABLE (
    id_модели INTEGER,
    название_модели TEXT,
    категория TEXT,
    цех_id INTEGER
) AS $$
BEGIN
    RETURN QUERY
    WITH Категории_изделий AS (
        SELECT m."id", m."название"::text AS "название", 'Самолеты'::text AS "категория"
        FROM "Модели_изделий" m JOIN "Самолеты" s ON m."id" = s."id"
        UNION ALL
        SELECT m."id", m."название"::text, 'Ракеты'::text
        FROM "Модели_изделий" m JOIN "Ракеты" r ON m."id" = r."id"
        UNION ALL
        SELECT m."id", m."название"::text, 'Вертолеты'::text
        FROM "Модели_изделий" m JOIN "Вертолеты" v ON m."id" = v."id"
        UNION ALL
        SELECT m."id", m."название"::text, 'Планеры'::text
        FROM "Модели_изделий" m JOIN "Планеры" p ON m."id" = p."id"
        UNION ALL
        SELECT m."id", m."название"::text, 'Дельтапланы'::text
        FROM "Модели_изделий" m JOIN "Дельтапланы" d ON m."id" = d."id"
    )
    SELECT DISTINCT
        k."id" AS id_модели,
        k."название" AS название_модели,
        k."категория",
        c."id" AS цех_id
    FROM "Выполненные_работы" wr
    JOIN "Участки_цехов" u ON wr."участок" = u."id"
    JOIN "Цеха" c ON u."цех" = c."id"
    JOIN "Выпускаемые_изделия" vi ON wr."изделие" = vi."id"
    JOIN Категории_изделий k ON vi."модель" = k."id"
    WHERE
        (p_цех_id IS NULL OR c."id" = p_цех_id)
        AND (p_категория_изделия IS NULL OR k."категория" = p_категория_изделия)
    ORDER BY c.id;
END;
$$ LANGUAGE plpgsql;
