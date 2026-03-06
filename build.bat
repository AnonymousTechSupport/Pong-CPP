@echo off

if not exist bin mkdir bin

clang++ -std=c++23 -g -O0 -Wall src\main.cpp src\platform\win32_window.cpp src\renderer\renderer.cpp src\game\game.cpp src\game\entity\player.cpp src\game\entity\ball.cpp src\game\entity\enemy.cpp src\utils\timer.cpp src\utils\logger.cpp -o bin\main.exe -Isrc -Isrc/game -Isrc/renderer -Isrc/platform -Isrc/utils -lgdi32 -lopengl32 -luser32 -lkernel32