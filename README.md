<div style="display: flex; align-items: center; flex-direction: column;" align="center">
    <img align="center" style="align-self: center; max-width: 256px" src="https://cdn.gmitch215.xyz/cmdfx.png" width="30%" alt="" />
    <h1 style="text-align: center;">CmdFX</h1>
    <p style="text-align: center;">A lightweight game engine for your terminal.</p>
    <div align="center">
        <img src="https://img.shields.io/github/v/release/gmitch215/cmdfx">
        <img src="https://img.shields.io/github/downloads/gmitch215/cmdfx/total">
        <img src="https://img.shields.io/github/license/gmitch215/cmdfx">
        <img src="https://img.shields.io/github/stars/gmitch215/cmdfx?style=flat">
        <img src="https://img.shields.io/github/commit-activity/t/gmitch215/cmdfx?color=violet">
    </div>
</div>

---

**cmdfx** is a lightweight game engine for your terminal, allowing you to create some pretty wild things. It is designed to be simple and easy to use, while still providing a powerful set of features.

It is written in C and is available cross-platform. It is licensed under the MIT license.

## 🍎 Features

- **Cross-platform**: cmdfx is available on Windows, macOS, and Linux.
- **Lightweight**: cmdfx is designed to be lightweight and fast.
- **Easy to use**: cmdfx is simple to use and easy to learn.
- **Powerful**: cmdfx provides a powerful set of features for creating terminal-based games.

### 📋 Highlighted Features

