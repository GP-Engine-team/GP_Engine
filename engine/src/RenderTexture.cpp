#include "Engine/Resources/RenderTexture.hpp"

#include <glad/glad.h>

#include "Engine/Core/Debug/Assert.hpp"

using namespace GPE;

RenderTexture::RenderTexture(const CreateArg& arg) noexcept
{
    // Create base buffer
    {
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    GPE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Error in frame buffer generation");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void RenderTexture::resize(unsigned int width, unsigned int height)
{
    m_width  = width;
    m_height = height;
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}

RenderTexture::~RenderTexture()
{
    glDeleteFramebuffers(1, &m_id);
    glDeleteTextures(1, &m_textureId);
}