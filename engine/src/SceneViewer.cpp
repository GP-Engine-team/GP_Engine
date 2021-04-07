#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

#include "Engine/Core/Debug/Log.hpp"
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/Script/FreeFly.hpp"

#include <GLFW/glfw3.h>
#include <map>
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

        GPE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                   "An error occured during this framebuffer's generation");

        glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    }
}

SceneViewer::SceneViewer(GPE::Scene& viewed, int width_, int height_)
    : pScene{&viewed}, cameraOwner{viewed, {"Editor_camera_" + std::to_string((size_t)this), {}, nullptr}},
      textureID{0u}, depthStencilID{0u}, framebufferID{0u}, width{width_}, height{height_}, m_captureInputs{false}
{
    Engine::getInstance()->resourceManager.add<Shader>("gameObjectIdentifier",
                                                       "./resources/shaders/vGameObjectIdentifier.vs",
                                                       "./resources/shaders/fGameObjectIdentifier.fs");
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
    pScene->sceneRenderer.draw(Engine::getInstance()->resourceManager, pScene->sceneRenderer.defaultRenderPipeline(),
                               framebufferID);

    std::cout << getIDOfSelectedGameObject() << std::endl;
}

unsigned int SceneViewer::getIDOfSelectedGameObject() const
{
    GLuint FBOIDtextureID;
    GLuint FBOIDdepthID;
    GLuint FBOIDframebufferID;

    // low sampling (we don't need 4K texture to select element)
    const int downScaleSampling = 4;
    const int FBOIDwidth        = width / 4;
    const int FBOIDheight       = height / 4;

    // Create FBO
    {
        // Initialize screen texture
        glGenTextures(1, &FBOIDtextureID);
        glBindTexture(GL_TEXTURE_2D, FBOIDtextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, FBOIDwidth, FBOIDheight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    { // Initialize depth-stencil buffer
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

        glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    }

    Shader& shaderGameObjectIdentifier = *Engine::getInstance()->resourceManager.get<Shader>("gameObjectIdentifier");

    glBindFramebuffer(GL_FRAMEBUFFER, FBOIDframebufferID);
    glUseProgram(shaderGameObjectIdentifier.getID());
    glViewport(0, 0, FBOIDwidth, FBOIDheight);

    // In capture input
    pScene->sceneRenderer.draw(
        Engine::getInstance()->resourceManager,
        [](const ResourceManagerType& rm, SceneRenderSystem& rs, std::vector<Renderer*>& pRenderers,
           std::vector<SubModel*>& pOpaqueSubModels, std::vector<SubModel*>& pTransparenteSubModels,
           std::vector<Camera*>& pCameras, std::vector<Light*>& pLights,
           std::vector<SceneRenderSystem::DebugShape>& debugShape, std::vector<SceneRenderSystem::DebugLine>& debugLine,
           unsigned int renderTextureID) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Frustum camFrustum = pCameras[0]->getFrustum();

            Shader& shaderGameObjectIdentifier =
                *Engine::getInstance()->resourceManager.get<Shader>("gameObjectIdentifier");

            /*Display opaque element*/
            {
                unsigned int id = 0;

                for (auto&& pSubModel : pOpaqueSubModels)
                {
                    if (!rs.isOnFrustum(camFrustum, pSubModel))
                        continue;

                    glUniform1ui(glGetUniformLocation(shaderGameObjectIdentifier.getID(), "id"), ++id);

                    rs.tryToBindMesh(pSubModel->pMesh->getID());
                    rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                    shaderGameObjectIdentifier.setMat4("projectViewModelMatrix",
                                                       (pCameras[0]->getProjectionView() *
                                                        pSubModel->pModel->getOwner().getTransform().getModelMatrix())
                                                           .e);
                    rs.drawModelPart(*pSubModel);
                }

                for (auto&& pSubModel : pTransparenteSubModels)
                {
                    if (!rs.isOnFrustum(camFrustum, pSubModel))
                        continue;

                    glUniform1ui(glGetUniformLocation(shaderGameObjectIdentifier.getID(), "id"), ++id);

                    rs.tryToBindMesh(pSubModel->pMesh->getID());
                    rs.tryToSetBackFaceCulling(pSubModel->enableBackFaceCulling);

                    shaderGameObjectIdentifier.setMat4("projectViewModelMatrix",
                                                       (pCameras[0]->getProjectionView() *
                                                        pSubModel->pModel->getOwner().getTransform().getModelMatrix())
                                                           .e);
                    rs.drawModelPart(*pSubModel);
                }
            }
            rs.resetCurrentRenderPassKey();
        },
        FBOIDframebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOIDframebufferID);
    unsigned int pixel = 0;

    ImVec2 currentScreenStart = ImGui::GetCursorScreenPos();
    ImVec2 cursPos            = ImGui::GetMousePos();

    glReadPixels((cursPos.x - currentScreenStart.x) / downScaleSampling,
                 (height - cursPos.y + currentScreenStart.y) / downScaleSampling, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT,
                 &pixel);

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

    // Delete FBO
    {
        glDeleteFramebuffers(1, &FBOIDframebufferID);
        glDeleteTextures(1, &FBOIDtextureID);
        glDeleteRenderbuffers(1, &FBOIDdepthID);
    }

    return pixel;
}

void SceneViewer::captureInputs()
{
    if (m_captureInputs)
    {
        return;
    }

    m_captureInputs = true;
    cameraOwner.addComponent<FreeFly>();
    // cameraOwner.addComponent<GPE::InputComponent>();
}

void SceneViewer::releaseInputs()
{
    if (!m_captureInputs)
    {
        return;
    }

    m_captureInputs = false;
    cameraOwner.destroyUniqueComponentNow<FreeFly>();
    // cameraOwner.destroyUniqueComponentNow<GPE::InputComponent>();
}

} // namespace GPE
