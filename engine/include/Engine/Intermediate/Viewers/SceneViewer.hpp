#pragma once

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"
#include "Engine/Resources/Camera.hpp"
#include "Engine/Resources/Scene.hpp"

namespace GPE
{

class SceneViewer
{
public:
    GPE::Scene&        scene;
    GPE::GameObject&   cameraOwner;
    GPE::Camera        camera;
    GPE::RenderTexture renderTexture;

    Texture      texture;
    RenderBuffer depthBuffer;
    RenderBuffer stencilBuffer;

private:
    GPE::RenderTexture::CreateArg generatedRenderTextureBuffers()
    {
        GPE::RenderTexture::CreateArg args;
        args.colorBuffers.emplace_back(&texture);
        args.depthBuffer   = &depthBuffer;
        args.stencilBuffer = &stencilBuffer;
        return args;
    }

public:
    SceneViewer(GPE::Scene& viewed, int width, int height)
        : scene{viewed}, cameraOwner{scene.addGameObject(scene.getRoot())}, camera{cameraOwner, {}},
          renderTexture{generatedRenderTextureBuffers()}, texture(width, height), depthBuffer(width, height),
          stencilBuffer(width, height)
    {

    }

    void resize(int width, int height)
    {
        texture.resize(width, height);
        depthBuffer.resize(width, height);
        stencilBuffer.resize(width, height);
    }

    void render() const
    {
        GPE::RenderSystem::getInstance()->draw(scene, renderTexture);
    }
};

} // namespace GPE
