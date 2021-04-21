/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{

class FreeFly;
class Scene;
class Camera;

class SceneViewer
{
// ==== Data members ====
public:
    GameObject*    cameraOwner = nullptr;
    FreeFly&      freeFly;
    Camera&       camera;
    Scene*        pScene;

    // Keep track of cameraOwner, from the perspective of
    // its parent's list of child
    GameObject::Children::iterator it;

    unsigned int  textureID;

private:
    // TODO: factorize framebuffer and renderbuffer in classes
    unsigned int  depthStencilID;
    unsigned int  framebufferID;
	unsigned int  FBOIDtextureID;
    unsigned int  FBOIDdepthID;
    unsigned int  FBOIDframebufferID;
	int		      FBOIDwidth;
	int		      FBOIDheight;

public:
    int           width;
    int           height;

private:
    bool          m_captureInputs;
    
// ==== Methods ====
private:
    void initializeFramebuffer();
    void initializePickingFBO ();

public:
    SceneViewer(GPE::Scene& viewed, int width = 1, int height = 1);
    ~SceneViewer();

    unsigned int getHoveredGameObjectID()                       const;

    void         resize                (int width, int height);
    void         bindScene             (Scene& scene);
    void         unbindScene           ();
    void         render                ()                       const;
    void         captureInputs         (bool shouldCapture);
};

} // namespace GPE
