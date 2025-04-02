INSERT INTO "Бригады" ("бригадир")
VALUES
    (1),
    (5),
    (6),
    (10),
    (15),
    (18),
    (21)
ON CONFLICT DO NOTHING;

UPDATE "Рабочие"
SET "бригада" = 1
WHERE "id" IN (22, 11, 8, 24, 1);

UPDATE "Рабочие"
SET "бригада" = 2
WHERE "id" IN (5, 14, 2);

UPDATE "Рабочие"
SET "бригада" = 3
WHERE "id" IN (23, 6, 3);

UPDATE "Рабочие"
SET "бригада" = 4
WHERE "id" IN (17, 19, 10, 25);

UPDATE "Рабочие"
SET "бригада" = 5
WHERE "id" IN (15, 20, 9, 4);

UPDATE "Рабочие"
SET "бригада" = 6
WHERE "id" IN (7, 13, 18);

UPDATE "Рабочие"
SET "бригада" = 7
WHERE "id" IN (21, 12, 16);