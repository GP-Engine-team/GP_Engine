#pragma once

#include "Engine/Core/Game/ContextStartup.hpp"

class EditorStartup final : public ContextStartup
{
private:
    class AbstractGame* m_game = nullptr;
    class Editor* m_editor = nullptr;

public:
    EditorStartup();
    void StartGame();
    void CloseGame();
    virtual void update() override final;
    virtual ~EditorStartup() final;
};