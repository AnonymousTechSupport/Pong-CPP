#include "game.h"
#include "logger.h"
int main()
{
    Game game;

    // Optional to set debug logs enabled/disabled
    Logger::Get().SetDebugEnabled(true);

    return game.Run();
}
