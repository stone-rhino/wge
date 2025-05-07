#!/bin/bash
set -e

DIR=$(dirname "$0")
OUTPUT=debug/output

function antlr4() {
  java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool "$@"
}

cd ${DIR}/../

# Generate lexer and parser files
antlr4 -Dlanguage=Java -o ${OUTPUT} SecLangLexer.g4 SecLangParser.g4

# Compile the generated lexer and parser files
javac -cp "/usr/local/lib/antlr-4.13.2-complete.jar" ${OUTPUT}/*.java

# Run the configuration rule on the input file, and print the parse tree
# java -cp "/usr/local/lib/antlr-4.13.2-complete.jar:${OUTPUT}" org.antlr.v4.gui.TestRig SecLang configuration -tokens debug/input.txt
java -cp "/usr/local/lib/antlr-4.13.2-complete.jar:${OUTPUT}" org.antlr.v4.gui.TestRig SecLang configuration -tree debug/input.txt

# Clean up generated files
rm -rf ${OUTPUT}