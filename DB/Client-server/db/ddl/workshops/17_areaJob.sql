CREATE TABLE IF NOT EXISTS "Виды_работ_на_участке" (
	"id" SERIAL PRIMARY KEY,
	"название" VARCHAR(255) NOT NULL UNIQUE
);