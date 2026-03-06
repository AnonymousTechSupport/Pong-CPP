# Pong-CPP Engine

A compact, Windows‑only 2D game engine written in modern C++23.

---

- **Platform:** Windows (Win32 API + OpenGL 1.1)
- **Executable:** `bin\main.exe` (built via `build.bat`)
- **Architecture:** modular `src/` folders: `game`, `renderer`,
  `platform`, `utils`, etc.

---

## Folder layout

```
src/
  main.cpp            # program entry point
  game/               # game logic and entity system
  renderer/           # OpenGL rendering wrapper
  platform/           # Win32 windowing and message pump
  utils/              # logging, input, timer, etc.
```

---

## Building

1. Open a terminal in the workspace root.
2. Run `build.bat` (requires clang++ on PATH).
3. Launch `bin\main.exe`; no command‑line arguments needed.

> Tip: enable debug output by calling
> `Logger::Get().SetDebugEnabled(true);` in `main()` before `game.Run()`.

---

## Usage & Extension

- **Initialization** is handled internally; `Game::Run()` creates the
  window, initializes subsystems, and enters the loop.
- **Main loop:** `Game::RunLoop` processes Win32 messages, updates input,
  advances the timer, updates all entities, and queues render commands.
- **Entities:** Implement `GameEntity` (see `entity.h`). New entities are
  added in `Game::InitializeEntities()`; they may query window dimensions via
  the `Window` reference provided in their constructors.

---

## Tools & Logging

The logger outputs colourised messages to the console (green/info, yellow/
warning, red/error, cyan/debug). Level control is available at runtime via the `Logger` instance. Input handling currently supports keyboard and
mouse via a simple Win32 message forwarder.
