#include "Editor/Viewport.hpp"

#include "glad/glad.h"

namespace Editor
{

Viewport::Viewport(float width, float height)
    : texture{0u}
{
    glGenTextures(1, &texture); // Initialize OpenGL texture
    ImGui::SetNextWindowSize(ImVec2{width, height}, ImGuiCond_FirstUseEver);
}


Viewport::~Viewport()
{
    // Free OpenGL texture
    glDeleteTextures(1, &texture);
}


// Casting tmp as intptr_t and then void* is a safe way to proceed
// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
void Viewport::render() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
    ImGui::Begin("Viewport");
    ImGui::BeginChild("Viewport-1");
    ImGui::Image((void*)(intptr_t)texture, ImGui::GetWindowSize(),
                 ImVec2{0.f, 1.f}, ImVec2{1.f, 0.f}); // UV --> VU
    ImGui::EndChild();
    ImGui::End();
}

} // End of namespace Editor