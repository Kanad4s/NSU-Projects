CREATE TABLE IF NOT EXISTS "Бригады" (
	"id" SERIAL PRIMARY KEY,
	"бригадир" INTEGER NOT NULL UNIQUE REFERENCES "Рабочие"("id") ON DELETE RESTRICT ON UPDATE CASCADE
);
ALTER TABLE "Рабочие" ADD CONSTRAINT "Рабочие_fk2" FOREIGN KEY ("бригада") REFERENCES "Бригады"("id") ON DELETE RESTRICT ON UPDATE CASCADE;
