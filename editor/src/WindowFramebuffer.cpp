#include "Editor/WindowFramebuffer.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "glad/glad.h"

namespace Editor
{

WindowFramebuffer::WindowFramebuffer(int width, int height)
    : m_textureID     {0u},
      m_depthStencilID{0u},
      m_framebufferID {0u},
      m_width         {width},
      m_height        {height}
{
    // Initialize screen texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0u);


    // Initialize depth-stencil buffer
    glGenRenderbuffers(1u, &m_depthStencilID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0u);


    // Initialize the framebuffer itself
    glGenFramebuffers(1, &m_framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilID);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    GPE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
               "An error occured during this framebuffer's generation");

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}


WindowFramebuffer::~WindowFramebuffer()
{
    glDeleteFramebuffers(1, &m_framebufferID);
    glDeleteTextures(1, &m_textureID);
    glDeleteRenderbuffers(1, &m_depthStencilID);
}


void WindowFramebuffer::resize(int width, int height)
{
    if (width == m_width && height == m_height)
    {
        return;
    }

    // Resize texture and depth-stencil buffers
    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
}


void WindowFramebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
    glViewport(0, 0, m_width, m_height);
}

GLint WindowFramebuffer::textureID() const
{
    return m_textureID;
}

GLint WindowFramebuffer::width() const
{
    return m_width;
}

GLint WindowFramebuffer::height() const
{
    return m_height;
}

}