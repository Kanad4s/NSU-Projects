INSERT INTO "Категории_самолетов" ("название") 
VALUES 
    ('Гражданский'),
    ('Военный'),
    ('Транспортный')
ON CONFLICT DO NOTHING;
