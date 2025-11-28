#!/usr/bin/env bash
set -euo pipefail

shopt -s nullglob

usage() {
	cat <<EOF
Usage: ./format.sh [files...]

Without arguments, formats all tracked C/C++ source files using clang-format.
With arguments, only the specified files are formatted (must exist).
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
	if command -v mapfile >/dev/null 2>&1; then
		mapfile -t files < <(git ls-files '*.c' '*.cpp' '*.h' '*.hpp')
	else
		# Alternative for systems without mapfile (older bash versions)
		files=()
		while IFS= read -r line; do
			files+=("$line")
		done < <(git ls-files '*.c' '*.cpp' '*.h' '*.hpp')
	fi
fi

if [[ ${#files[@]} -eq 0 ]]; then
	echo "No files to format." >&2
	exit 0
fi

fail=0
for file in "${files[@]}"; do
	if [[ ! -f $file ]]; then
		echo "Skip (not a file): $file" >&2
		continue
	fi
	if ! clang-format -i "$file" --style=file; then
		echo "Error formatting $file" >&2
		fail=1
	else
		echo "Formatted $file" >&2
	fi
done

exit $fail
