#include <Engine/Intermediate/Viewers/SceneViewer.hpp>
// Engine

#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/ECS/System/InputManagerGLFW.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Resources/Script/FreeFly.hpp>
#include <GPM/Vector3.hpp>

// Third-party
#include <glad/glad.h>

namespace GPE
{

#define INV_DOWN_SAMPLING_COEF .25f

// ========================== Private methods ==========================
// TODO (Sami): needs factorization --> WindowFramebuffer
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

// TODO (Sami): needs factorization --> WindowFramebuffer
void SceneViewer::initializePickingFBO()
{
    // low sampling (we don't need 4K texture to select element)
    FBOIDwidth  = static_cast<int>(ceilf(width * INV_DOWN_SAMPLING_COEF));
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

void SceneViewer::initializeInputs()
{
    inputs.bindAction("up", EKeyMode::KEY_DOWN, "Level editor", &freeFly, "up");
    inputs.bindAction("down", EKeyMode::KEY_DOWN, "Level editor", &freeFly, "down");
    inputs.bindAction("right", EKeyMode::KEY_DOWN, "Level editor", &freeFly, "right");
    inputs.bindAction("left", EKeyMode::KEY_DOWN, "Level editor", &freeFly, "left");
    inputs.bindAction("forward", EKeyMode::KEY_DOWN, "Level editor", &freeFly, "forward");
    inputs.bindAction("backward", EKeyMode::KEY_DOWN, "Level editor", &freeFly, "backward");
    inputs.bindAction("sprint", EKeyMode::KEY_PRESSED, "Level editor", &freeFly, "sprint");
    inputs.bindAction("walk", EKeyMode::KEY_RELEASED, "Level editor", &freeFly, "walk");
}

// ========================== Public methods ==========================
SceneViewer::SceneViewer(GPE::Scene& viewed, int width_, int height_)
    : cameraOwner{new GameObject(viewed, {"Editor camera", {}, &viewed.getWorld()})},
      freeFly{cameraOwner->addComponent<FreeFly>()},
      camera{cameraOwner->addComponent<Camera>(
          Camera::PerspectiveCreateArg{"Editor camera", Camera::computeAspect(width_, height_), .001f, 1000.f, 90.f})},
      inputs{cameraOwner->addComponent<GPE::InputComponent>()}, pScene{&viewed}, textureID{0u}, depthStencilID{0u},
      framebufferID{0u}, FBOIDtextureID{0u}, FBOIDdepthID{0u}, FBOIDframebufferID{0u},
      FBOIDwidth{static_cast<int>(ceilf(width_ * INV_DOWN_SAMPLING_COEF))},
      FBOIDheight{static_cast<int>(ceilf(height_ * INV_DOWN_SAMPLING_COEF))}, width{width_}, height{height_},
      m_capturingInputs{false}
{
    Engine::getInstance()->resourceManager.add<Shader>("gameObjectIdentifier",
                                                       "./resources/shaders/vGameObjectIdentifier.vs",
                                                       "./resources/shaders/fGameObjectIdentifier.fs");
    initializeFramebuffer();
    initializePickingFBO();
    initializeInputs();

    // Update the Camera component and cameraOwner scene and parent
    camera.setActive(true);
    freeFly.setActive(false);
    // inputs.setActive(true);
}

SceneViewer::~SceneViewer()
{
    delete cameraOwner;

    glDeleteFramebuffers(1, &framebufferID);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &depthStencilID);

    glDeleteFramebuffers(1, &FBOIDframebufferID);
    glDeleteTextures(1, &FBOIDtextureID);
    glDeleteRenderbuffers(1, &FBOIDdepthID);
}

unsigned int SceneViewer::getHoveredGameObjectID() const
{
    // Set active view
    pScene->sceneRenderer.setMainCamera(&camera);

    { // Select the shader
        Shader& shaderGameObjectIdentifier =
            *Engine::getInstance()->resourceManager.get<Shader>("gameObjectIdentifier");
        glUseProgram(shaderGameObjectIdentifier.getID());
    }

    // Setup the viewport and bind the picking framebuffer
    glViewport(0, 0, FBOIDwidth, FBOIDheight);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOIDframebufferID);

    { // Render in the picking framebuffer
        RenderSystem& renderSys = pScene->sceneRenderer;
        renderSys.render(renderSys.mousePickingPipeline());
    }

    // Find the hovered object, if any
    GLuint pixel = 0u;
    GLint  x, y;

    { // Find the coordinates of the pixel to read
        const ImVec2 currentScreenStart = ImGui::GetCursorScreenPos();
        const ImVec2 cursPos            = ImGui::GetMousePos();
        const ImVec2 cursorRelativePos{ceilf((cursPos.x - currentScreenStart.x)),
                                       ceilf((cursPos.y - currentScreenStart.y))};

        x = GLint(cursorRelativePos.x * INV_DOWN_SAMPLING_COEF);
        y = GLint((float(height) - cursorRelativePos.y) * INV_DOWN_SAMPLING_COEF);
    }

