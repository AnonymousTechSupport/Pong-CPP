#pragma once
#include "utils/input/interface/iinput.h"
#include <SDL3/SDL.h>
#include <array>
#include <bitset>
#include <string>

// --- Input (SDL-backed implementation)
// -----------------------------------------
class Input : public IInput
{
  public:
    static Input& Get();

    void Update() override;
    bool IsKeyDown(int key) const override;
    bool IsKeyPressed(int key) const override;
    bool IsKeyReleased(int key) const override;
    bool IsMouseButtonDown(MouseButton button) const override;
    Vector2 GetMousePosition() const override;
    void SetMousePosition(int x, int y) override;
    void Clear() override;

    // SDL-only helper: forward SDL events into the input system.
    void ProcessSDLInput(const SDL_Event& event);

  private:
    Input();

    // Resolve an input integer (ASCII letter or raw scancode index) to an SDL_Scancode.
    // Accepts 'A'..'Z' or 'a'..'z' and numeric scancode indices.
    SDL_Scancode ResolveScancode(int key) const;
    // Resolve to a bitset index, or -1 if invalid.
    int ResolveKeyIndex(int key) const;
    // Return a short human-readable name for a scancode.
    std::string ScancodeName(SDL_Scancode sc) const;

    static constexpr size_t kKeyCount = 512;
    std::bitset<kKeyCount> m_keys;
    std::bitset<kKeyCount> m_prevKeys;
    std::array<bool, 3> m_mouseButtons;
    int m_mouseX = 0;
    int m_mouseY = 0;
};
