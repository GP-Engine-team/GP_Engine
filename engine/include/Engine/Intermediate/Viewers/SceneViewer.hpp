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
    GameObject    cameraOwner;
    FreeFly&      freeFly;
    Camera&       camera;
    Scene*        pScene;
    GameObject::Children::iterator it;

    unsigned int  textureID;

private:
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
    void         render                ()                       const;
    void         captureInputs         (bool shouldCapture);
};

} // namespace GPE
