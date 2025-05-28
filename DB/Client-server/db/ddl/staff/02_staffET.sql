CREATE TABLE IF NOT EXISTS "ИТ_персонал" (
    "id" SERIAL PRIMARY KEY,
    "человек" INTEGER NOT NULL UNIQUE REFERENCES "Люди"("id") ON DELETE CASCADE ON UPDATE CASCADE,
    "категория" INTEGER NOT NULL REFERENCES "Категории_ИТ_персонала"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
    "уровень_допуска" CHAR NOT NULL CHECK ("уровень_допуска" IN ('A', 'B', 'C')),
    "знания_английского" varchar(255) NOT NULL CHECK ("знания_английского" IN ('A1', 'A2', 'B1', 'B2', 'C1', 'C2'))
);