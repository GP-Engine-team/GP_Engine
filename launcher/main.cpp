#include "Engine/Core/Debug/Log.hpp"

#include "Engine/Core/Game/ContextStartup.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"

#include "game.hpp"

#include <cassert>

class GameStartup final : public ContextStartup
{
private:
    class AbstractGame* game;

public:
    GameStartup() : game(createGameInstance())
    {

    }

    virtual void update() override final
    {
        assert(game != nullptr);
        game->update();
    }

    virtual ~GameStartup() final
    {
        destroyGameInstance(game);
    }
};

using namespace Engine::Core::Debug;

int main(int argc, char* argv[])
{
    Log::logFileHeader();

    Log::logInitializationStart("GameStartup creation");

    GameStartup gameStartup;
    gameStartup.run();

    return 0;
}