#pragma once

#include "Engine/Intermediate/GameObject.hpp"
#include "glad/glad.h"

namespace GPE
{

class Scene;

class SceneViewer
{
public:
    GameObject    cameraOwner;
    Scene*        m_pScene;
    class Camera* pCamera;

    GLuint        textureID;
    GLuint        depthStencilID;
    GLuint        framebufferID;
    int           width;
    int           height;

private:
    void initializeFramebuffer();

public:
    SceneViewer(GPE::Scene& viewed, int width = 1, int height = 1);
    ~SceneViewer();

    void resize   (int width, int height);
    void bindScene(Scene& scene)          noexcept;
    void render   ()                      const;
};

} // namespace GPE
