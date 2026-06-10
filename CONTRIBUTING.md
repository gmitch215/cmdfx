# Contributing

Contributing guidelines for the CmdFX project. We appreciate your help in helping us to improve the project.

## Pull Requeset Process

1.) Ensure that the pull request is at the correct branch you want to merge in.

2.) Ensure that your new code has been tested and works properly. If necessary, add test according to the `test` directory.

3.) Confirm that the code is ready to be merged while meeting our conventions.

We will approve and merge your pull request as soon as possible if these requirements are met.

## Building & Conventions

cmdfx builds with CMake (>= 3.30) and renders through (n)curses (PDCurses on
Windows). Install the curses development package as described in the README
prerequisites.

- Format every C/C++ change with `./format.sh`, then confirm `./check-format.sh`
  exits 0. CI gates on this before building.
- Run the automated suite from the build directory with `ctest`.
- Check memory safety and data races with the sanitizer helpers: `./asan.sh`
  (AddressSanitizer + UBSan), `./ubsan.sh`, and `./tsan.sh` (ThreadSanitizer).
