CREATE OR REPLACE FUNCTION текущие_изделия(
    участок_id INTEGER DEFAULT NULL,
    цех_id INTEGER DEFAULT NULL,
    категория_изделия TEXT DEFAULT NULL
)
RETURNS TABLE (
    id_модели INTEGER,
    название_модели TEXT,
    категория TEXT,
    изделие_id INTEGER,
    участокid INTEGER,
    цехid INTEGER
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
    SELECT DISTINCT
        k."id" AS id_модели,
        k."название",
        k."категория",
        wr."изделие" AS изделие_id,
        u."id" AS участокid,
        c."id" AS цехid
    FROM "Выполненные_работы" wr
    JOIN "Участки_цехов" u ON wr."участок" = u."id"
    JOIN "Цеха" c ON u."цех" = c."id"
    JOIN "Выпускаемые_изделия" vi ON wr."изделие" = vi."id"
    JOIN Категории_изделий k ON vi."модель" = k."id"
    WHERE wr."покидание" IS NULL
      AND (участок_id IS NULL OR u."id" = участок_id)
      AND (цех_id IS NULL OR c."id" = цех_id)
      AND (категория_изделия IS NULL OR k."категория" = категория_изделия);
END;
$$ LANGUAGE plpgsql;


-- SELECT * FROM текущие_изделия();