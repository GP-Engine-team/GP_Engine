#pragma once

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Texture.hpp"
#include "Engine/Resources/RenderBuffer.hpp"
#include "glad/glad.h"

namespace GPE
{

class Scene;

class SceneViewer
{
public:
    Scene&            scene;
    GameObject        cameraOwner;

    Texture           texture;
    RenderBuffer      depthStencilBuffer;
    GLuint            framebufferID;

public:
    SceneViewer(GPE::Scene& viewed, int width, int height);
    ~SceneViewer();
    void resize(int width, int height);

    void render() const;
};

} // namespace GPE
