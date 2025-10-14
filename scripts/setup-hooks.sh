#!/usr/bin/env bash
set -euo pipefail

HOOKS_DIR="$(git rev-parse --show-toplevel)/.githooks"

echo "Configuring git hooks path to $HOOKS_DIR"

git config core.hooksPath "$HOOKS_DIR"

echo "Done. Test with: git commit --allow-empty -m 'test'"
