/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <GPM/Vector3.hpp>
#include <GPM/Quaternion.hpp>

namespace GPE
{

class GameObject;
class FreeFly;
class Scene;
class Camera;
class InputComponent;

class SceneViewer
{
    // ==== Data members ====
public:
    GameObject*     cameraOwner = nullptr;
    FreeFly&        freeFly;
    Camera&         camera;
    InputComponent& inputs;
    Scene*          pScene = nullptr;
    unsigned int    textureID;

private:
    // TODO: factorize framebuffer and renderbuffer in classes
    unsigned int depthStencilID;
    unsigned int framebufferID;
    unsigned int FBOIDtextureID;
    unsigned int FBOIDdepthID;
    unsigned int FBOIDframebufferID;
    int          FBOIDwidth;
    int          FBOIDheight;

protected:
    // TODO: move to class Camera, or to a new class
    // Camera controller to look object
    bool      isTransitionActive = false;
    GPM::Vec3 startPos;
    GPM::Vec3 finalPos;

    GPM::Quat startRotation;
    GPM::Quat finalRotation;

    float lerpT              = 0.f; // coefficient
    float transitionDuration = 1.f; // in second
    float transitionRadius   = 10.f;

public:
    int width;
    int height;

private:
    bool m_capturingInputs;

    // ==== Methods ====
private:
    void initializeFramebuffer();
    void initializePickingFBO();
    void initializeInputs();

public:
    SceneViewer(GPE::Scene& viewed, int width = 1, int height = 1);
    ~SceneViewer();

    unsigned int getHoveredGameObjectID() const;

    void resize(int width, int height);
    void bindScene(Scene& scene);
    void unbindScene();
    void update();
    void render() const;
    void captureInputs(bool shouldCapture);
    bool capturingInputs() const;

    /**
     * @brief Allow user to move and turn toward indicate GO
     */
    void lookAtObject(const GameObject& GOToLook);
};

} // namespace GPE
