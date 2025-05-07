#!/bin/bash
set -e

DIR=$(dirname "$0")
OUTPUT=debug/output

function antlr4() {
  java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool "$@"
}

cd ${DIR}/../

# Generate lexer files
antlr4 -Dlanguage=Java -o ${OUTPUT} SecLangLexer.g4

# Compile the generated lexer files
javac -cp "/usr/local/lib/antlr-4.13.2-complete.jar" ${OUTPUT}/*.java

# Run the lexer on the input file
java -cp "/usr/local/lib/antlr-4.13.2-complete.jar:${OUTPUT}" org.antlr.v4.gui.TestRig SecLangLexer tokens -tokens debug/input.txt

# Clean up generated files
rm -rf ${OUTPUT}