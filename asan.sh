#!/usr/bin/env bash

# Builds cmdfx with AddressSanitizer (+ UndefinedBehaviorSanitizer) and runs
# the automated test suite under it. Catches heap/stack overflows, use-after-
# free, leaks (Linux), and undefined behavior.
#
# Usage: ./asan.sh [extra cmake args...]
#   BUILD_DIR   override the build directory (default: build-asan)

set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build-asan}"

cmake -B "$BUILD_DIR" \
	-DCMAKE_BUILD_TYPE=Debug \
	-DSANITIZE_CMDFX=address,undefined \
	-DKN_CMDFX=OFF -DDOCS_CMDFX=OFF -DPACKAGE_CMDFX=OFF \
	"$@"
cmake --build "$BUILD_DIR" -j

ASAN_OPTIONS="${ASAN_OPTIONS:-abort_on_error=1:detect_leaks=1}" \
	UBSAN_OPTIONS="${UBSAN_OPTIONS:-halt_on_error=1:print_stacktrace=1}" \
	ctest --test-dir "$BUILD_DIR" --output-on-failure --timeout 120
