#!/usr/bin/env bash

# Verifies the cmdfx curses backend on Linux end to end: installs the curses +
# ALSA dependencies, builds, runs the test suite (serial and parallel), and runs
# the AddressSanitizer/UBSan/ThreadSanitizer scripts (ASan leak detection works
# on Linux). Intended for a fresh ("startup") Linux machine.
#
# All build directories it creates are torn down automatically on exit.
#
# Usage: ./linux-verify.sh
#   On a non-Linux host it re-runs itself inside an ubuntu:24.04 Docker
#   container (requires Docker); the container is removed when done.

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$REPO_ROOT"

BUILD_DIRS=(build-linux build-asan-linux build-ubsan-linux build-tsan-linux)

teardown() {
	echo "== tearing down build directories =="
	rm -rf "${BUILD_DIRS[@]}"
}
trap teardown EXIT

# On a non-Linux host, run the whole thing inside a throwaway Linux container.
if [ "$(uname -s)" != "Linux" ]; then
	echo "== host is not Linux; running inside ubuntu:24.04 via Docker =="
	if ! command -v docker >/dev/null 2>&1; then
		echo "Docker is required to run this on a non-Linux host." >&2
		exit 1
	fi
	docker run --rm -v "$REPO_ROOT":/src -w /src ubuntu:24.04 \
		bash linux-verify.sh
	exit $?
fi

SUDO=""
if [ "$(id -u)" -ne 0 ] && command -v sudo >/dev/null 2>&1; then SUDO="sudo"; fi

echo "== installing dependencies (best-effort) =="
if command -v apt-get >/dev/null 2>&1; then
	export DEBIAN_FRONTEND=noninteractive
	$SUDO apt-get update -qq || true
	$SUDO apt-get install -y -qq \
		build-essential git pkg-config \
		libncursesw5-dev libncurses-dev libasound2-dev || true
fi

# cmdfx requires CMake >= 3.30; install a current one via pip if the system
# package is older (older Ubuntu releases ship 3.28).
ensure_cmake() {
	local ok=0
	if command -v cmake >/dev/null 2>&1; then
		local ver major minor
		ver="$(cmake --version | head -1 | grep -oE '[0-9]+\.[0-9]+' | head -1)"
		major="${ver%%.*}"
		minor="${ver##*.}"
		if [ "$major" -gt 3 ] || { [ "$major" -eq 3 ] && [ "$minor" -ge 30 ]; }; then
			ok=1
		fi
	fi
	if [ "$ok" -eq 0 ]; then
		echo "== installing a current cmake via pip =="
		$SUDO apt-get install -y -qq python3-pip >/dev/null 2>&1 || true
		pip3 install --break-system-packages -q cmake 2>/dev/null ||
			pip3 install -q cmake || true
		hash -r
	fi
}
ensure_cmake

echo "== configure + build (Linux ncursesw) =="
cmake -B build-linux -DCMAKE_BUILD_TYPE=Debug -DSAMPLES_CMDFX=ON \
	-DKN_CMDFX=OFF -DDOCS_CMDFX=OFF -DPACKAGE_CMDFX=OFF
cmake --build build-linux -j

echo "== test (serial) =="
ctest --test-dir build-linux --output-on-failure --timeout 90
echo "== test (parallel, -j4) =="
ctest --test-dir build-linux --timeout 90 -j4

echo "== AddressSanitizer + UBSan (with leak detection) =="
BUILD_DIR=build-asan-linux bash asan.sh
echo "== UndefinedBehaviorSanitizer =="
BUILD_DIR=build-ubsan-linux bash ubsan.sh
echo "== ThreadSanitizer =="
BUILD_DIR=build-tsan-linux bash tsan.sh

echo "== all Linux verification passed =="
