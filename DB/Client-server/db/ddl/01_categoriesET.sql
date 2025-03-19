CREATE TABLE IF NOT EXISTS "Категории_ИТ_персонала" 
(
	id         serial NOT NULL UNIQUE,
	"название" varchar(255) NOT NULL UNIQUE,
	PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS categoriesET
 (
    id    INTEGER NOT NULL ON DELETE CASCADE,
    name VARCHAR (100) NOT NULL ON DELETE CASCADE,
    PRIMARY KEY (army_id, brigade_id)
 );
 
 COMMENT ON TABLE categoriesET IS 'Категории ИТ персонала';
 
 COMMENT ON COLUMN categoriesET.id IS 'ID категории';
 COMMENT ON COLUMN categoriesET.name IS 'название категории';