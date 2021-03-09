#include "Engine/Resources/RenderBuffer.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "glad/glad.h"

using namespace GPE;

RenderBuffer::RenderBuffer(const CreateArg& arg) noexcept
    : internalFormat{(GLenum)arg.internalFormat}
{
    switch(internalFormat)
    {
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32F:
        bufferType = GL_DEPTH_COMPONENT;
        break;

    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
        bufferType = GL_DEPTH_STENCIL;
        break;

    case GL_STENCIL_INDEX8:
        bufferType = GL_STENCIL_INDEX;
        break;

    default:
        FUNCT_WARNING((std::string("Unsupported internal format (") + std::to_string(internalFormat) + ')').c_str());
        return;
    }

    glGenRenderbuffers(1, &m_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(arg.internalFormat), arg.width, arg.height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer() noexcept
{
    glDeleteRenderbuffers(1, &m_id);
    Log::log("RenderBuffer released");
}


void RenderBuffer::resize(int width, int height)
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_RENDERBUFFER, bufferType, width, height);
}