#!/usr/bin/env bash

set -euo pipefail

PROJECT_NAME="cmdfx"
DEFAULT_REPO_URL="https://github.com/gmitch215/cmdfx.git"
DEFAULT_BRANCH="master"
DEFAULT_BUILD_TYPE="Release"

# Colors (minimal, only if terminal)
if [ -t 1 ]; then
	BOLD="\033[1m"; RESET="\033[0m"; GREEN="\033[32m"; YELLOW="\033[33m"; RED="\033[31m"
else
	BOLD=""; RESET=""; GREEN=""; YELLOW=""; RED=""
fi

usage() {
	cat <<EOF
${BOLD}${PROJECT_NAME} installer${RESET}

Usage: $0 [options]

Options:
	-r, --repo <url>        Git repository URL (default: ${DEFAULT_REPO_URL})
	-b, --branch <name>     Git branch or tag to checkout (default: ${DEFAULT_BRANCH})
	-d, --dir <path>        Working directory for clone/build (default: temporary)
	-p, --prefix <path>     CMake install prefix (default: platform-specific)
	-t, --type <type>       CMake build type (default: ${DEFAULT_BUILD_TYPE})
	-j, --jobs <n>          Parallel build jobs (default: number of CPUs)
	--no-docs               Disable building documentation (DOCS_CMDFX=OFF)
	--no-tests              Disable building tests (TEST_CMDFX=OFF)
	--no-package            Disable packaging (PACKAGE_CMDFX=OFF)
	--no-kn                 Disable Kotlin/Native bindings (KN_CMDFX=OFF)
	--sudo                  Use sudo for install step
	--no-sudo               Do not use sudo even if prefix is system path
	--shallow               Shallow clone (depth=1)
	--keep                  Keep working directory (do not delete)
	-h, --help              Show this help and exit

Examples:
	$0
	$0 --branch v0.3.3 --prefix /usr/local --sudo
	$0 --repo git@github.com:gmitch215/cmdfx.git --dir "$HOME/build-cmdfx" --jobs 8
EOF
}

log() { echo -e "${BOLD}${GREEN}==>${RESET} $*"; }
warn() { echo -e "${BOLD}${YELLOW}==>${RESET} $*"; }
err() { echo -e "${BOLD}${RED}ERROR:${RESET} $*" >&2; }

# Defaults
REPO_URL="${DEFAULT_REPO_URL}"
BRANCH="${DEFAULT_BRANCH}"
WORK_DIR=""
BUILD_TYPE="${DEFAULT_BUILD_TYPE}"
JOBS=""
PREFIX=""
USE_SUDO="auto"   # auto|yes|no
SHALLOW="false"
KEEP_DIR="false"

DOCS_OPT="ON"
TESTS_OPT="ON"
PACKAGE_OPT="ON"
KN_OPT="ON"

# Parse args
while [ $# -gt 0 ]; do
	case "$1" in
		-r|--repo) REPO_URL="$2"; shift 2;;
		-b|--branch) BRANCH="$2"; shift 2;;
		-d|--dir) WORK_DIR="$2"; shift 2;;
		-p|--prefix) PREFIX="$2"; shift 2;;
		-t|--type) BUILD_TYPE="$2"; shift 2;;
		-j|--jobs) JOBS="$2"; shift 2;;
		--no-docs) DOCS_OPT="OFF"; shift;;
		--no-tests) TESTS_OPT="OFF"; shift;;
		--no-package) PACKAGE_OPT="OFF"; shift;;
		--no-kn) KN_OPT="OFF"; shift;;
		--sudo) USE_SUDO="yes"; shift;;
		--no-sudo) USE_SUDO="no"; shift;;
		--shallow) SHALLOW="true"; shift;;
		--keep) KEEP_DIR="true"; shift;;
		-h|--help) usage; exit 0;;
		*) err "Unknown argument: $1"; usage; exit 1;;
	esac
done

# OS detection for defaults
UNAME_S="$(uname -s 2>/dev/null || echo Unknown)"
case "$UNAME_S" in
	Darwin) DEFAULT_PREFIX="/usr/local";;
	Linux)  DEFAULT_PREFIX="/usr";;
	MINGW*|MSYS*|CYGWIN*) DEFAULT_PREFIX="/c/Program Files/${PROJECT_NAME}";;
	*) DEFAULT_PREFIX="/usr/local";;
esac

if [ -z "$PREFIX" ]; then PREFIX="$DEFAULT_PREFIX"; fi

# Jobs default
if [ -z "$JOBS" ]; then
	if command -v sysctl >/dev/null 2>&1; then
		JOBS="$(sysctl -n hw.ncpu 2>/dev/null || echo 1)"
	elif command -v nproc >/dev/null 2>&1; then
		JOBS="$(nproc)"
	else
		JOBS=1
	fi
