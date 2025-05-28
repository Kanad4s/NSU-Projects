TARGET = client

SRC_DIR = ../cmd
BUILD_DIR = .
GO = go
PYTHON = python3
SQL_BUILD_SCRIPT = ./buildSQL.py 
DB_QUERIES_DIR = ../db
PORT ?= 3000

.PHONY: build, run, dml, ddl, clean, help, killport

build: ddl dml
	$(GO) mod tidy
	$(GO) build $(BUILD_FLAGS) -o $(TARGET) $(SRC_DIR)
ddl:
	$(PYTHON) $(SQL_BUILD_SCRIPT) $(DB_QUERIES_DIR)/ddl
dml:
	$(PYTHON) $(SQL_BUILD_SCRIPT) $(DB_QUERIES_DIR)/dml

run: ddl dml killport
	$(GO) run $(SRC_DIR)

killport:
	@echo "Checking port $(PORT)..."
	@PID=$$(lsof -ti tcp:$(PORT)); \
	if [ -n "$$PID" ]; then \
		echo "Port $(PORT) is in use by PID $$PID. Killing..."; \
		kill -9 $$PID; \
	else \
		echo "Port $(PORT) is free."; \
	fi


clean:
	rm -rf $(TARGET)

help:
	@echo "Available commands:"
	@echo "\t build:"
	@echo "\t\t form sql dml and ddl scripts and build client"
	@echo "\t run:"
	@echo "\t\t form sql dml and ddl scripts and run client"
	@echo "\t dml:"
	@echo "\t\t form sql scripts to fill db tables"
	@echo "\t ddl:"
	@echo "\t\t form sql scripts to create db tables"
	@echo "\t clean:"
	@echo "\t\t"
	@echo "\t help:"
	@echo "\t\t show this message"

.DEFAULT_GOAL = run