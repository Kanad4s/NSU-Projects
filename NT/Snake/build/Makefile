TARGET = snake-node

SRC_DIR = ../cmd
BUILD_DIR = ./
GO = go
PROTOC = protoc


.PHONY: protobuf
protobuf:
	@echo "Generating code for protobuf..."
	$(PROTOC) -I=../protocol --go_out=../internal ../protocol/p2p.proto



.PHONY: clean
clean:
	@echo "Cleaning the executable file..."
	@rm -f $(TARGET)


.PHONY: build
build: clean
	@echo "Building the executable file..."
	$(GO) mod download
	$(GO) mod tidy
	$(GO) build $(BUILD_FLAGS) -o $(TARGET) $(SRC_DIR)


.PHONY: help
help:
	@echo "Available commands:"
	@echo "    make protobuf"
	@echo "        Generate code for protobuf"
	@echo "    make clean"
	@echo "        Clean generated files"
	@echo "    make build"
	@echo "        Build the executable file"
	@echo "    make run"
	@echo "        Run the built executable file"
	@echo "    make help"
	@echo "        Display this message"

# Default action is to display help
.DEFAULT_GOAL := help