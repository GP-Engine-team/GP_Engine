/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Game/ContextStartup.hpp"

#include <functional>

class AbstractGame;

namespace Editor
{

class Editor;

class EditorStartup final : public ContextStartup
{
private:
    const std::function<void(double, double)> m_fixedUpdate;
    const std::function<void(double, double)> m_update;
    const std::function<void()>               m_render;

    AbstractGame* m_game;
    Editor*       m_editor;

public:
    EditorStartup();
    virtual ~EditorStartup() final;

    void startGame();
    void closeGame();
    virtual void update() override final;
};

} // End of namespace Editor