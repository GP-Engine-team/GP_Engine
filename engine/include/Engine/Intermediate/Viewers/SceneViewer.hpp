#pragma once


#include "Engine/Intermediate/RenderSystem.hpp"
#include "Engine/Resources/Camera.hpp"

namespace GPE
{

class Scene;
class GameObject;

class SceneViewer
{
public:
    Scene&        scene;
    GameObject&   cameraOwner;
    Camera        camera;

    Texture       texture;
    RenderBuffer  depthBuffer;
    RenderBuffer  stencilBuffer;

private:
    GPE::RenderTexture::CreateArg generatedRenderTextureBuffers();

public:
    SceneViewer(GPE::Scene& viewed, int width, int height);

    void resize(int width, int height);

    void render() const;
};

} // namespace GPE
