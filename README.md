## Pong Engine in C++

A small game engine using C++, SDL3 and OpenGL. Premake is provided for project generation.

Requirements
- C++ toolchain (MSVC on Windows, or gcc/clang on Linux/macOS)
- SDL3 (bundled at `third_party/SDL3-3.4.2` for convenience)

Quick build
- Windows: run `build.bat` from the repository root. Output: `bin\\main.exe`.
- Linux / macOS: run `make -f build/Makefile` from the repository root. Output: `bin/main`.

Run
- Execute the built binary: `bin\main.exe` on Windows or `./bin/main` on Linux/macOS.

Project layout
- `src/` — engine source code
  - `src/main.cpp` — program entry, startup and main loop orchestration
  - `src/platform/` — platform abstraction and windowing backends
  - `src/renderer/` — rendering code and public renderer interface
  - `src/utils/` — small utilities and helpers

- `third_party/SDL3-3.4.2/` — bundled SDL3 headers and prebuilt libraries for supported architectures; used to avoid external dependency setup during development

- `build/` — build makefiles and intermediate build scripts (`build/Makefile`, `build/Engine.make`)
- `build.bat` — Windows convenience script to build the project
- `premake5.lua` — premake project generator (optional; use if you prefer IDE project files)

- `bin/` — build outputs and runtime DLLs (e.g. `main.exe`, `SDL3.dll`)
- `compile_flags.txt` — recommended compiler flags used during development
- `.clang-format` — formatting rules for consistent code style
