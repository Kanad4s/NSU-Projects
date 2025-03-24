CREATE TABLE IF NOT EXISTS "Рабочие" (
    "id" serial PRIMARY KEY,
    "человек" INTEGER NOT NULL UNIQUE REFERENCES "Люди"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "бригада" INTEGER,
    "категория" INTEGER NOT NULL REFERENCES "Категории_рабочих"("id") ON DELETE CASCADE ON UPDATE RESTRICT,
    "физическая_форма" varchar(255) NOT NULL CHECK ("физическая_форма" IN ('отличная', 'хорошая', 'удовлетворительная', 'плохая')),
    "размер_спецодежды" INTEGER NOT NULL CHECK ("размер_спецодежды" BETWEEN 40 AND 60)
);