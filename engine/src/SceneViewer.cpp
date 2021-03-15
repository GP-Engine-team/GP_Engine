#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/ECS/Components/Camera.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"


namespace GPE
{

SceneViewer::SceneViewer(GPE::Scene& viewed, int width, int height)
    : scene{viewed}, cameraOwner{scene, {"Editor_camera_0", {}, nullptr}},
      texture({width, height}),
      depthBuffer({width, height}),
      stencilBuffer({width, height, RenderBuffer::EInternalFormat::STENCIL_INDEX8})
{
    const Camera::PerspectiveCreateArg args{16.f / 9.f, .001f, 1000.f, 90.f, "Editor_camera_0"};
    cameraOwner.addComponent<Camera>(args);

    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getID(), 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer.getID());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer.getID());

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
    depthBuffer.resize(width, height);
    stencilBuffer.resize(width, height);
}


void SceneViewer::render() const
{
    //GPE::RenderSystem::getInstance()->draw(scene, renderTexture);
}

} // End of namespace Editor