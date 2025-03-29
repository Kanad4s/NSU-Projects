TARGET = client

SRC_DIR = ../cmd
BUILD_DIR = .
GO = go
PYTHON = python3
SQL_BUILD_SCRIPT = ./buildSQL.py 
DB_QUERIES_DIR = ../db

.PHONY: build, run, data, ddl, clean, help

build: ddl, data
	$(GO) mod tidy
	$(GO) build $(BUILD_FLAGS) -o $(TARGET) $(SRC_DIR)
ddl:
	$(PYTHON) $(SQL_BUILD_SCRIPT) $(DB_QUERIES_DIR)/ddl
data:
	$(PYTHON) $(SQL_BUILD_SCRIPT) $(DB_QUERIES_DIR)/data

run: ddl, data
	$(GO) run $(SRC_DIR)

clean:
	rm -rf $(TARGET)

help:
	@echo "Available commands:"
	@echo "\t build:"
	@echo "\t\t form sql data and ddl scripts and build client"
	@echo "\t run:"
	@echo "\t\t form sql data and ddl scripts and run client"
	@echo "\t data:"
	@echo "\t\t form sql scripts to fill db tables"
	@echo "\t ddl:"
	@echo "\t\t form sql scripts to create db tables"
	@echo "\t clean:"
	@echo "\t\t"
	@echo "\t help:"
	@echo "\t\t show this message"

.DEFAULT_GOAL = run