/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Resources/Texture.hpp>

namespace Editor
{

class EditorStartup;
class Editor;

class GameControlBar
{
public:
    // Helper used for clarity and readability
    enum EButton
    {
        STOP  = 0u,
        PLAY  = (1u << 0),
        PAUSE = (1u << 1)
    };

private:
    // Data members
    GPE::Texture playButtonTex;
    GPE::Texture pauseButtonTex;
    GPE::Texture stopButtonTex;
    unsigned int buttonColors[2];

public:
    unsigned char buttonMask;

    GameControlBar();

    // Methods
    void render(EditorStartup& startup);
};

} // End of namespace Editor