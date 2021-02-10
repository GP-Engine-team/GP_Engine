#include "Editor/EditorStartup.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Editor/Editor.hpp"
#include "game.hpp"

EditorStartup::EditorStartup() : m_editor(new Editor())
{

}

void EditorStartup::EditorStartup::StartGame()
{
    if (m_game != nullptr)
    {
        delete m_game;
    }
    m_game = new Game();
}

void EditorStartup::CloseGame()
{
    if (m_game != nullptr)
    {
        delete m_game;
        m_game = nullptr;
    }
}

void EditorStartup::update() 
{
    if (m_game != nullptr)
    {
        m_game->update();
    }

    GPE_ASSERT(m_editor != nullptr, "m_editor should be valid since we've just ran the editor.");
    m_editor->update();
}

EditorStartup::~EditorStartup()
{
    if (m_game != nullptr)
    {
        destroyGameInstance(m_game);
    }

    GPE_ASSERT(m_editor != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete m_editor;
}