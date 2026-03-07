#include "input.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "utils/logger.h"

Input& Input::Get()
{
    static Input instance;
    return instance;
}

Input::Input()
{
    Clear();
    m_mouseX = 0;
    m_mouseY = 0;
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
    SDL_Scancode scancode = event.key.scancode;
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
        if (scancode < m_keys.size())
            m_keys.set(scancode);

        LOG_DEBUG(
            "-> KEYDOWN CODE: " +
            std::to_string(static_cast<int>(scancode)));
        break;

    case SDL_EVENT_KEY_UP:
        if (scancode < m_keys.size())
            m_keys.reset(scancode);

        LOG_DEBUG(
            "-> KEYUP CODE: " +
            std::to_string(static_cast<int>(scancode)));
        break;
    case SDL_EVENT_MOUSE_MOTION:
        m_mouseX = event.motion.x;
        m_mouseY = event.motion.y;

        LOG_DEBUG(
            "-> MOUSE MOTION: (" + std::to_string(m_mouseX) + ", " +
            std::to_string(m_mouseY) + ")");
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:

        if (event.button.button == SDL_BUTTON_LEFT)
            m_mouseButtons[static_cast<int>(MouseButton::Left)] =
                true;
        else if (event.button.button == SDL_BUTTON_RIGHT)
            m_mouseButtons[static_cast<int>(MouseButton::Right)] =
                true;
        else if (event.button.button == SDL_BUTTON_MIDDLE)
            m_mouseButtons[static_cast<int>(MouseButton::Middle)] =
                true;

        LOG_DEBUG(
            "-> MOUSE BUTTON DOWN: " +
            std::to_string(event.button.button));
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button == SDL_BUTTON_LEFT)
            m_mouseButtons[static_cast<int>(MouseButton::Left)] =
                false;
        else if (event.button.button == SDL_BUTTON_RIGHT)
            m_mouseButtons[static_cast<int>(MouseButton::Right)] =
                false;
        else if (event.button.button == SDL_BUTTON_MIDDLE)
            m_mouseButtons[static_cast<int>(MouseButton::Middle)] =
                false;
        break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        Clear();
        break;

    default:
        break;
    }
}

bool Input::isKeyDown(int key) const
{
    if (key < 0 || key >= static_cast<int>(m_keys.size()))
        return false;

    return m_keys.test(static_cast<size_t>(key));
}

bool Input::isMouseButtonDown(MouseButton button) const
{
    return m_mouseButtons[static_cast<int>(button)];
}

bool Input::isKeyPressed(int key) const
{
    if (key < 0 || key >= static_cast<int>(m_keys.size()))
        return false;

    return m_keys.test(static_cast<size_t>(key)) &&
           !m_prevKeys.test(static_cast<size_t>(key));
}

bool Input::isKeyReleased(int key) const
{
    if (key < 0 || key >= static_cast<int>(m_keys.size()))
        return false;

    return !m_keys.test(static_cast<size_t>(key)) &&
           m_prevKeys.test(static_cast<size_t>(key));
}

Vector2 Input::GetMousePosition() const
{
    LOG_DEBUG(
        "Mouse Position: (" + std::to_string(m_mouseX) + ", " +
        std::to_string(m_mouseY) + ")");
    Vector2 position;
    position.x = m_mouseX;
    position.y = m_mouseY;
    return position;
}

void Input::SetMousePosition(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;

    LOG_DEBUG(
        "Set Mouse Position: (" + std::to_string(m_mouseX) + ", " +
        std::to_string(m_mouseY) + ")");
}
