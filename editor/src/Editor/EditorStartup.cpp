#include "Editor/EditorStartup.hpp"
#include "Editor/Editor.hpp"
#include "game.hpp"
#include <cassert>

EditorStartup::EditorStartup() : editor(new Editor())
{

}

void EditorStartup::EditorStartup::StartGame()
{
    if (game != nullptr)
    {
        delete game;
    }
    game = new Game();
}

void EditorStartup::CloseGame()
{
    if (game != nullptr)
    {
        delete game;
        game = nullptr;
    }
}

void EditorStartup::update() 
{
    if (game != nullptr)
    {
        game->update();
    }

    assert(editor != nullptr);
    editor->update();
}

EditorStartup::~EditorStartup()
{
    if (game != nullptr)
    {
        destroyGameInstance(game);
    }

    assert(editor != nullptr);
    delete editor;
}