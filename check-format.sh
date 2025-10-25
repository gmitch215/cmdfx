#!/usr/bin/env bash
set -euo pipefail

shopt -s nullglob

usage() {
	cat <<EOF
Usage: ./check-format.sh [files...]

Without arguments, checks all tracked C/C++ source files for clang-format compliance.
With arguments, only the specified files are checked (must exist).
Exit code 0 if all files are properly formatted, non-zero otherwise.
EOF
}

if [[ ${1:-} == "-h" || ${1:-} == "--help" ]]; then
	usage
	exit 0
fi

if ! command -v clang-format >/dev/null 2>&1; then
	echo "Error: clang-format not found in PATH" >&2
	exit 1
fi

if [[ $# -gt 0 ]]; then
	files=("$@")
else
	# Use git to list tracked files matching extensions
	mapfile -t files < <(git ls-files '*.c' '*.cpp' '*.h' '*.hpp')
fi

if [[ ${#files[@]} -eq 0 ]]; then
	echo "No files to check." >&2
	exit 0
fi

fail=0
for file in "${files[@]}"; do
	if [[ ! -f $file ]]; then
		echo "Skip (not a file): $file" >&2
		continue
	fi

	# Check if file is properly formatted by comparing original with formatted version
	if ! diff -q "$file" <(clang-format "$file" --style=file) >/dev/null 2>&1; then
		echo "Format check failed: $file" >&2
		fail=1
	else
		echo "Format check passed: $file" >&2
	fi
done

if [[ $fail -eq 0 ]]; then
	echo "All files are properly formatted." >&2
else
	echo "Some files are not properly formatted. Run ./format.sh to fix them." >&2
fi

exit $fail
