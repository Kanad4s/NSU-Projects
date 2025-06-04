CREATE OR REPLACE FUNCTION кадровый_состав(
    цех_id INTEGER DEFAULT NULL,
    категория_ит TEXT DEFAULT NULL,
    категория_рабочих TEXT DEFAULT NULL
)
RETURNS TABLE (
    id_человека INTEGER,
    фио TEXT,
    категория TEXT,
    тип TEXT,
    цех INTEGER
) AS $$
BEGIN
    RETURN QUERY

    -- ИТ-персонал
    SELECT 
        p."id",
        p."ФИО"::TEXT,
        кат_ит."название"::TEXT,
        'ИТ_персонал'::TEXT,
        ц."id"
    FROM "ИТ_персонал" i
    JOIN "Люди" p ON p."id" = i."id"
    JOIN "Категории_ИТ_персонала" кат_ит ON кат_ит."id" = i."категория"
    LEFT JOIN "Цеха" ц ON i."id" = ц."начальник_цеха"
    WHERE 
        (категория_ит IS NULL OR кат_ит."название" = категория_ит)
        AND (цех_id IS NULL OR ц."id" = цех_id)

    UNION ALL

    -- Рабочие
    SELECT 
        p."id",
        p."ФИО"::TEXT,
        кат_раб."название"::TEXT,
        'Рабочие'::TEXT,
        NULL
    FROM "Рабочие" r
    JOIN "Люди" p ON p."id" = r."id"
    JOIN "Категории_рабочих" кат_раб ON кат_раб."id" = r."категория"
    WHERE 
        (категория_рабочих IS NULL OR кат_раб."название" = категория_рабочих);
END;
$$ LANGUAGE plpgsql;

-- SELECT * FROM кадровый_состав();