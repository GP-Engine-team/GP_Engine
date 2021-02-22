#include "Engine/Resources/RenderTexture.hpp"

#include <glad/glad.h>

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Resources/Texture.hpp"

using namespace GPE;

RenderTexture::RenderTexture(const CreateArg& arg) noexcept
{
    GPE_ASSERT(arg.colorBuffers.size() < 32, "32 color buffer max in OpenGl frameBuffer");

    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    std::vector<GLenum> drawBuffers;
    drawBuffers.reserve(arg.colorBuffers.size());

    for (size_t i = 0; i < arg.colorBuffers.size(); i++)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, arg.colorBuffers[i]->getID(), 0);
        drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(arg.colorBuffers.size(), drawBuffers.data());

    if (arg.depthBuffer != nullptr)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, arg.depthBuffer->getID());
    }

    if (arg.stencilBuffer != nullptr)
    {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, arg.stencilBuffer->getID());
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    GPE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Error in frame buffer generation");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteFramebuffers(1, &m_id);
}