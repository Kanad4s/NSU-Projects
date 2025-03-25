CREATE TABLE IF NOT EXISTS "Виды_испытательного_оборудования" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);