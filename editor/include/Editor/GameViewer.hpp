/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "WindowFramebuffer.hpp"

namespace Editor
{

class EditorStartup;

class GameViewer
{
public:
    WindowFBO framebuffer;

private:
    bool      m_captureInputs;

public:
    GameViewer(int width = 1, int height = 1);

    void render(EditorStartup& startup);
};

}