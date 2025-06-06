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


-- select * from перечень_изделий(NULL, 'Самолеты');

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



