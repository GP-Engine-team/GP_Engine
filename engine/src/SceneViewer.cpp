﻿#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Script/FreeFly.hpp"
#include "Engine/Core/Debug/Log.hpp"

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
    : pScene{&viewed},
      cameraOwner{viewed, {"Editor_camera_" + std::to_string((size_t)this), {}, nullptr}},
      textureID{0u}, depthStencilID{0u}, framebufferID{0u},
      width{width_}, height{height_},
      m_captureInputs{false}
{
    {
        Camera::PerspectiveCreateArg camArg;
        camArg.name   = cameraOwner.getName().c_str();
        camArg.aspect = Camera::computeAspect(width, height);

        pCamera = &cameraOwner.addComponent<Camera>(camArg);
    }

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
    if (width == width_ && height == height_)
    {
        return;
    }

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
    if (pScene == &scene)
    {
        return;
    }

    cameraOwner.moveTowardScene(scene);
    pScene = &scene;
}


void SceneViewer::render() const
{
    pScene->sceneRenderer.draw(Engine::getInstance()->resourceManager,
                               pScene->sceneRenderer.defaultRenderPipeline(), framebufferID);
}


void SceneViewer::captureInputs()
{
    if (m_captureInputs)
    {
        return;
    }

    m_captureInputs = true;
    cameraOwner.addComponent<FreeFly>();
    //cameraOwner.addComponent<GPE::InputComponent>();
}


void SceneViewer::releaseInputs()
{
    if (!m_captureInputs)
    {
        return;
    }

    m_captureInputs = false;
    cameraOwner.destroyUniqueComponentNow<FreeFly>();
    //cameraOwner.destroyUniqueComponentNow<GPE::InputComponent>();
}

} // namespace GPE
