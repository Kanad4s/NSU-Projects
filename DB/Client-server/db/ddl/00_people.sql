CREATE TABLE IF NOT EXISTS "Люди" (
    "id" serial NOT NULL UNIQUE,
    "ФИО" varchar(255) NOT NULL,
    "дата_рождения" date NOT NULL,
    "кол-во_детей" bigint NOT NULL CHECK ("кол-во_детей" >= 0),
    "дата_устройства" date NOT NULL,
    "дата_увольнения" date,
    PRIMARY KEY ("id"),
    CONSTRAINT "уникальное_ФИО_и_дата_рождения" UNIQUE ("ФИО", "дата_рождения"),
    CONSTRAINT "дата_увольнения_после_устройства" 
        CHECK ("дата_увольнения" IS NULL OR "дата_увольнения" > "дата_устройства"),
    CONSTRAINT "дата_рождения_до_устройства" 
        CHECK ("дата_рождения" < "дата_устройства")
);