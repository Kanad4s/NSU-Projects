CREATE TABLE IF NOT EXISTS "Выпускаемые_изделия" (
    "id" SERIAL PRIMARY KEY,
    "модель" INTEGER NOT NULL REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "начало_производства" DATE NOT NULL CHECK ("начало_производства" <= CURRENT_DATE),
    "дата_выпуска" DATE CHECK ("дата_выпуска" >= "начало_производства" OR "дата_выпуска" IS NULL)
);