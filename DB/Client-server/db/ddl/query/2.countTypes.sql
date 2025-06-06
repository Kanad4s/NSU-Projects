CREATE OR REPLACE FUNCTION получить_изделия2(
    цех_id INTEGER,
    участок_id INTEGER,
    категория_изделия TEXT,
    дата_с DATE,
    дата_по DATE
)
RETURNS TABLE (
    id_модели INTEGER,
    название_модели TEXT,
    категория TEXT,
    количество BIGINT
) AS $$
BEGIN
    RETURN QUERY
    WITH Категории_изделий AS (
        SELECT m."id", m."название"::TEXT, 'Самолеты'::TEXT AS "категория" FROM "Модели_изделий" m JOIN "Самолеты" s ON m."id" = s."id"
        UNION ALL
        SELECT m."id", m."название"::TEXT, 'Ракеты'::TEXT FROM "Модели_изделий" m JOIN "Ракеты" r ON m."id" = r."id"
        UNION ALL
        SELECT m."id", m."название"::TEXT, 'Вертолеты'::TEXT FROM "Модели_изделий" m JOIN "Вертолеты" v ON m."id" = v."id"
        UNION ALL
        SELECT m."id", m."название"::TEXT, 'Планеры'::TEXT FROM "Модели_изделий" m JOIN "Планеры" p ON m."id" = p."id"
        UNION ALL
        SELECT m."id", m."название"::TEXT, 'Дельтапланы'::TEXT FROM "Модели_изделий" m JOIN "Дельтапланы" d ON m."id" = d."id"
    )
    SELECT
        k."id" AS id_модели,
        k."название"::TEXT AS название_модели,
        k."категория"::TEXT AS категория,
        COUNT(*) AS количество
    FROM "Выполненные_работы" wr
    JOIN "Участки_цехов" u ON wr."участок" = u."id"
    JOIN "Цеха" c ON u."цех" = c."id"
    JOIN "Выпускаемые_изделия" vi ON wr."изделие" = vi."id"
    JOIN Категории_изделий k ON vi."модель" = k."id"
    WHERE
        (цех_id IS NULL OR c."id" = цех_id) AND
        (участок_id IS NULL OR u."id" = участок_id) AND
        (категория_изделия IS NULL OR k."категория" = категория_изделия) AND
        wr."поступление" BETWEEN дата_с AND дата_по
    GROUP BY k."id", k."название", k."категория";
END;
$$ LANGUAGE plpgsql;

-- SELECT * FROM получить_изделия2(NULL, NULL, NULL, '2012-01-01', '2024-12-31');