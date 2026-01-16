#!/bin/bash

git config core.hooksPath .githooks

chmod +x .githooks/commit-msg
chmod +x .githooks/pre-commit

echo "Git hooks installed successfully!"
echo "Located in: .githooks/"