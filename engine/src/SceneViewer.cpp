#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Script/FreeFly.hpp"

#include <GLFW/glfw3.h>

#include <string>

namespace GPE
{

SceneViewer::SceneViewer(GPE::Scene& viewed, int width, int height)
    : m_pScene{&viewed}, cameraOwner{viewed, {"Editor_camera_" + std::to_string((size_t)this), {}, nullptr}},
      texture({width, height}), depthStencilBuffer({width, height, RenderBuffer::EInternalFormat::DEPTH24_STENCIL8})
{
    GPE::InputManager& iManager = GPE::Engine::getInstance()->inputManager;

    iManager.bindInput(GLFW_KEY_W, "forward");
    iManager.bindInput(GLFW_KEY_S, "back");
    iManager.bindInput(GLFW_KEY_A, "left");
    iManager.bindInput(GLFW_KEY_D, "right");
    iManager.bindInput(GLFW_KEY_SPACE, "up");
    iManager.bindInput(GLFW_KEY_LEFT_CONTROL, "down");
    iManager.bindInput(GLFW_KEY_LEFT_SHIFT, "sprint");

    {
        Camera::PerspectiveCreateArg camArg;
        camArg.name   = "Editor_camera_0";
        camArg.aspect = Camera::computeAspect(width, height);

        pCamera = &cameraOwner.addComponent<Camera>(camArg);
    }

    cameraOwner.addComponent<FreeFly>();

    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getID(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer.getID());
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    GPE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Error in framebuffer generation");

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

SceneViewer::~SceneViewer()
{
    glDeleteFramebuffers(1, &framebufferID);
}

void SceneViewer::resize(int width, int height)
{
    texture.resize(width, height);
    depthStencilBuffer.resize(width, height);

    pCamera->setAspect(Camera::computeAspect(width, height));
}

void SceneViewer::bindScene(Scene& scene) noexcept
{
    if (m_pScene == &scene)
        return;

    cameraOwner.moveTowardScene(scene);
    m_pScene = &scene;
}

void SceneViewer::render() const
{
    m_pScene->sceneRenderer.draw(Engine::getInstance()->resourceManager,
                                 m_pScene->sceneRenderer.defaultRenderPipeline(), framebufferID);
}

} // namespace GPE
