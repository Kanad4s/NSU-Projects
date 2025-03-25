CREATE TABLE IF NOT EXISTS "Ракеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "категория" INTEGER NOT NULL REFERENCES "Категории_ракет"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "мощность" NUMERIC(10, 2) NOT NULL CHECK ("мощность" > 0),
    "дальность" INTEGER NOT NULL CHECK ("дальность" > 0),
    "вес" NUMERIC(10, 2) NOT NULL CHECK ("вес" > 0)
);