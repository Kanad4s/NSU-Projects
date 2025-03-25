CREATE TABLE IF NOT EXISTS "Самолеты" (
    "id" INTEGER NOT NULL UNIQUE REFERENCES "Модели_изделий"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "категория" INTEGER NOT NULL REFERENCES "Категории_самолетов"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "размах_крыла" NUMERIC(6, 2) NOT NULL CHECK ("размах_крыла" > 0),
    "кол-во_мест" INTEGER NOT NULL CHECK ("кол-во_мест" > 0),
    "макс_скорость" INTEGER NOT NULL CHECK ("макс_скорость" BETWEEN 0 AND 300000000)
);