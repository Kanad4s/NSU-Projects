CREATE TABLE IF NOT EXISTS "Ракеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "категория" INTEGER NOT NULL REFERENCES "Категории_ракет"("id") ON DELETE RESTRICT ON UPDATE CASCADE,
    "калибр" INTEGER NOT NULL CHECK ("калибр" BETWEEN 1 AND 1000000),
    "мощность" NUMERIC(10, 2) NOT NULL CHECK ("мощность" > 0),
    "дальность" INTEGER NOT NULL CHECK ("дальность" > 0),
    "вес" NUMERIC(10, 2) NOT NULL CHECK ("вес" > 0),
    "макс_скорость" INTEGER NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000)
);