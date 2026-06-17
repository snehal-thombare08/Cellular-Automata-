# 🧬 Conway's Game of Life

A real-time, interactive implementation of Conway's Game of Life built with **C++** and **SFML 3.0** — the classic cellular automaton where complex, emergent patterns arise from just four simple rules.

![Game of Life Preview](https://raw.githubusercontent.com/snehal-thombare08/Cellular-Automata-/main/PASTE_YOUR_SCREENSHOT_FILENAME_HERE.png)

## ✨ Features

- **Classic Game of Life rules** — birth, survival, and death based on live neighbor count
- **Interactive drawing** — paint cells alive or dead directly with the mouse
- **Play / Pause / Step** controls — watch generations evolve live or step through one at a time
- **Adjustable simulation speed** — speed up or slow down evolution on the fly
- **Aging color gradient** — newly born cells glow cyan, long-surviving cells fade toward purple, making stable structures visually distinct from active chaos
- **Built-in classic patterns** — load a Glider, a Pulsar oscillator, or a Gosper Glider Gun with a single keypress
- **Random seeding** — generate a fresh random starting state instantly
- **Toggleable grid overlay** for precise cell placement
## screenshot
https://raw.githubusercontent.com/snehal-thombare08/Cellular-Automata-/main/Screenshot%202026-06-17%20122200.png

## 🎮 Controls

| Key / Mouse        | Action                              |
|---------------------|--------------------------------------|
| Left Mouse Button   | Draw (set cells alive)              |
| Right Mouse Button  | Erase (set cells dead)               |
| Space               | Play / Pause                         |
| N                   | Advance one generation (step)        |
| C                   | Clear the grid                       |
| R                   | Randomize the grid                   |
| G                   | Load a Glider                        |
| P                   | Load a Pulsar                        |
| U                   | Load a Gosper Glider Gun              |
| Up / Down Arrow     | Increase / decrease simulation speed |
| ` (backtick)        | Toggle grid lines                    |
| Esc                 | Quit                                  |

## 🧠 How It Works

Each cell on the grid is either alive or dead. Every generation, each cell's next state is decided by counting its eight neighbors:

- A live cell with 2 or 3 live neighbors **survives**
- A live cell with fewer than 2 or more than 3 live neighbors **dies**
- A dead cell with exactly 3 live neighbors **is born**

From these four rules alone, remarkably complex behavior emerges — stable "still life" shapes, oscillating patterns like the Pulsar, and self-propelling structures like the Glider. The Gosper Glider Gun goes a step further, periodically generating new Gliders forever from a fixed structure.

This implementation tracks each cell's age to drive the color gradient, so the simulation visually separates newly active regions from long-term stable structures at a glance.

## 🛠️ Built With

- **C++17**
- **SFML 3.0** — windowing, rendering, and input
- **CMake** — build configuration
- **MinGW** — compiler toolchain (Windows)

## 🚀 Building from Source

### Prerequisites
- CMake 3.16+
- MinGW-w64 (or any C++17 compiler)
- SFML 3.0 (via vcpkg recommended)

### Build Steps

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake -G "MinGW Makefiles"
mingw32-make
./GameOfLife.exe
```

## 📦 Download

Prebuilt Windows binary available under [Releases](../../releases) — just download, extract, and run `GameOfLife.exe`.

## 📄 License

This project is open source and available for learning and experimentation.

---

⭐ If you found this interesting, consider starring the repo!
