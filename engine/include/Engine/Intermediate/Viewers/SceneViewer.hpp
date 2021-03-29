#pragma once

#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{

class FreeFly;
class Scene;
class Camera;

class SceneViewer
{
public:
    GameObject    cameraOwner;
    FreeFly&      freeFly;
    Camera&       camera;
    Scene*        pScene;
    GameObject::Children::iterator it;

    unsigned int  textureID;
    unsigned int  depthStencilID;
    unsigned int  framebufferID;
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
    void captureInputs(bool shouldCapture);
};

} // namespace GPE
