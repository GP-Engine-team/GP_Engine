#include "Engine/Resources/RenderBuffer.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "glad/glad.h"

using namespace GPE;

RenderBuffer::RenderBuffer(const CreateArg& arg) noexcept
{
    glGenRenderbuffers(1, &m_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(arg.internalFormat), arg.width, arg.height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer() noexcept
{
    glDeleteRenderbuffers(1, &m_id);
    Log::log("RenderBuffer release");
}