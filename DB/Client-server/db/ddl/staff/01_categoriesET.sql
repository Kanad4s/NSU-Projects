CREATE TABLE IF NOT EXISTS "Категории_ИТ_персонала" 
(
	"id"       serial PRIMARY KEY,
	"название" varchar(255) NOT NULL UNIQUE
);
 
 COMMENT ON TABLE "Категории_ИТ_персонала"  IS 'Категории ИТ персонала';
 
 COMMENT ON COLUMN "Категории_ИТ_персонала".id IS 'ID категории';
 COMMENT ON COLUMN "Категории_ИТ_персонала"."название" IS 'название категории';