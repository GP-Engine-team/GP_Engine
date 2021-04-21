#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Script/FreeFly.hpp"

#include "glad/glad.h"

namespace GPE
{

#define INV_DOWN_SAMPLING_COEF .25f

// ========================== Private methods ==========================
void SceneViewer::initializeFramebuffer()
{
    // TODO: factorize framebuffer and renderbuffer in classes
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

    { // Initialize depth-stencil renderbuffer
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

        GPE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                   "An error occured during this framebuffer's generation");

        glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    }
}


void SceneViewer::initializePickingFBO()
{
    // low sampling (we don't need 4K texture to select element)
    FBOIDwidth  = static_cast<int>(ceilf(width  * INV_DOWN_SAMPLING_COEF));
    FBOIDheight = static_cast<int>(ceilf(height * INV_DOWN_SAMPLING_COEF));

    // Create FBO
    { // Initialize screen texture
        glGenTextures(1, &FBOIDtextureID);
        glBindTexture(GL_TEXTURE_2D, FBOIDtextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, FBOIDwidth, FBOIDheight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    { // Initialize depth buffer
        glGenRenderbuffers(1u, &FBOIDdepthID);
        glBindRenderbuffer(GL_RENDERBUFFER, FBOIDdepthID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FBOIDwidth, FBOIDheight);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    { // Initialize the framebuffer itself
        glGenFramebuffers(1, &FBOIDframebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, FBOIDframebufferID);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOIDtextureID, 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBOIDdepthID);
        GPE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                   "An error occured during this framebuffer's generation");
    }
}




// ========================== Public methods ==========================
SceneViewer::SceneViewer(GPE::Scene& viewed, int width_, int height_)
    : cameraOwner    {new GameObject(viewed, {"Editor camera", {}, &viewed.getWorld()})},
      freeFly        {cameraOwner->addComponent<FreeFly>()},
      camera         {cameraOwner->addComponent<Camera>(Camera::PerspectiveCreateArg{width_ / (float)height_, .001f, 1000.f, 90.f})},
      pScene         {&viewed},
      it             {viewed.getWorld().children.emplace(viewed.getWorld().children.end(), cameraOwner)},
      textureID      {0u},
      depthStencilID {0u},
      framebufferID  {0u},
      FBOIDtextureID {0u},
      FBOIDdepthID   {0u},
      FBOIDframebufferID{0u},
      FBOIDwidth     {static_cast<int>(ceilf(width_ * INV_DOWN_SAMPLING_COEF))},
      FBOIDheight    {static_cast<int>(ceilf(height_ * INV_DOWN_SAMPLING_COEF))},
      width          {width_},
      height         {height_},
      m_captureInputs{false}
{
    Engine::getInstance()->resourceManager.add<Shader>("gameObjectIdentifier",
                                                       "./resources/shaders/vGameObjectIdentifier.vs",
                                                       "./resources/shaders/fGameObjectIdentifier.fs");
    initializeFramebuffer();
    initializePickingFBO();

    freeFly.enableFixedUpdate(m_captureInputs);
    freeFly.setActive(m_captureInputs);
}

SceneViewer::~SceneViewer()
{
    //cameraOwner.destroyUniqueComponentNow<Camera>();
    //cameraOwner.destroyUniqueComponentNow<FreeFly>();
    pScene->getWorld().children.erase(it);

    glDeleteFramebuffers(1, &framebufferID);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &depthStencilID);

    glDeleteFramebuffers(1, &FBOIDframebufferID);
    glDeleteTextures(1, &FBOIDtextureID);
    glDeleteRenderbuffers(1, &FBOIDdepthID);
}


unsigned int SceneViewer::getHoveredGameObjectID() const
{
    // Render the picking texture in the identifier FBO
    Shader& shaderGameObjectIdentifier = *Engine::getInstance()->resourceManager.get<Shader>("gameObjectIdentifier");

    glUseProgram(shaderGameObjectIdentifier.getID());
    glViewport(0, 0, FBOIDwidth, FBOIDheight);

    glBindFramebuffer(GL_FRAMEBUFFER, FBOIDframebufferID);

    SceneRenderSystem renderSys{pScene->sceneRenderer};
    renderSys.draw(Engine::getInstance()->resourceManager, renderSys.gameObjectIdentifierPipeline());

    // Find the hovered game object, if any
    unsigned int pixel = 0u;
    int x, y;

    { // Find the coordinates of the pixel to read
        const ImVec2 currentScreenStart = ImGui::GetCursorScreenPos();
        const ImVec2 cursPos            = ImGui::GetMousePos();
        const ImVec2 cursorRelativePos   {ceilf((cursPos.x - currentScreenStart.x)),
                                          ceilf((cursPos.y - currentScreenStart.y))};

        x = static_cast<GLint>(cursorRelativePos.x * INV_DOWN_SAMPLING_COEF);
        y = static_cast<GLint>((static_cast<float>(height) - cursorRelativePos.y) * INV_DOWN_SAMPLING_COEF);
    }
    
    glReadPixels(x, y, 1u, 1u, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

    return pixel;
}


void SceneViewer::resize(int width_, int height_)
{
    if (width == width_ && height == height_)
    {
        return;
    }

    // ==== Update screen's texture framebuffer ====
    width  = width_;
    height = height_;

    // Resize texture and depth-stencil buffers
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);

    camera.setAspect(width / (float)height);


    // ==== Update selection framebuffer ====
    // Low sampling (we don't need 4K texture to select element)
    FBOIDwidth  = static_cast<int>(ceilf(width_  * INV_DOWN_SAMPLING_COEF));
    FBOIDheight = static_cast<int>(ceilf(height_ * INV_DOWN_SAMPLING_COEF));

    // Resize texture and depth buffers
    glBindTexture(GL_TEXTURE_2D, FBOIDtextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, FBOIDwidth, FBOIDheight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, FBOIDdepthID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FBOIDwidth, FBOIDheight);
}

void SceneViewer::bindScene(Scene& scene)
{
    if (pScene == &scene)
    {
        return;
    }

    { // Move cameraOwner to the other scene
        // Transfer ownership of &cameraOwner to the new scene
        using iterator = GameObject::Children::iterator;
        const iterator newIt = scene.getWorld().children.emplace(scene.getWorld().children.end(), cameraOwner);

        // Update the previous scene and the iterator to cameraOwner's parent's children list
        if (pScene != nullptr)
            pScene->getWorld().children.erase(it);

        it = newIt;
    }

    // Update the Camera component and cameraOwner scene and parent
    // 
    // SERIALIZATION CRASH : TODO : use setActive(false) when done, 
    //to remove camera from the old scene without adding it to a new scene
    camera.moveTowardScene(scene); 
    cameraOwner->setParent(scene.getWorld());
    cameraOwner->pOwnerScene = &scene;
    pScene = &scene;
}

void SceneViewer::unbindScene()
{
    cameraOwner->detach(it);
    pScene = nullptr;
}

void SceneViewer::render() const
{
    camera.updateView();
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, width, height);

    pScene->sceneRenderer.draw(Engine::getInstance()->resourceManager, pScene->sceneRenderer.defaultRenderPipeline());
}

void SceneViewer::captureInputs(bool shouldCapture)
{
    if (m_captureInputs == shouldCapture)
        return;

    m_captureInputs = shouldCapture;
    
    freeFly.enableUpdate(shouldCapture);
    freeFly.setActive(shouldCapture);
}

} // namespace GPE
