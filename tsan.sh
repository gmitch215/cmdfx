#!/usr/bin/env bash

# Builds cmdfx with ThreadSanitizer and runs the automated test suite under it.
# Catches data races in the physics engine start -> tick -> end lifecycle.
# ThreadSanitizer cannot be combined with AddressSanitizer, so it has its own
# build directory.
#
# Usage: ./tsan.sh [extra cmake args...]
#   BUILD_DIR   override the build directory (default: build-tsan)

set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build-tsan}"

cmake -B "$BUILD_DIR" \
	-DCMAKE_BUILD_TYPE=Debug \
	-DSANITIZE_CMDFX=thread \
	-DKN_CMDFX=OFF -DDOCS_CMDFX=OFF -DPACKAGE_CMDFX=OFF \
	"$@"
cmake --build "$BUILD_DIR" -j

TSAN_OPTIONS="${TSAN_OPTIONS:-halt_on_error=1}" \
	ctest --test-dir "$BUILD_DIR" --output-on-failure --timeout 120
