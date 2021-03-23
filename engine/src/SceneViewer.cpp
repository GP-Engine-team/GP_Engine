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

void SceneViewer::initializeFramebuffer()
{
    { // Initialize screen texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    { // Initialize depth-stencil buffer
        glGenRenderbuffers(1u, &depthStencilID);
        glBindRenderbuffer(GL_RENDERBUFFER, depthStencilID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    { // Initialize the framebuffer itself
        glGenFramebuffers(1, &framebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilID);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        GPE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "An error occured during this framebuffer's generation");

        glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    }
}


SceneViewer::SceneViewer(GPE::Scene& viewed, int width_, int height_)
    : m_pScene{&viewed},
      cameraOwner{viewed, {"Editor_camera_" + std::to_string((size_t)this), {}, nullptr}},
      textureID{0u}, depthStencilID{0u}, framebufferID{0u},
      width{width_}, height{height_}
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
        camArg.name   = cameraOwner.getName().c_str();
        camArg.aspect = Camera::computeAspect(width, height);

        pCamera = &cameraOwner.addComponent<Camera>(camArg);
    }

    cameraOwner.addComponent<FreeFly>();

    initializeFramebuffer();
}


SceneViewer::~SceneViewer()
{
    glDeleteFramebuffers(1, &framebufferID);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &depthStencilID);
}


void SceneViewer::resize(int width_, int height_)
{
    width  = width_;
    height = height_;
    
    // Resize texture and depth-stencil buffers
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);

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
