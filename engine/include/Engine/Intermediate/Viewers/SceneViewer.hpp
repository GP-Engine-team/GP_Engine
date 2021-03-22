#pragma once

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/RenderBuffer.hpp"
#include "Engine/Resources/Texture.hpp"
#include "glad/glad.h"

namespace GPE
{

class Scene;

class SceneViewer
{
protected:
    Scene* m_pScene;

public:
    GameObject    cameraOwner;
    class Camera* pCamera;

    Texture      texture;
    RenderBuffer depthStencilBuffer;
    GLuint       framebufferID;

public:
    SceneViewer(GPE::Scene& viewed, int width, int height);
    ~SceneViewer();
    void resize(int width, int height);

    void bindScene(Scene& scene) noexcept;

    void render() const;
};

} // namespace GPE
