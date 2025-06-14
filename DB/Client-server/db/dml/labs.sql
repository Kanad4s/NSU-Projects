INSERT INTO "Виды_испытаний"("название") 
VALUES 
    ('Летное'),
    ('Термовакуумная камера'),
    ('Акустическая камера'),
    ('Коррозийный тест'),
    ('Испытание электронных систем'),
    ('Статическое испытание')
ON CONFLICT DO NOTHING;

INSERT INTO "Лаборатории"("название") 
VALUES 
    ('Летный полигон'),
    ('ХимИспЛаб'),
    ('Солнышко'),
    ('Электрон'),
    ('Лаборатория №1')
ON CONFLICT DO NOTHING;

INSERT INTO "Лаборатория_цех"("цех", "лаборатория") 
VALUES 
    (1, 1),
    (1, 2),
    (1, 3),
    (1, 4),
    (1, 5),
    (2, 3),
    (2, 4),
    (2, 5),
    (3, 2),
    (3, 3),
    (3, 5),
    (4, 1),
    (4, 2),
    (4, 5)
ON CONFLICT DO NOTHING;

INSERT INTO "Испытатели" ("id", "лаборатория") 
VALUES 
    (78, 1),
    (77, 2),
    (76, 3),
    (75, 4),
    (74, 3),
    (73, 2),
    (72, 1),
    (71, 5),
    (70, 4),
    (69, 4),
    (68, 5),
    (67, 1),
    (66, 2),
    (65, 3),
    (64, 5)
ON CONFLICT DO NOTHING;

INSERT INTO "Набор_испытаний"("модель", "вид_испытания", "описание") 
VALUES 
    (1, 2, 'Термовакуумная камера'),
    (1, 3, 'Акустическая камера'),
    (1, 5, 'Испытание электронных систем'),
    (1, 6, 'Статическое испытание'),
    (2, 1, 'Летное'),
    (2, 5, 'Испытание электронных систем'),
    (2, 6, 'Статическое испытание'),    
    (3, 1, 'Летное'),
    (3, 2, 'Термовакуумная камера'),
    (3, 3, 'Акустическая камера'),
    (3, 5, 'Испытание электронных систем'),
    (3, 6, 'Статическое испытание'),    
    (4, 1, 'Летное'),
    (4, 4, 'Коррозийный тест'),
    (4, 5, 'Испытание электронных систем'),
    (4, 6, 'Статическое испытание'),    
    (5, 3, 'Акустическая камера'),
    (5, 5, 'Испытание электронных систем'),
    (5, 6, 'Статическое испытание'),    
    (6, 3, 'Акустическая камера'),
    (6, 4, 'Коррозийный тест'),
    (6, 5, 'Испытание электронных систем'),    
    (7, 1, 'Летное'),
    (7, 6, 'Статическое испытание'),
    (8, 1, 'Летное'),
    (8, 4, 'Коррозийный тест'),
    (8, 5, 'Испытание электронных систем'),    
    (9, 1, 'Летное'),
    (9, 2, 'Термовакуумная камера'),
    (9, 5, 'Испытание электронных систем'),
    (9, 6, 'Статическое испытание'),
    
    (10, 1, 'Летное'),
    (10, 4, 'Коррозийный тест'),
    
    (11, 1, 'Летное'),
    (11, 3, 'Акустическая камера'),
    
    (12, 1, 'Летное'),
    (12, 3, 'Акустическая камера'),
    
    (13, 6, 'Статическое испытание'),
    
    (14, 1, 'Летное'),
    (14, 6, 'Статическое испытание'),
    
    (15, 1, 'Летное'),
    (15, 4, 'Коррозийный тест'),
    
    (16, 2, 'Термовакуумная камера'),
    (16, 6, 'Статическое испытание'),
    
    (17, 2, 'Термовакуумная камера'),
    (17, 6, 'Статическое испытание'),
    
    (18, 1, 'Летное'),
    (18, 6, 'Статическое испытание'),
    
    (19, 2, 'Термовакуумная камера'),
    (19, 5, 'Испытание электронных систем'),
    
    (20, 2, 'Термовакуумная камера'),
    (20, 6, 'Статическое испытание'),
    
    (21, 3, 'Акустическая камера'),
    (21, 6, 'Статическое испытание'),
    
    (22, 5, 'Испытание электронных систем'),
    
    (23, 6, 'Статическое испытание'),
    
    (24, 2, 'Термовакуумная камера')
