// --- Program entry
// -------------------------------------------------------
#include "game/game.h"
#include "utils/logger.h"

int main()
{
    Game game;

    // Set to false to suppress verbose per-frame debug output.
    Logger::Get().SetDebugEnabled(true);

    return game.Run();
}
