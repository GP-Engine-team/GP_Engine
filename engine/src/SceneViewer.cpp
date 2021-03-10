#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"

namespace GPE
{

GPE::RenderTexture::CreateArg SceneViewer::generatedRenderTextureBuffers()
{
    GPE::RenderTexture::CreateArg args;
    args.colorBuffers.emplace_back(&texture);
    args.depthBuffer   = &depthBuffer;
    args.stencilBuffer = &stencilBuffer;
    return args;
}


SceneViewer::SceneViewer(GPE::Scene& viewed, int width, int height)
    : scene{viewed}, cameraOwner{scene.addGameObject(scene.getRoot())}, camera{cameraOwner, Camera::PerspectiveCreateArg()},
      texture(Texture::CreateArg{width, height}),
      depthBuffer({width, height}),
      stencilBuffer({width, height, RenderBuffer::EInternalFormat::STENCIL_INDEX8})
{

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

}