For a full method list, check out the [documentation](https://gmitch215.github.io/cmdfx/).

- **Events API**
  - Handle window events such as key presses and window resizing.
  - Get the current state of the keyboard.
- **Canvas API**
  - Draw characters and shapes on the terminal.
  - Set foreground and background colors.
  - Clear the screen.
  - Get the size of the terminal and the screen.
  - ...and much more!
  - **Sprites API**
    - Load and draw sprites on the terminal.
    - Set the color of a sprite.
    - Create Gradients for a sprite.
    - ...and much more!
- **Builder API**
  - Easily build 2D arrays of characters and strings.
  - Set the color of the text.
  - Create gradients of text and colors!
  - ...and much more!
- **Physics Engine**
  - Create and manage physics objects with the Sprite API.
  - Set the velocity and acceleration of a sprite.
  - Detect collisions between sprites.
  - ...and much more!
- **Input API**
  - Get the current state of the keyboard and mouse events.
  - Handle key presses and key releases.
  - ..and much more!
- **Cross-Platform Exposure**
  - Expose platform-specific features and utilities such as setting the title of the terminal.

## 📦 Installation

You can download the latest release of cmdfx from the [releases page](https://github.com/gmitch215/cmdfx/releases).

### Install via Script

Use the cross-platform Bash installer to clone, build, and install cmdfx with CMake.

```sh
curl -fsSL https://raw.githubusercontent.com/gmitch215/cmdfx/master/install.sh | bash
```

- macOS/Linux (bash/zsh):

```sh
curl -fsSL https://raw.githubusercontent.com/gmitch215/cmdfx/master/install.sh -o install-cmdfx.sh
bash install-cmdfx.sh
```

- Windows: run from Git Bash (bundled with Git for Windows) or MSYS2 shell:

```sh
curl -fsSL https://raw.githubusercontent.com/gmitch215/cmdfx/master/install.sh -o install-cmdfx.sh
bash install-cmdfx.sh
```

Defaults after a no-flags run:

- MacOS: installs to `/usr/local`
- Linux: installs to `/usr`
- Windows (Git Bash/MSYS): installs to `C:/Program Files/cmdfx`

Common options:

- `--branch <name>`: install a tag/branch (e.g. `v0.3.3`)
- `--prefix <path>`: change install prefix
- `--type <Release|Debug>`: CMake build type
- `--shallow`: faster shallow clone
- `--no-tests`, `--no-docs`, `--no-package`, `--no-kn`: toggle optional components

Run `./install.sh --help` for full options.

### Use in CMake Projects

cmdfx installs a CMake package with the target `cmdfx::cmdfx`.

```cmake
find_package(cmdfx REQUIRED)
add_executable(app main.c)
target_link_libraries(app PRIVATE cmdfx::cmdfx)
```

If CMake cannot find the package, point it at the install prefix:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="/usr/local"   # macOS
cmake -S . -B build -DCMAKE_PREFIX_PATH="/usr"         # Linux
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Program Files/cmdfx"  # Windows
```

### GitHub Actions

Install cmdfx via the script, then build your project with CMake using the exported package.

```yml
name: Build with cmdfx

on: [push, pull_request]

jobs:
  build:
  runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        include:
          - os: ubuntu-latest
            cmake_prefix: /usr
          - os: macos-latest
            cmake_prefix: /usr/local
          - os: windows-latest
            cmake_prefix: 'C:/Program Files/cmdfx'
    steps:
      - uses: actions/checkout@v6
      - name: Install cmdfx
        shell: bash
        run: |
          curl -fsSL https://raw.githubusercontent.com/gmitch215/cmdfx/master/install.sh | bash
      - name: Configure
        shell: bash
        run: |
          cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="${{ matrix.cmake_prefix }}"
      - name: Build
        shell: bash
        run: cmake --build build
```

### Kotlin/Native

If you are using Kotlin/Native, you can add cmdfx as a dependency in your `build.gradle.kts` file:

```kotlin
plugins {
    kotlin("multiplatform") version "<version>"
}

repositories {
    maven("https://repo.calcugames.xyz/repository/maven-releases/")
}

kotlin {
    // Define your targets here
    macosArm64()
    mingwX64()

    sourceSets {
        // for example, if you are using macOS arm64
        macosArm64Main.dependencies {
            implementation("dev.gmitch215.cmdfx:cmdfx-macosarm64:<version>@klib")
        }

        // or, if you are using windows
        mingwX64Main.dependencies {
            implementation("dev.gmitch215.cmdfx:cmdfx-mingwx64:<version>@klib")
        }
    }
}
```

## 🚀 Examples

```c
#include <cmdfx.h>

int main() {
    // Set character at position (4, 4) to 'X'
    Canvas_setChar(4, 4, 'X');

    // Draw Circle with '#' at position (10, 10) with radius 5
    Canvas_fillCircle(10, 10, 5, '#');

    // Set Foreground to Red, then draw a line from (0, 0) to (10, 0)
    Canvas_setForeground(0xFF0000);
    Canvas_hLine(0, 0, 10);
}

```

```c
#include <cmdfx.h>

int main() {
    // (path, z-index)
    CmdFX_Sprite* mySprite = Sprite_loadFromFile("sprite.txt", 0);
    Sprite_setForegroundAll(mySprite, 0xFF0000); // Set Color to Red

    // Draw Sprite at position (5, 5)
    Sprite_draw(5, 5, mySprite);

    // Move Sprite to position (10, 10)
    Sprite_moveTo(mySprite, 10, 10);

    // (width, height, char, ansi, z-index)
    CmdFX_Sprite* background = Sprite_createFilled(10, 10, '#', 0, 0);

    // Set Gradient to Foreground with Red, Green, and Gold
    Sprite_setForegroundGradientAll(background, GRADIENT_ANGLE_45, 0xFF0000, 0x00FF00, 0xFFD700);
}

```

```c
#include <stdio.h>
#include <cmdfx.h>

// Detect when the terminal window is resized
int onResize(CmdFX_Event* event) {
    // Get payload data
    CmdFX_ResizeEvent* resizeEvent = (CmdFX_ResizeEvent*) event->data;

    // Print the previous and new size of the terminal
    printf("Terminal resized from %dx%d to %dx%d\n", resizeEvent->prevWidth, resizeEvent->prevHeight, resizeEvent->newWidth, resizeEvent->newHeight);

    return 0;
}

int main() {
    int r = 0;

    Canvas_clearScreen();
    addCmdFXEventListener(CMDFX_EVENT_RESIZE, onResize);

    while (1) {
        // Do nothing while we wait for an event
    }
}
```

More examples can be found in the [samples directory](/samples).

## 📝 Contributing

If you would like to contribute to cmdfx, please see the [contributing guidelines](CONTRIBUTING.md). All contributions are welcome!
