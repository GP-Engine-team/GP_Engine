#pragma once

#include "Engine/Intermediate/RenderSystem.hpp"

namespace GPE
{

class Scene;
class GameObject;

class SceneViewer
{
public:
    Scene*            scene;
    SceneRenderSystem sceneRenderSys;
    GameObject        cameraOwner;

    Texture           texture;
    RenderBuffer      depthBuffer;
    RenderBuffer      stencilBuffer;
    GLuint            framebufferID;

public:
    SceneViewer(GPE::Scene& viewed, int width, int height);
    ~SceneViewer();
    void resize(int width, int height);

    void render() const;
};

} // namespace GPE
