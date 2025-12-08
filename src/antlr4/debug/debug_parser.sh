#!/bin/bash
set -e

DIR=$(dirname "$0")
PROJECT_ROOT=${DIR}/../../..

# Change to project root directory
cd ${PROJECT_ROOT}
BUILD_DIR=$(pwd)/build/debug
INPUT_FILE=$(pwd)/src/antlr4/debug/input.txt

# Build the antlr4_debug_lexer target using cmake
echo "Building antlr4_debug_lexer target..."
cmake --build "${BUILD_DIR}" --target antlr4_debug_parser

# Run the debug lexer with the input file
echo "Running antlr4_debug_parser with input file: ${INPUT_FILE}"
${BUILD_DIR}/src/antlr4/debug/antlr4_debug_parser ${INPUT_FILE}