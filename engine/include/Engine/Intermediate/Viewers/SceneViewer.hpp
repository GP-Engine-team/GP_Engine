#pragma once

#include "Engine/Intermediate/GameObject.hpp"
#include "glad/glad.h"

#include <iostream>
#include <stdlib.h>

namespace GPE
{

class Scene;

class SceneViewer
{
public:
    GameObject    cameraOwner;
    Scene*        pScene;
    class Camera* pCamera;

    GLuint textureID;
    GLuint depthStencilID;
    GLuint framebufferID;
    int    width;
    int    height;

private:
    bool m_captureInputs;

    void initializeFramebuffer();

public:
    SceneViewer(GPE::Scene& viewed, int width = 1, int height = 1);
    ~SceneViewer();

    /**
     * @brief Generate texture to get the ID of selected element thank's to GPU ID buffer.
     * This texture is down sampleted to increase performance.
     * Use this function only if user need ID of selected object only.
     * Don't use it in loop because it will created a texture each iteration
     * @return
     */
    unsigned int getIDOfSelectedGameObject() const;

    void resize(int width, int height);
    void bindScene(Scene& scene) noexcept;
    void render() const;
    void captureInputs();
    void releaseInputs();
};

} // namespace GPE