ON CONFLICT DO NOTHING;

INSERT INTO "Испытательное_оборудование"("вид", "название", "дата_производства", "лаборатория") 
VALUES 
    (1, 'Взлетная полоса 5км', '2010-01-01', 1),
    (1, 'Вертолетная площадка', '2010-02-03', 1),
    (1, 'Высокий склон', '2010-02-12', 1),
    (1, 'Яма', '2010-06-12', 1),
    (2, 'Камера высокой температуры до +70', '2010-03-02', 3),
    (2, 'Камера низкой температуры до -30', '2009-02-03', 3),
    (2, 'Камера высокой температуры до +160', '2010-03-02', 3),
    (2, 'Камера низкой температуры до -50', '2009-02-03', 3),
    (3, 'Камеры высоких частот', '2010-08-06', 4),
    (3, 'Камеры низких частот', '2009-05-08', 4),
    (4, 'Климатическая камера', '2010-09-09', 2),
    (4, 'Электрохимическая камера', '2010-03-09', 2),
    (4, 'Климатическая камера', '2010-09-09', 2),
    (4, 'Электрохимическая камера', '2010-03-09', 2),
    (5, 'Программный комплекс Зима-5.1', '2010-05-01', 4),
    (5, 'Программный комплекс Пакман-3', '2009-11-11', 4),
    (6, 'Подвешивание к потолку', '2010-01-01', 5),
    (6, 'Вибронагрузка', '2009-01-01', 5)
ON CONFLICT DO NOTHING;

INSERT INTO "Испытания"("испытание", "изделие", "дата", "испытатель", "оборудование") 
VALUES 
-- 1 - 78, 72
-- 2 - 74, 76
-- 3 - 75, 70
-- 4 - 66(до 2019), 73(с 2019)
-- 5 - 75, 70
-- 6 - 71(16 - 22), 68, 64
-- 1
    (1, 1, '2011-02-01', 74, 5),
    (1, 2, '2011-02-02', 76, 5),
    (2, 1, '2011-02-08', 75, 9),
    (2, 2, '2011-02-09', 75, 9),
    (3, 1, '2011-02-10', 70, 15),
    (3, 2, '2011-02-11', 70, 15),
    (4, 1, '2011-02-12', 64, 17),
    (4, 2, '2011-02-13', 64, 17),
-- 2
    (5, 3, '2015-07-01', 78, 1),
    (5, 4, '2015-07-01', 72, 1),
    (5, 5, '2015-07-02', 78, 1),
    (6, 3, '2015-07-05', 75, 15),
    (6, 4, '2015-07-06', 70, 15),
    (6, 5, '2015-07-07', 75, 15),
    (7, 3, '2015-07-12', 64, 17),
    (7, 4, '2015-07-13', 68, 17),
    (7, 5, '2015-07-13', 68, 17),
-- 3
    (8, 6, '2016-09-01', 78, 1),
    (9, 6, '2016-09-01', 74, 8),
    (10, 6, '2016-09-02', 70, 10),
    (11, 6, '2016-09-05', 75, 16),
    (12, 6, '2016-09-06', 68, 17),

-- 4
    (13, 7, '2017-03-01', 78, 1),
    (13, 8, '2017-03-01', 72, 1),
    (13, 9, '2023-08-02', 78, 1),
    (14, 7, '2017-03-02', 66, 11),
    (14, 8, '2017-03-02', 66, 11),
    (14, 9, '2023-08-03', 73, 11),
    (15, 7, '2017-03-03', 75, 15),
    (15, 8, '2017-03-03', 70, 15),
    (15, 9, '2023-08-04', 70, 15),
    (16, 7, '2017-03-04', 64, 17),
    (16, 8, '2017-03-04', 68, 17),
    -- (6, 9, '2023-08-13', 7, 17)
-- сотрудник должен работать, когда его ставят в испытание

-- 5
    (17, 10, '2024-05-01', 72, 9), -- 1 испытание
    (17, 11, '2019-07-02', 75, 10),
    (17, 12, '2017-09-03', 70, 10),
    (18, 11, '2019-07-03', 75, 16),
    (18, 12, '2017-09-04', 70, 16),
    (19, 11, '2019-07-04', 64, 17),
    (19, 12, '2017-09-13', 71, 17),

