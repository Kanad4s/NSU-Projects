CREATE TABLE IF NOT EXISTS "Люди" (
    "id" SERIAL PRIMARY KEY,
    "ФИО" VARCHAR(255) NOT NULL,
    "дата_рождения" DATE NOT NULL,
    "кол-во_детей" INTEGER NOT NULL CHECK ("кол-во_детей" >= 0),
    "дата_устройства" DATE NOT NULL,
    "дата_увольнения" DATE,
    UNIQUE ("ФИО", "дата_рождения"),
    CONSTRAINT "дата_увольнения_после_устройства" 
        CHECK ("дата_увольнения" IS NULL OR "дата_увольнения" > "дата_устройства"),
    CONSTRAINT "дата_рождения_до_устройства" 
        CHECK ("дата_рождения" < "дата_устройства")
);