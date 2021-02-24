#include "Editor/Viewport.hpp"

#include "glad/glad.h"

namespace Editor
{

Viewport::Viewport(float width_, float height_)
    : width{static_cast<GLsizei>(width_)},
      height{static_cast<GLsizei>(height_)},
      id{0u}, screen{0u}
{
    // Screen texture
    glGenTextures(1, &screen);
    glBindTexture(GL_TEXTURE_2D, screen);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Create depth buffer
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Framebuffer generation & configuration
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Viewport::~Viewport()
{
    // Free OpenGL texture
    glDeleteFramebuffers(1, &id);
    glDeleteTextures(1, &screen);
    glDeleteRenderbuffers(1, &depth);
}


void Viewport::resize(int width_, int height_)
{
    width  = width_;
    height = height_;
    glBindTexture(GL_TEXTURE_2D, screen);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

// Casting screen as intptr_t and then void* is a safe way to proceed
// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
void Viewport::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glBindTexture(GL_TEXTURE_2D, screen);
    //glClearColor(1.f, .5f, .25f, 1.f);
    //glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Image((void*)(intptr_t)screen, ImVec2{100.f, 100.f},
                 ImVec2{0.f, 1.f}, ImVec2{1.f, 0.f}); // UV --> VU

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // End of namespace Editor