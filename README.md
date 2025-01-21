<div style="display: flex; align-items: center; flex-direction: column;">
    <img style="align-self: center;" src="https://cdn.gmitch215.xyz/cmdfx.png" width="30%" style="max-width: 256px" alt="" />
    <h1 style="text-align: center;">CmdFX</h1>
    <p style="text-align: center;">A lightweight game engine for your terminal.</p>
    <div>
        <img src="https://img.shields.io/github/v/release/gmitch215/cmdfx">
        <img src="https://img.shields.io/github/downloads/gmitch215/cmdfx/total">
        <img src="https://img.shields.io/github/license/gmitch215/cmdfx">
        <img src="https://img.shields.io/github/stars/gmitch215/cmdfx?style=flat">
    </div>
</div>

---

**cmdfx** is a lightweight game engine for your terminal. It is designed to be simple and easy to use, while still providing a powerful set of features.

It is written in C and is available cross-platform. It is licensed under the MIT license.

## 🍎 Features

- **Cross-platform**: cmdfx is available on Windows, macOS, and Linux.
- **Lightweight**: cmdfx is designed to be lightweight and fast.
- **Easy to use**: cmdfx is simple to use and easy to learn.
- **Powerful**: cmdfx provides a powerful set of features for creating terminal-based games.

## 📦 Installation

You can download the latest release of cmdfx from the [releases page](https://github.com/gmitch215/cmdfx/releases).

## 🚀 Example

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

## 📝 Contributing

If you would like to contribute to cmdfx, please see the [contributing guidelines](CONTRIBUTING.md). All contributions are welcome!
