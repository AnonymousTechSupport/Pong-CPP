#include "input.h"
#include "utils/logger.h"
#include <cctype>
#include <string>

// --- Input implementation
// ---------------------------------------------------------

// IInput::Get() is defined here so that callers including only
// iinput.h get the concrete Input singleton without seeing SDL types.
IInput& IInput::Get()
{
    return Input::Get();
}

Input& Input::Get()
{
    static Input instance;
    return instance;
}

Input::Input()
{
    Clear();
}

SDL_Scancode Input::ResolveScancode(int key) const
{
    if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z'))
    {
        int upper = std::toupper(static_cast<unsigned char>(key));
        return static_cast<SDL_Scancode>(SDL_SCANCODE_A + (upper - 'A'));
    }
    else if (key >= 0 && static_cast<size_t>(key) < Input::kKeyCount)
    {
        return static_cast<SDL_Scancode>(key);
    }
    return SDL_SCANCODE_UNKNOWN;
}

int Input::ResolveKeyIndex(int key) const
{
    SDL_Scancode sc = ResolveScancode(key);
    if (sc == SDL_SCANCODE_UNKNOWN)
        return -1;
    size_t idx = static_cast<size_t>(sc);
    if (idx >= m_keys.size())
        return -1;
    return static_cast<int>(idx);
}

std::string Input::ScancodeName(SDL_Scancode sc) const
{
    if (sc == SDL_SCANCODE_UNKNOWN)
        return "UNKNOWN";

    if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z)
    {
        char c = static_cast<char>('A' + (sc - SDL_SCANCODE_A));
        return std::string(1, c);
    }

#if defined(SDL_GetScancodeName)
    const char* name = SDL_GetScancodeName(sc);
    if (name && name[0] != '\0')
        return std::string(name);
#endif

    return std::string("SC_") + std::to_string(static_cast<int>(sc));
}

void Input::Update()
{
    m_prevKeys = m_keys;
}

void Input::Clear()
{
    m_keys.reset();
    m_prevKeys.reset();
    m_mouseButtons.fill(false);
}

void Input::ProcessSDLInput(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
    {
        SDL_Scancode scancode = event.key.scancode;
        if (static_cast<size_t>(scancode) < m_keys.size())
            m_keys.set(static_cast<size_t>(scancode));
        LOG_DEBUG("-> KEYDOWN CODE: " + std::to_string(static_cast<int>(scancode)) + " (" +
                  ScancodeName(scancode) + ")");
        break;
    }

    case SDL_EVENT_KEY_UP:
    {
        SDL_Scancode scancode = event.key.scancode;
        if (static_cast<size_t>(scancode) < m_keys.size())
            m_keys.reset(static_cast<size_t>(scancode));
        LOG_DEBUG("-> KEYUP   CODE: " + std::to_string(static_cast<int>(scancode)) + " (" +
                  ScancodeName(scancode) + ")");
        break;
    }

    case SDL_EVENT_MOUSE_MOTION:
        m_mouseX = static_cast<int>(event.motion.x);
        m_mouseY = static_cast<int>(event.motion.y);
        LOG_DEBUG("-> MOUSE MOTION: (" + std::to_string(m_mouseX) + ", " +
                  std::to_string(m_mouseY) + ")");
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
            m_mouseButtons[static_cast<int>(MouseButton::Left)] = true;
        else if (event.button.button == SDL_BUTTON_RIGHT)
            m_mouseButtons[static_cast<int>(MouseButton::Right)] = true;
        else if (event.button.button == SDL_BUTTON_MIDDLE)
            m_mouseButtons[static_cast<int>(MouseButton::Middle)] = true;
        LOG_DEBUG("-> MOUSE BUTTON DOWN: " + std::to_string(event.button.button));
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button == SDL_BUTTON_LEFT)
            m_mouseButtons[static_cast<int>(MouseButton::Left)] = false;
        else if (event.button.button == SDL_BUTTON_RIGHT)
            m_mouseButtons[static_cast<int>(MouseButton::Right)] = false;
        else if (event.button.button == SDL_BUTTON_MIDDLE)
            m_mouseButtons[static_cast<int>(MouseButton::Middle)] = false;
        break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        Clear();
        break;

    default:
        break;
    }
}

bool Input::IsKeyDown(int key) const
{
    int idx = ResolveKeyIndex(key);
    if (idx < 0)
        return false;
    return m_keys.test(static_cast<size_t>(idx));
}

bool Input::IsMouseButtonDown(MouseButton button) const
{
    return m_mouseButtons[static_cast<int>(button)];
}

bool Input::IsKeyPressed(int key) const
{
    int idx = ResolveKeyIndex(key);
    if (idx < 0)
        return false;
    return m_keys.test(static_cast<size_t>(idx)) && !m_prevKeys.test(static_cast<size_t>(idx));
}

bool Input::IsKeyReleased(int key) const
{
    int idx = ResolveKeyIndex(key);
    if (idx < 0)
        return false;
    return !m_keys.test(static_cast<size_t>(idx)) && m_prevKeys.test(static_cast<size_t>(idx));
}

Vector2 Input::GetMousePosition() const
{
    return {static_cast<float>(m_mouseX), static_cast<float>(m_mouseY)};
}

void Input::SetMousePosition(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}
