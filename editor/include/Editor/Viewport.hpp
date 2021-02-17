#pragma once

#include "imgui/imgui.h"
#include "glad/glad.h"

namespace Editor
{

class Viewport
{
private:
    GLuint texture;

public:
    Viewport(float width, float height);
    ~Viewport();
    
    void render() const;
};

} // End of namespace Editor