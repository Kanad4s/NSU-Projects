TARGET = socks5

SRC_DIR = ../cmd
BUILD_DIR = .
GO = go

.PHONY: clean, build, help

clean:
	@rm -rf $(TARGET)

build: clean
	$(GO) mod tidy
	$(GO) build $(BUILD_FLAGS) -o $(TARGET) $(SRC_DIR)

help:
	@echo "Available commands:"
	@echo "    clean"
	@echo "        Clean generated files"
	@echo "    build"
	@echo "        Build executable file"
	@echo "    help"
	@echo "        Display this message"

.DEFAULT_GOAL := help