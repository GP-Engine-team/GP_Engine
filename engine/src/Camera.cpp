#include <Engine/Core/Debug/Assert.hpp>
#include <Engine/Core/Debug/Log.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <GPM/Transform.hpp>
#include <GPM/Vector3.hpp>

#include <Engine/ECS/Component/Camera.hpp>
File_GENERATED

    using namespace GPM;
namespace GPE
{

// Static method
float Camera::computeAspect(int width, int height) noexcept
{
    return width / float(height);
}

void Camera::updateView()
{
    m_viewMatrix           = getOwner().getTransform().getModelMatrix().inversed();
    m_projectionViewMatrix = m_projection * m_viewMatrix;
}

void Camera::updateProjection()
{
    switch (type)
    {
    case EProjectionType::ORTHOGRAPHIC:
        m_projection = Transform::orthographic(hSide * .5f, -hSide * .5f, vSide * .5f, -vSide * .5f, znear, zfar);
        break;

    case EProjectionType::PERSPECTIVE:
        m_projection = Transform::perspective(fovY, aspect, znear, zfar);
        break;

    default:
        FUNCT_WARNING("Other projection not implemented");
        break;
    }
}

void Camera::moveTowardScene(Scene& newOwner)
{
    if (getOwner().pOwnerScene)
        getOwner().pOwnerScene->sceneRenderer.removeCamera(*this);
    newOwner.sceneRenderer.addCamera(*this);
}

Camera::Camera(GameObject& owner) noexcept : Camera(owner, PerspectiveCreateArg{})
{
    updateToSystem();
}

Camera::Camera() noexcept
{
    if (type == EProjectionType::PERSPECTIVE)
    {
        fovX  = aspect * fovY;
        hSide = zfar * tanf(fovX * .5f) * 2.f;
        vSide = zfar * tanf(fovY * .5f) * 2.f;
        Log::getInstance()->log((std::string("Perspective projection added with name \"") + name + "\"").c_str());
    }
    else
    {
        fovY = atanf((vSide * .5f) / zfar) * 180 / PI;
        fovX = atanf((hSide * .5f) / zfar) * 180.f / PI;

        Log::getInstance()->log((std::string("Orthographic projection add with name \"") + name + "\"").c_str());
    }
}

Camera::Camera(GameObject& owner, const PerspectiveCreateArg& arg) noexcept : Component(owner)
{
    GPE_ASSERT(arg.nearVal > 0.f, "Near must be greater than 0");

    name   = arg.name;
    type   = EProjectionType::PERSPECTIVE;
    aspect = arg.aspect;
    znear  = arg.nearVal;
    zfar   = arg.farVal;
    fovY   = arg.fovY * PI / 180.f;
    fovX   = arg.aspect * fovY;
    hSide  = arg.farVal * tanf(fovX * .5f) * 2.f;
    vSide  = arg.farVal * tanf(fovY * .5f) * 2.f;

    m_projection = Transform::perspective(fovY, aspect, znear, zfar);
    onPostLoad();

    Log::getInstance()->log((std::string("Perspective projection added with name \"") + arg.name + "\"").c_str());
}

Camera::Camera(GameObject& owner, const OrthographicCreateArg& arg) noexcept : Component(owner)
{
    GPE_ASSERT(arg.nearVal > 0.f, "Near must be greater than 0");

    name   = arg.name;
    type   = EProjectionType::ORTHOGRAPHIC;
    aspect = arg.hSide / arg.vSide;
    znear  = arg.nearVal;
    zfar   = arg.farVal;
    fovY   = atanf((arg.vSide * .5f) / arg.farVal) * 180 / PI;
    fovX   = atanf((arg.hSide * .5f) / arg.farVal) * 180.f / PI;
    hSide  = arg.hSide;
    vSide  = arg.vSide;

    m_projection = Transform::orthographic(hSide * .5f, -hSide * .5f, vSide * .5f, -vSide * .5f, znear, zfar);

    onPostLoad();

    Log::getInstance()->log((std::string("Orthographic projection add with name \"") + arg.name + "\"").c_str());
}

void Camera::onPostLoad()
{
    getOwner().getTransform().OnUpdate += GPE::Function::make(this, "updateView");
    if (type == EProjectionType::PERSPECTIVE)
    {
        hSide = zfar * tanf(fovX * .5f) * 2.f;
        vSide = zfar * tanf(fovY * .5f) * 2.f;
    }
    updateToSystem();
    updateView();
}

Camera::~Camera() noexcept
{
    getOwner().getTransform().OnUpdate -= GPE::Function::make(this, "updateView");
    setActive(false);
}

void Camera::updateFovY()
{
    fovX  = aspect * fovY;
    hSide = zfar * tanf(fovX * .5f) * 2.f;
    vSide = zfar * tanf(fovY * .5f) * 2.f;

    updateProjection();
    updateView();
}

void Camera::updateAspect()
{
    if (type != EProjectionType::ORTHOGRAPHIC)
    {
        fovX  = aspect * fovY;
        hSide = zfar * tanf(fovX * .5f) * 2.f;
    }

    updateProjection();
    updateView();
}

void Camera::setFovY(const float newfovY) noexcept
{
    fovY = newfovY * PI / 180.f;
    updateFovY();
}

void Camera::setAspect(const float newAspect) noexcept
{
    aspect = newAspect;
    updateAspect();
}

Frustum Camera::getFrustum() const noexcept
{
    // TODO: Optimization with furstum matrix ??
    Frustum     frustum;
    const Vec3& forward        = getOwner().getTransform().getVectorForward();
    const Vec3& right          = getOwner().getTransform().getVectorRight();
    const Vec3& up             = getOwner().getTransform().getVectorUp();
    const Vec3& globalPosition = -getOwner().getTransform().getGlobalPosition();
    const float halfHSide      = hSide * .5f;
    const float halfVSide      = vSide * .5f;

    frustum.nearFace   = {globalPosition + znear * -forward, forward};
    frustum.farFace    = {globalPosition + zfar * -forward, -forward};
    frustum.rightFace  = {globalPosition, -Vec3::cross(forward * zfar + right * halfHSide, up)};
    frustum.leftFace   = {globalPosition, Vec3::cross(forward * zfar + right * -halfHSide, up)};
    frustum.topFace    = {globalPosition, -Vec3::cross(forward * zfar + up * -halfVSide, right)};
    frustum.bottomFace = {globalPosition, Vec3::cross(forward * zfar + up * halfVSide, right)};

    return frustum;
}

void Camera::updateToSystem() noexcept
{
    if (m_isActivated)
        getOwner().pOwnerScene->sceneRenderer.addCamera(*this);
    else
    {
        if (getOwner().pOwnerScene)
            getOwner().pOwnerScene->sceneRenderer.removeCamera(*this);
    }
}

/*
Frustum Camera::getFrustum() const noexcept
{
    Frustum frustum;
    Vec3    norm;
    float   d;
    Mat4    m = m_projection * getView();

    float playerDist = 0.f;
    float test       = 1.f;

    // Left clipping plane
    norm.x = m[0][3] + m[0][0];
    norm.y = m[1][3] + m[1][0];
    norm.z = m[2][3] + m[2][0];
    d      = m[3][3] + m[3][0];

    // m_distance(p1.dot(m_normal))

    frustum.leftFace = {d, norm.normalized()};

    // Right clipping plane
    norm.x = m[0][3] - m[0][0];
    norm.y = m[1][3] - m[1][0];
    norm.z = m[2][3] - m[2][0];
    d      = m[3][3] - m[3][0];

    frustum.rightFace = {d, norm.normalized()};

    // Top clipping plane
    norm.x = m[0][3] - m[0][1];
    norm.y = m[1][3] - m[1][1];
    norm.z = m[2][3] - m[2][1];
    d      = m[3][3] - m[3][1];

    frustum.topFace = {(playerDist + d) * test, norm.normalized()};

    // Bottom clipping plane
    norm.x = m[0][3] + m[0][1];
    norm.y = m[1][3] + m[1][1];
    norm.z = m[2][3] + m[2][1];
    d      = m[3][3] + m[3][1];

    frustum.bottomFace = {(playerDist + d) * test, norm.normalized()};

    // Near clipping plane
    norm.x = m[0][3] + m[0][2];
    norm.y = m[1][3] + m[1][2];
    norm.z = m[2][3] + m[2][2];
    // d      = m[3][3] + m[3][2];

    d = -znear;

    frustum.nearFace = {(playerDist + d) * test, norm.normalized()};

    // Far clipping plane
    norm.x = m[0][3] - m[0][2];
    norm.y = m[1][3] - m[1][2];
    norm.z = m[2][3] - m[2][2];
    // d      = m[3][3] - m[3][2];
    d = zfar;

    frustum.farFace = {(playerDist + d) * test, norm.normalized()};

    return frustum;
}*/

} // End of namespace GPE
