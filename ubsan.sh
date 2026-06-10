#!/usr/bin/env bash

# Builds cmdfx with UndefinedBehaviorSanitizer and runs the automated test
# suite under it. Catches signed overflow, bad shifts, misaligned access,
# invalid enum/bool values, and incompatible function-pointer calls.
#
# Usage: ./ubsan.sh [extra cmake args...]
#   BUILD_DIR   override the build directory (default: build-ubsan)

set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build-ubsan}"

cmake -B "$BUILD_DIR" \
	-DCMAKE_BUILD_TYPE=Debug \
	-DSANITIZE_CMDFX=undefined \
	-DKN_CMDFX=OFF -DDOCS_CMDFX=OFF -DPACKAGE_CMDFX=OFF \
	"$@"
cmake --build "$BUILD_DIR" -j

UBSAN_OPTIONS="${UBSAN_OPTIONS:-halt_on_error=1:print_stacktrace=1}" \
	ctest --test-dir "$BUILD_DIR" --output-on-failure --timeout 120
