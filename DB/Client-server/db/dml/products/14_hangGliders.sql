INSERT INTO "Дельтапланы" ("id", "вес", "угол_веера", "макс_скорость") 
VALUES
    (13, 40, 120, 120),
    (14, 60, 130, 130),
    (15, 45, 120, 110)
ON CONFLICT DO NOTHING;