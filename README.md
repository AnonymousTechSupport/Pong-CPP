# Pong-CPP Engine

This repository contains a simple 2D Pong-style game engine written in C++
for Windows using the Win32 API and OpenGL for rendering.

## Features

- Window management via Win32
- Simple renderer with orthographic projection (OpenGL 1.1 style)
- Input handling (keyboard/mouse)
- Basic game loop and entity management
- Logging utility with debug toggle
- Modular code organized into `src` subdirectories

## Building

A batch file `build.bat` compiles the project using `clang++`:

Run the resulting `bin\main.exe` to launch the game. You can enable
debug logging by calling `Logger::Get().SetDebugEnabled(true);` in `main()`.

## Usage

The entry point is `main.cpp`, which creates a `Window` and `Game` object.
Game state updates and rendering occur within the `Game::Run` loop.

### Logging

The `utils/logger` module provides levelled logging to the debug output.
Call `Logger::Get().LogInfo()`, `LogWarning()`, `LogError()`, or `LogDebug()`.
Debug messages are controlled via `SetDebugEnabled()`.

### Entities

Player and enemy paddles are represented by simple structs and drawn by
`Renderer`.  Coordinates and sizes are configured in `Game::InitializeEntities()`.

## License

This project is provided without warranty under the MIT license.