    glReadPixels(x, y, 1u, 1u, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

    return static_cast<unsigned int>(pixel);
}

// TODO (Sami): needs factorization --> WindowFramebuffer
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

    // ==== Update picking framebuffer ====
    // Low sampling (we don't need a 4K texture to select an element)
    FBOIDwidth  = int(ceilf(width_ * INV_DOWN_SAMPLING_COEF));
    FBOIDheight = int(ceilf(height_ * INV_DOWN_SAMPLING_COEF));

    // Resize texture and depth buffers
    glBindTexture(GL_TEXTURE_2D, FBOIDtextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, FBOIDwidth, FBOIDheight, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, FBOIDdepthID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FBOIDwidth, FBOIDheight);

    // ==== Update the camera ==== // TODO: does not work
    camera.setAspect(Camera::computeAspect(width, height));
}

void SceneViewer::bindScene(Scene& scene)
{
    // Move cameraOwner to the other scene
    cameraOwner->forceSetParent(scene.getWorld());
    cameraOwner->pOwnerScene = &scene;

    // Update the Camera component and cameraOwner scene and parent
    // inputs.setActive(true);
    freeFly.setActive(true);
    camera.setActive(true);
    scene.sceneRenderer.setMainCamera(&camera);
    pScene = &scene;
}

void SceneViewer::unbindScene()
{
    // inputs.setActive(false);
    freeFly.setActive(false);
    camera.setActive(false);
    pScene = nullptr;
}

void SceneViewer::update()
{
    // Camera is not set as child of world. So we need to update it.
    cameraOwner->getTransform().update();

    // When the game is not launched, behaviours are not updated
    // Update FreeFly manually
    if (m_capturingInputs)
    {
        freeFly.update(GPE::Engine::getInstance()->timeSystem.getUnscaledDeltaTime());
    }

    // TODO: move to class Camera, or to a new class
    if (isTransitionActive)
    {
        lerpT += float(GPE::Engine::getInstance()->timeSystem.getUnscaledDeltaTime());

        if (lerpT / transitionDuration >= 1.f)
        {
            isTransitionActive = false;
            lerpT              = 1.f;
        }

        cameraOwner->getTransform().setTranslation(startPos.lerp(finalPos, lerpT));

        if (startRotation.angleWith(finalRotation) > .0f)
        {
            cameraOwner->getTransform().setRotation(startRotation.slerp(finalRotation, lerpT));
        }
    }
}

void SceneViewer::render() const
{
    pScene->sceneRenderer.setMainCamera(&camera);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, width, height);

    pScene->sceneRenderer.tryToResize(width, height);
    pScene->sceneRenderer.render(pScene->sceneRenderer.defaultRenderPipeline());
    pScene->sceneRenderer.render(pScene->sceneRenderer.debugRenderPipeline());
}

void SceneViewer::captureInputs(bool shouldCapture)
{
    if (m_capturingInputs == shouldCapture)
        return;

    m_capturingInputs = shouldCapture;
}

// TODO: move to class Camera, or to a new class
void SceneViewer::lookAtObject(const GameObject& GOToLook)
{
    using namespace GPM;

    const TransformComponent &cam{cameraOwner->getTransform()}, &target{GOToLook.getTransform()};

    // Set defaults
    startPos = finalPos = cameraOwner->getTransform().getGlobalPosition();
    startRotation = finalRotation = cameraOwner->getTransform().getGlobalRotation();

    { // Find the position which must be reached at the end of the transition
        const Vec3 pos0{cam.getGlobalPosition()};
        const Vec3 pos1 = [&]() -> const Vec3 {
            Vec3 flatForward{target.getGlobalPosition() - cam.getGlobalPosition()};
            flatForward.y = .0f;
            flatForward.safelyNormalize();

            return target.getGlobalPosition() - flatForward * transitionRadius;
        }();

        // Check whether a transition really is necessary
        if (pos0.isNotEqualTo(pos1))
        {
            startPos = pos0;
            finalPos = pos1;

            lerpT              = 0.f;
            isTransitionActive = true;
        }
    }

    // Find the orientation which must be reached at the end of the transition
    const f32 angle = [&]() -> const f32 {
        const Vec3 flatTargetPos{target.getGlobalPosition().x, .0f, target.getGlobalPosition().z};
        const Vec3 to{flatTargetPos - finalPos};

        // The camera looks toward negative z
        return (-Vec3::forward()).signedAngleWithUnitary(to.safelyNormalized());
    }();

    startRotation = cam.getGlobalRotation();
    finalRotation = Quat::angleAxis(angle, Vec3::up());

    lerpT              = 0.f;
    isTransitionActive = true;
}

} // namespace GPE
