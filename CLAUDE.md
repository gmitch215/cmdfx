# CLAUDE.md

The following is a reference for developers new to the cmdfx codebase, or as a refresher for veterans. It covers the project structure, coding conventions, and working style. It is not an introduction to C or game development — it assumes you are already comfortable with both.

## What cmdfx is

A lightweight, cross-platform (Windows / macOS / Linux) 2D terminal game engine
written in C17, built and installed with CMake (`>= 3.30`). It ships as a static
library exposing the CMake target `cmdfx::cmdfx`, and is meant to be a **drop-in
install** via `install.sh`. MIT licensed. Repo: `gmitch215/cmdfx`.

Public API surface (do not break — see Guardrails):

- **Canvas** (`Canvas_*`) — draw chars/shapes, colors, screen size, clear
- **Sprites** (`Sprite_*`) — load/draw/move, color, gradients, z-index
- **Builder** (`Char2DBuilder_*` / string builders) — 2D char/string arrays
- **Events** (`addCmdFXEventListener`, `CmdFX_Event`, resize/key/mouse)
- **Physics** (`Engine_*`, `Sprite_*` velocity/accel/force/mass/collision)
- **Input** (keyboard + mouse state)
- **Screen / Window** (`Window_*`) — title, size
- **Sound** (`Sound_*`) and **UI** (button/switch/path/scenes)

## Coding conventions (non-negotiable)

### Comments
- inline implementation comments are **sparse, minimal, one-liners**
- **no punctuation**, **all lowercase**, no trailing periods
- one short clause per comment, only where intent is not obvious from the code
- stay consistent with the existing internal comment voice — do not invent a new style
- example:
  ```c
  // skip static sprites
  if (Sprite_isStatic(sprite)) continue;

  // persist velocity across the tick
  double dvx = Sprite_getVelocityX(sprite);
  ```
- **Doxygen `/** ... */` header documentation is the exception** — it is the public
  API reference, keep its existing full-sentence style. Do not lowercase or strip
  doc comments. The lowercase one-liner rule is for implementation comments only.

### Pointers
- asterisk binds **left**, to the type: `CmdFX_Sprite* sprite`, `int* counter`,
  `void** mutexes`. Never `Struct *s` or `Struct * s`.
- this is already encoded in `.clang-format` (`PointerAlignment: Left`).

### Formatting
- `.clang-format`: BasedOnStyle Microsoft, IndentWidth 4, ColumnLimit 80,
  UseTab Never, BraceWrapping after-function false (K&R-ish braces).
- after **every** change to a `.c/.h/.cpp/.hpp` file, run `./format.sh`, then
  confirm `./check-format.sh` exits 0. CI gates on this before it builds anything.
- do not hand-format; let clang-format do it, then verify.

### General C style (match what is already there)
- early-return guard clauses, not nested `if` pyramids
- integer error returns: `0`/positive on success, `-1` on failure (existing convention)
- prefer `calloc` for zeroed allocations; always check the result and clean up on the
  failure path
- keep platform-specific code in its platform directory, shared logic in `src/common`

## Architecture map (where things live)

```
include/cmdfx/        public headers — the API contract. treat as frozen.
  core/   canvas.h screen.h events.h scenes.h sprites.h device.h util.h builder.h costumes.h
  physics/  engine.h force.h mass.h motion.h util.h
  sound/  sound.h
  ui/     button.h switch.h path.h scenes.h keys.h

src/common/           cross-platform logic (most of the engine)
  core/     canvas.c events.c scenes.c sprites.c builder.c costumes.c util.c animation/*
  physics/  engine.c force.c mass.c motion.c util.c
  sound/    sound.c
  ui/       button.c switch.c path.c scenes.c

src/posix/            POSIX terminal backend (Linux + macOS share this)
  core/     canvas.c screen.c events.c scenes.c util.c   <- termios + ANSI escapes
  physics/  engine.c force.c                              <- pthreads physics loop

src/windows/          Windows backend (Console API + ANSI emulation)
  core/     canvas.c screen.c events.c scenes.c util.c device.c
  physics/  engine.c force.c                              <- win32 threads physics loop

src/linux/  core/ device.c screen.c     <- /dev/input device reads (needs elevated perms)
src/macos/  core/ device.c screen.c , sound/ sound.c   <- CoreGraphics/Carbon/IOKit input

test/                 custom test harness (test/src/test.h) + CTest
  src/core/ src/physics/ src/sound/   auto-discovered, run in CI
  src/manual/                          tty-interactive, not run in CI
samples/              collision.c launcher.c square-adventure.c
cmake/Config.cmake.in install.sh format.sh check-format.sh doxygen.sh renovate.json
.github/workflows/    build.yml codeql.yml release.yml
```

**Backend selection** (top-level `CMakeLists.txt`): on `WIN32` it globs
`src/windows/*.c`; otherwise `src/posix/*.c` plus `src/macos/*.c` (APPLE) or
`src/linux/*.c` (LINUX). Sources are `GLOB_RECURSE`'d, so new files in a platform
dir are picked up automatically — but you must re-run CMake configure.

---

## Build / test / format commands

```bash
# configure + build (debug, with samples)
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DSAMPLES_CMDFX=ON
cmake --build build -j

# run the automated suite
cd build && ctest -C Debug --output-on-failure -j0 && cd ..

# run a single test by name
cd build && ctest -R physics/engine --output-on-failure && cd ..

# run the suite under sanitizers (opt-in via -DSANITIZE_CMDFX, or use the helpers)
./asan.sh    # AddressSanitizer + UndefinedBehaviorSanitizer
./ubsan.sh   # UndefinedBehaviorSanitizer only
./tsan.sh    # ThreadSanitizer (physics start/tick/end race + lifecycle)

# format + verify (run after any source edit)
./format.sh && ./check-format.sh

# drop-in installer (the cross-platform path users take)
bash install.sh --no-kn --shallow --type Debug

# read prior CI failures (use this to diagnose the flaky physics test)
gh run list --workflow build.yml --limit 20
gh run view <run-id> --log-failed
```

Linux needs `libasound2-dev` (ALSA) for sound. The ncurses migration adds an
ncurses/PDCurses dependency — see the migration prompt.

## Hard guardrails

1. **Do not change public headers** under `include/cmdfx/` in a way that breaks the
   existing API or ABI. Existing game code and the samples must compile unchanged.
   If a signature genuinely must change, stop and flag it for the developer first.
2. **Preserve the drop-in install.** `install.sh` and `find_package(cmdfx)` →
   `cmdfx::cmdfx` must keep working on all three platforms after every change.
3. **Cross-platform parity.** ncurses is POSIX-only; do not strand Windows. Any
   terminal backend change must have a working Windows path (PDCurses) — see prompt.
4. **Verify, don't trust.** Bug reports in the prompt (and from Copilot) are leads,
   not gospel. Read the actual code, confirm the bug, then fix it. Do the same with
   CI logs via `gh`.
5. **Format gate.** Never leave the tree in a state where `./check-format.sh` fails.
6. **Determinism in physics.** The physics integration must produce identical
   positions run-to-run for identical inputs. If a "fix" reintroduces nondeterminism,
   it is wrong.
7. **No silenced warnings as a fix.** Removing `-w`/warnings to make the audit quiet
   is not a fix. Resolve the underlying issue.

## Working style for this project

- Work in **small, reviewable commits**, one concern each, with the format gate green.
- Before editing a platform backend, read the matching `src/common` file first — most
  behavior is shared and the platform file is thin.
- When unsure whether something is load-bearing, grep the headers and samples for the
  symbol before removing it.
- Prefer fixing the root cause over adding a workaround layer.
