#pragma once

#include "imgui/imgui.h"
#include "glad/glad.h"

namespace Editor
{

struct Viewport
{
    GLsizei width {0u};
    GLsizei height{0u};
    GLuint  id    {0u};
    GLuint  screen{0u};
    GLuint  depth {0u};

    Viewport() = default;
    Viewport(float width, float height);
    ~Viewport();
    
    void resize(int width, int height);
    void render() const;
};

} // End of namespace Editor