#include "Engine/Core/Debug/Log.hpp"

#include "Engine/Core/Game/ContextStartup.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"

#include "game.hpp"

#include <cassert>

class Editor
{
public:
    void update()
    {

    }
};

class EditorStartup final : public ContextStartup
{
private:
    class AbstractGame* game = nullptr;
    class Editor* editor = nullptr;

public:
    EditorStartup() : editor(new Editor())
    {

    }

    void StartGame()
    {
        if (game != nullptr)
        {
            delete game;
        }
        game = new Game();
    }

    void CloseGame()
    {
        if (game != nullptr)
        {
            delete game;
            game = nullptr;
        }
    }

    virtual void update() override final
    {
        if (game != nullptr)
        {
            game->update();
        }

        assert(editor != nullptr);
        editor->update();
    }

    virtual ~EditorStartup() final
    {
        if (game != nullptr)
        {
            destroyGameInstance(game);
        }

        assert(editor != nullptr);
        delete editor;
    }
};

using namespace Engine::Core::Debug;

int main(int argc, char* argv[])
{
    Log::logFileHeader();

    Log::logInitializationStart("GameStartup creation");

    EditorStartup editorStartup;
    editorStartup.run();

    return 0;
}