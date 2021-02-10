/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

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
