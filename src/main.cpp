#include "game.h"
#include "logger.h"
#include "win32_window.h"

int main()
{
    Window window;
    Game game;

    // Optional to set debug logs enabled.disabled
    Logger::Get().SetDebugEnabled(true);

    if (!game.Init(window))
    {
        return -1;
    }

    game.Run(window);
    game.Shutdown();
    return 0;
}