fi

# Dependency checks
need_cmd() { command -v "$1" >/dev/null 2>&1 || { err "Missing required command: $1"; exit 1; }; }
need_cmd git
need_cmd cmake

# A C compiler is required (and possibly C++)
if ! command -v cc >/dev/null 2>&1 && ! command -v clang >/dev/null 2>&1 && ! command -v gcc >/dev/null 2>&1; then
	err "No C compiler found (gcc/clang). Please install Xcode Command Line Tools (macOS) or build-essential (Linux)."
	exit 1
fi

# Prepare working directory
TMP_ROOT=""
if [ -z "$WORK_DIR" ]; then
	TMP_ROOT="$(mktemp -d -t ${PROJECT_NAME}-install-XXXXXX)"
	WORK_DIR="$TMP_ROOT"
else
	mkdir -p "$WORK_DIR"
fi

SRC_DIR="$WORK_DIR/${PROJECT_NAME}-src"
BUILD_DIR="$WORK_DIR/${PROJECT_NAME}-build"

cleanup() {
	if [ "$KEEP_DIR" = "false" ] && [ -n "$TMP_ROOT" ] && [ -d "$TMP_ROOT" ]; then
		rm -rf "$TMP_ROOT"
	fi
}
trap cleanup EXIT

log "Cloning repository: $REPO_URL (branch: $BRANCH)"
GIT_CLONE_ARGS=("$REPO_URL" "$SRC_DIR")
if [ "$SHALLOW" = "true" ]; then GIT_CLONE_ARGS=("--depth" "1" "--branch" "$BRANCH" "$REPO_URL" "$SRC_DIR"); fi
if [ "$SHALLOW" = "false" ]; then GIT_CLONE_ARGS=("--branch" "$BRANCH" "$REPO_URL" "$SRC_DIR"); fi
git clone "${GIT_CLONE_ARGS[@]}"

log "Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

log "Configuring CMake (${BUILD_TYPE})"
cmake -S "$SRC_DIR" -B "$BUILD_DIR" \
	-DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
	-DCMAKE_INSTALL_PREFIX="$PREFIX" \
	-DTEST_CMDFX="$TESTS_OPT" \
	-DDOCS_CMDFX="$DOCS_OPT" \
	-DPACKAGE_CMDFX="$PACKAGE_OPT" \
	-DKN_CMDFX="$KN_OPT"

# Detect generator to decide if we need --config (multi-config generators)
GENERATOR=""
if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
	GENERATOR="$(awk -F= '/^CMAKE_GENERATOR:INTERNAL=/{print $2}' "$BUILD_DIR/CMakeCache.txt" 2>/dev/null || true)"
fi

is_multi_config=false
case "$GENERATOR" in
	*"Visual Studio"*|*"Xcode"*|*"Ninja Multi-Config"*) is_multi_config=true;;
esac

log "Building (${JOBS} job(s))"
if [ "$is_multi_config" = true ]; then
	cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel "$JOBS"
else
	cmake --build "$BUILD_DIR" --parallel "$JOBS"
fi

# Decide whether to use sudo
should_use_sudo() {
	if [ "$USE_SUDO" = "yes" ]; then return 0; fi
	if [ "$USE_SUDO" = "no" ]; then return 1; fi
	# auto: use sudo when prefix is a system directory and user lacks write perms
	if [ -d "$PREFIX" ] && [ ! -w "$PREFIX" ]; then return 0; fi
	case "$PREFIX" in
		/usr|/usr/*|/opt|/opt/*|/Library|/Library/*|/System|/System/*) return 0;;
	esac
	return 1
}

log "Installing to: $PREFIX"
if should_use_sudo; then
	if command -v sudo >/dev/null 2>&1; then
		if [ "$is_multi_config" = true ]; then
			sudo cmake --install "$BUILD_DIR" --config "$BUILD_TYPE"
		else
			sudo cmake --install "$BUILD_DIR"
		fi
	else
		warn "sudo not available; attempting install without sudo"
		if [ "$is_multi_config" = true ]; then
			cmake --install "$BUILD_DIR" --config "$BUILD_TYPE"
		else
			cmake --install "$BUILD_DIR"
		fi
	fi
else
	if [ "$is_multi_config" = true ]; then
		cmake --install "$BUILD_DIR" --config "$BUILD_TYPE"
	else
		cmake --install "$BUILD_DIR"
	fi
fi

log "Installation complete. Binaries and libraries installed to '$PREFIX'."
echo "To use in CMake, add: find_package(${PROJECT_NAME} REQUIRED) and target_link_libraries(<your> PRIVATE ${PROJECT_NAME})"

