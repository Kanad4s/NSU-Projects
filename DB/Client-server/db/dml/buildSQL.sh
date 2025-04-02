#!/bin/bash

PYTHON_CMD="python3"

SCRIPT_PATH="$(pwd)/../../build/buildSQL.py"

ROOT_DIR="$(pwd)"

$PYTHON_CMD "$SCRIPT_PATH" $ROOT_DIR
