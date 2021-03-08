#include "Editor/EditorStartup.hpp"
#include "Editor/Editor.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
//#include "Game/Game.hpp"

namespace Editor
{

EditorStartup::EditorStartup()
    : m_fixedUpdate{[&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
      {        
          if (m_game != nullptr)
              m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
      }},
      m_update{[&](double fixedUnscaledDeltaTime, double deltaTime)
      {
          if (m_game != nullptr)
              m_game->update(fixedUnscaledDeltaTime, deltaTime);
      
          //m_editor->update(fixedUnscaledDeltaTime, deltaTime);
      }},
      m_render{[&]()
      {
          if (m_game != nullptr)
              m_game->render();

          m_editor->update();
          m_editor->render();
      }},
      m_editor{new Editor()}, m_game{nullptr}
{
    GPE_ASSERT(m_editor != nullptr, "m_editor allocation failed");
}


EditorStartup::~EditorStartup()
{
    if (m_game != nullptr)
    {
        //destroyGameInstance(m_game);
    }

    GPE_ASSERT(m_editor != nullptr, "m_editor should be valid since we've just ran the editor.");
    delete m_editor;
}


void EditorStartup::startGame()
{
    if (m_game != nullptr)
    {
        delete m_game;
    }
    //m_game = new Game();
}


void EditorStartup::closeGame()
{
    if (m_game != nullptr)
    {
        delete m_game;
        m_game = nullptr;
    }
}


void EditorStartup::update() 
{
    timeSystem.update(m_update, m_fixedUpdate, m_render);
    isRunning = m_editor->isRunning();
}

} // End of namespace Editor