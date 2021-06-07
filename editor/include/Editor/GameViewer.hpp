/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "WindowFramebuffer.hpp"

struct GLFWwindow;

namespace Editor
{

class EditorStartup;

class GameViewer
{
public:
    WindowFBO   framebuffer;
    GLFWwindow* window;

private:
    bool m_captureInputs           = false;
    bool cursorLockStateInGame     = false;
    bool cursorTrackingStateInGame = false;
    bool setMouseInWindow          = false;

    void checkInputs(EditorStartup& startup);

public:
    GameViewer(int width = 1, int height = 1);

    void render(EditorStartup& startup);

    void captureInputs();
    void releaseInputs();
};

} // namespace Editor