#pragma once

#include "Engine/Core/Game/ContextStartup.hpp"

class EditorStartup final : public ContextStartup
{
private:
    class AbstractGame* game = nullptr;
    class Editor* editor = nullptr;

public:
    EditorStartup();
    void StartGame();
    void CloseGame();
    virtual void update() override final;
    virtual ~EditorStartup() final;
};