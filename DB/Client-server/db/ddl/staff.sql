CREATE TABLE IF NOT EXISTS "Люди" (
    "id" serial PRIMARY KEY,
    "ФИО" varchar(255) NOT NULL,
    "дата_рождения" date NOT NULL,
    "кол-во_детей" INTEGER NOT NULL CHECK ("кол-во_детей" >= 0),
    "дата_устройства" date NOT NULL,
    "дата_увольнения" date,
    UNIQUE ("ФИО", "дата_рождения"),
    CONSTRAINT "дата_увольнения_после_устройства" 
        CHECK ("дата_увольнения" IS NULL OR "дата_увольнения" > "дата_устройства"),
    CONSTRAINT "дата_рождения_до_устройства" 
        CHECK ("дата_рождения" < "дата_устройства")
);

CREATE TABLE IF NOT EXISTS "Категории_ИТ_персонала" 
(
	"id"       serial PRIMARY KEY,
	"название" varchar(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "ИТ_персонал" (
    "id" SERIAL PRIMARY KEY,
    "человек" INTEGER NOT NULL UNIQUE REFERENCES "Люди"("id") ON DELETE CASCADE,
    "категория" INTEGER NOT NULL REFERENCES "Категории_ИТ_персонала"("id") ON DELETE CASCADE,
    "уровень_допуска" CHAR NOT NULL CHECK ("уровень_допуска" IN ('A', 'B', 'C')),
    "знания_английского" varchar(255) NOT NULL CHECK ("знания_английского" IN ('A1', 'A2', 'B1', 'B2', 'C1', 'C2'))
);

CREATE TABLE IF NOT EXISTS "Категории_рабочих" (
	"id" serial PRIMARY KEY,
	"название" INTEGER NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS "Рабочие" (
    "id" serial PRIMARY KEY,
    "человек" INTEGER NOT NULL UNIQUE REFERENCES "Люди"("id") ON DELETE CASCADE,
    "бригада" INTEGER,
    "категория" INTEGER NOT NULL REFERENCES "Категории_рабочих"("id") ON DELETE CASCADE,
    "физическая_форма" varchar(255) NOT NULL CHECK ("физическая_форма" IN ('отличная', 'хорошая', 'удовлетворительная', 'плохая')),
    "размер_спецодежды" INTEGER NOT NULL CHECK ("размер_спецодежды" BETWEEN 40 AND 60)
);

CREATE TABLE IF NOT EXISTS "Бригады" (
	"id" serial PRIMARY KEY,
	"Бригадир" INTEGER NOT NULL REFERENCES "Рабочие"("id") ON DELETE CASCADE
);
ALTER TABLE "Рабочие" ADD CONSTRAINT "Рабочие_fk2" FOREIGN KEY ("бригада") REFERENCES "Бригады"("id") ON DELETE CASCADE;