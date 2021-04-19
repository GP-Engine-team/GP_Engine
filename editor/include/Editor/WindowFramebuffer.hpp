/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

// Forward typedef
typedef int          GLint;
typedef unsigned int GLuint;

namespace Editor
{

class WindowFramebuffer
{
private:
    GLuint m_textureID;
    GLuint m_depthStencilID;
    GLuint m_framebufferID;
    GLint  m_width;
    GLint  m_height;

public:
    WindowFramebuffer(int width, int height);
    ~WindowFramebuffer();

    void   resize   (int width, int height);
    void   bind     () const;
    GLint  textureID() const;
};

using WindowFBO = WindowFramebuffer;

}