-- 6
    -- (3, 13, '2024-05-01', 1, 1), -- недостроен
    (20, 14, '2016-01-10', 70, 10),
    (20, 15, '2014-09-02', 70, 10),
    (21, 14, '2016-01-12', 66, 13),
    (21, 15, '2014-09-05', 66, 13),
    (22, 14, '2016-01-18', 70, 16),
    (22, 15, '2014-09-08', 70, 16),
-- 7
    (23, 16, '2011-05-08', 78, 2),
    (23, 17, '2014-05-15', 72, 2),
    (24, 16, '2011-05-10', 68, 18),
    (24, 17, '2014-05-17', 64, 18),
    
-- 8
    (25, 18, '2015-05-08', 78, 2),
    (25, 19, '2016-08-08', 72, 2),
    (25, 20, '2015-05-10', 78, 2),
    (26, 18, '2015-05-10', 66, 14),
    (26, 19, '2016-08-10', 66, 14),
    (26, 20, '2015-05-15', 66, 14),
    (27, 18, '2015-05-11', 70, 15),
    (27, 19, '2016-08-11', 75, 15),
    (27, 20, '2015-05-20', 70, 15),

-- 9
    (28, 21, '2016-11-18', 78, 2),
    (28, 22, '2017-02-18', 72, 2),
    (29, 21, '2016-11-20', 74, 5),
    (29, 22, '2017-02-20', 74, 5),
    (30, 21, '2016-11-21', 75, 15),
    (30, 22, '2017-02-21', 75, 15),
    (31, 21, '2016-11-23', 64, 18),
    (31, 22, '2017-02-22', 71, 18),

-- 10
    (32, 23, '2017-08-20', 78, 3),
    (32, 24, '2023-02-18', 72, 3),
    (33, 23, '2017-08-21', 66, 13),
    (33, 24, '2023-02-20', 73, 13),

-- 11
    (34, 25, '2023-08-20', 78, 3),
    (34, 26, '2019-07-25', 72, 3),
    (35, 25, '2023-08-21', 75, 10),
    (35, 26, '2019-07-29', 75, 10),

-- 12
    (36, 27, '2017-08-20', 78, 3),
    (37, 27, '2017-07-25', 75, 9),

-- 13
    (38, 28, '2023-08-20', 68, 17),
    (38, 29, '2019-07-25', 71, 17),
    (38, 30, '2019-07-25', 71, 17),

-- 14
    (39, 31, '2012-04-20', 78, 3),
    (39, 32, '2014-04-25', 72, 3),
    (40, 31, '2012-04-21', 64, 17),
    (40, 32, '2014-04-29', 64, 17),

-- 15
    (41, 33, '2023-08-20', 78, 3),
    (42, 33, '2019-07-25', 66, 13),

-- 16
    (43, 34, '2016-05-01', 74, 5),
    (43, 35, '2015-03-01', 74, 5),
    (44, 34, '2016-05-02', 68, 17),
    (44, 35, '2015-03-02', 64, 17),

-- 17
    (45, 36, '2016-11-10', 74, 7),
    (45, 37, '2017-02-02', 74, 7),
    (46, 36, '2016-11-14', 64, 18),
    (46, 37, '2017-02-03', 68, 18),

-- 18
    (47, 38, '2017-08-20', 72, 4),
    (48, 39, '2024-07-25', 64, 18),
    (47, 38, '2017-08-21', 72, 4),

-- 19
    (49, 41, '2019-07-25', 74, 7),
    (50, 41, '2019-07-29', 75, 16),

-- 20
    (51, 42, '2017-09-10', 74, 7),
    (51, 43, '2023-12-10', 74, 7),
    (52, 42, '2017-09-11', 64, 18),
    (52, 43, '2023-12-11', 68, 18),

-- 21
    (53, 44, '2016-12-20', 75, 9),
    (53, 45, '2014-09-05', 70, 9),
    (54, 44, '2016-12-28', 71, 18),
    (54, 45, '2014-09-10', 64, 18),

-- 22
    (55, 46, '2012-06-01', 75, 15),
    (55, 47, '2014-05-20', 70, 15),
    
-- 23
    (56, 48, '2015-08-02', 64, 18),
    (56, 49, '2016-05-07', 71, 18),
    (56, 50, '2015-03-08', 64, 18),

-- 24
    (57, 51, '2016-11-15', 74, 7),
    (57, 52, '2017-02-02', 76, 7)
ON CONFLICT DO NOTHING;

