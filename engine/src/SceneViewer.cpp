#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Script/FreeFly.hpp"
#include "GPM/DebugOutput.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <memory>

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
    : cameraOwner   {viewed, {"Editor_camera", {}, &viewed.world}},
      freeFly       {cameraOwner.addComponent<FreeFly>()},
      camera        {cameraOwner.addComponent<Camera>(Camera::PerspectiveCreateArg{width_ / (float)height_, .001f, 1000.f, 90.f})},
      pScene        {&viewed},
      textureID     {0u},
      depthStencilID{0u},
      framebufferID {0u},
      width         {width_},
      height        {height_},
      it            {viewed.world.children.emplace(viewed.world.children.end(), &cameraOwner)}
{
    initializeFramebuffer();
}


SceneViewer::~SceneViewer()
{
    //cameraOwner.destroyUniqueComponentNow<Camera>();
    //cameraOwner.destroyUniqueComponentNow<FreeFly>();
    it->release();
    pScene->world.destroyChild(&cameraOwner);

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

    camera.setAspect(width / (float)height);
}


void SceneViewer::bindScene(Scene& scene) noexcept
{
    if (pScene == &scene)
    {
        return;
    }

    { // Move cameraOwner to the other scene
        // Transfer ownership of &cameraOwner to the new scene
        using iterator = GameObject::Children::iterator;
        const iterator newIt = scene.world.children.emplace(scene.world.children.end(), it->release());

        // Update the previous scene and the iterator to cameraOwner's parent's children list
        pScene->world.children.erase(it);
        it = newIt;
    }

    // Update the Camera component and cameraOwner scene and parent
    camera.moveTowardScene(scene);
    cameraOwner.setParent(scene.world);
    cameraOwner.pOwnerScene = &scene;
    pScene = &scene;
}


void SceneViewer::render() const
{
    // TODO: make sure camera.updateView() is called outside of this scope,
    // then remove it here
    camera.updateView();
    pScene->sceneRenderer.draw(Engine::getInstance()->resourceManager,
                               pScene->sceneRenderer.defaultRenderPipeline(), framebufferID);
}


void SceneViewer::captureInputs(bool shouldCapture)
{
    freeFly.enableUpdate(shouldCapture);
    freeFly.setActive(shouldCapture);
}

} // namespace GPE
