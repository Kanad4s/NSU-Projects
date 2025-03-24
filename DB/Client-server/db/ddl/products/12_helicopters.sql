CREATE TABLE IF NOT EXISTS "Вертолеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "грузоподъемность" NUMERIC(10, 2) NOT NULL CHECK ("грузоподъемность" > 0),
    "высота_подъема" INTEGER NOT NULL CHECK ("высота_подъема" BETWEEN 0 AND 100000),
    "макс_скорость" INTEGER NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000)
);