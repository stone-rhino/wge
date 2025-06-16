#!/bin/bash

clang-format -i --style=file $(find ./src -name "*.cc" -o -name "*.h")
clang-format -i --style=file $(find ./test \( -path ./test/test_data -prune \) -o \( -name "*.cc" -o -name "*.h" \) -print)