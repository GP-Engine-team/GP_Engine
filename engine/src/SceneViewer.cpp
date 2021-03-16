#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"

#include <string>

namespace GPE
{

SceneViewer::SceneViewer(GPE::Scene& viewed, int width, int height)
    : scene{viewed}, cameraOwner{scene, {"Editor_camera_" + std::to_string((size_t)this), {}, nullptr}}, texture({width, height}),
      depthStencilBuffer({width, height, RenderBuffer::EInternalFormat::DEPTH24_STENCIL8})
{
    const Camera::PerspectiveCreateArg args{16.f / 9.f, .001f, 1000.f, 90.f, "Editor_camera_0"};
    cameraOwner.addComponent<Camera>(args);

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
}

void SceneViewer::render() const
{
    /*   scene.sceneRenderer.draw(SystemsManager::getInstance()->resourceManager,
                                scene.sceneRenderer.defaultRenderPipeline(), framebufferID);*/
}

} // namespace GPE
