#include "Engine/ECS/Component/Camera.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/ECS/System/SceneRenderSystem.hpp"
#include "Engine/Resources/Scene.hpp"
#include "GPM/Transform.hpp"
#include "GPM/Vector3.hpp"

using namespace GPM;

namespace GPE
{

void Camera::updateProjection()
{
    switch (m_projInfo.type)
    {
    case EProjectionType::ORTHOGRAPHIC:
        m_projection =
            Transform::orthographic(m_projInfo.hSide * .5f, m_projInfo.vSide * .5f, m_projInfo.znear, m_projInfo.zfar);
        break;

    case EProjectionType::PERSPECTIVE:
        m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect, m_projInfo.znear, m_projInfo.zfar);
        break;

    default:
        FUNCT_WARNING("Other projection not implemented");
        break;
    }
}

void Camera::moveTowardScene(class Scene& newOwner)
{
    getOwner().pOwnerScene->sceneRenderer.removeCamera(this);
    newOwner.sceneRenderer.addCamera(this);
}

Camera::Camera(GameObject& owner, const PerspectiveCreateArg& arg) noexcept : Component(owner)
{
    GPE_ASSERT(arg.nearVal > 0.f, "Near must be greater than 0");

    m_projInfo.name   = arg.name;
    m_projInfo.type   = EProjectionType::PERSPECTIVE;
    m_projInfo.aspect = arg.aspect;
    m_projInfo.znear  = arg.nearVal;
    m_projInfo.zfar   = arg.farVal;
    m_projInfo.fovY   = arg.fovY * PI / 180.f;
    m_projInfo.fovX   = arg.aspect * m_projInfo.fovY;
    m_projInfo.hSide  = arg.farVal * tanf(m_projInfo.fovX * .5f) * 2.f;
    m_projInfo.vSide  = arg.farVal * tanf(m_projInfo.fovY * .5f) * 2.f;

    m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect, m_projInfo.znear, m_projInfo.zfar);

    getOwner().pOwnerScene->sceneRenderer.addCamera(this);

    Log::getInstance()->log((std::string("Perspective projection added with name \"") + arg.name + "\"").c_str());
}

Camera::Camera(GameObject& owner, const OrthographicCreateArg& arg) noexcept : Component(owner)
{
    GPE_ASSERT(arg.nearVal > 0.f, "Near must be greater than 0");

    m_projInfo.name   = arg.name;
    m_projInfo.type   = EProjectionType::ORTHOGRAPHIC;
    m_projInfo.aspect = arg.hSide / arg.vSide;
    m_projInfo.znear  = arg.nearVal;
    m_projInfo.zfar   = arg.farVal;
    m_projInfo.fovY   = atanf((arg.vSide * .5f) / arg.farVal) * 360.f / PI;
    m_projInfo.fovX   = atanf((arg.hSide * .5f) / arg.farVal) * 360.f / PI;
    m_projInfo.hSide  = arg.hSide;
    m_projInfo.vSide  = arg.vSide;

    m_projection =
        Transform::orthographic(m_projInfo.hSide * .5f, m_projInfo.vSide * .5f, m_projInfo.znear, m_projInfo.zfar);

    getOwner().pOwnerScene->sceneRenderer.addCamera(this);
    Log::getInstance()->log((std::string("Orthographic projection add with name \"") + arg.name + "\"").c_str());
}

Camera::~Camera() noexcept
{
    getOwner().pOwnerScene->sceneRenderer.removeCamera(this);
    DataChunk<Camera>::getInstance()->destroy(this);
}

Camera& Camera::operator=(Camera&& other) noexcept
{
    m_projInfo             = std::move(other.m_projInfo);
    m_projection           = std::move(other.m_projection);
    m_viewMatrix           = std::move(other.m_viewMatrix);
    m_viewProjectionMatrix = std::move(other.m_viewProjectionMatrix);

    getOwner().pOwnerScene->sceneRenderer.updateCameraPointer(this, &other);

    return static_cast<Camera&>(Component::operator=(std::move(other)));
}

void Camera::setFovY(const float fovY) noexcept
{
    m_projInfo.fovY  = fovY * PI / 180.f;
    m_projInfo.fovX  = m_projInfo.aspect * m_projInfo.fovY;
    m_projInfo.hSide = m_projInfo.zfar * tanf(m_projInfo.fovX * .5f) * 2.f;
    m_projInfo.vSide = m_projInfo.zfar * tanf(m_projInfo.fovY * .5f) * 2.f;

    updateProjection();
}

void Camera::setAspect(const float newAspect) noexcept
{
    m_projInfo.aspect = newAspect;
    m_projInfo.fovX   = m_projInfo.aspect * m_projInfo.fovY;
    m_projInfo.hSide  = m_projInfo.zfar * tanf(m_projInfo.fovX * .5f) * 2.f;

    updateProjection();
}

Frustum Camera::getFrustum() const noexcept
{
    // TODO: Optimization with furstum matrix ??
    Frustum     frustum;
    const Vec3& forward        = -getOwner().getTransform().getVectorForward();
    const Vec3& right          = getOwner().getTransform().getVectorRight();
    const Vec3& up             = getOwner().getTransform().getVectorUp();
    const Vec3& globalPosition = -getOwner().getTransform().getGlobalPosition();
    const float halfHSide      = m_projInfo.hSide * .5f;
    const float halfVSide      = m_projInfo.vSide * .5f;

    frustum.nearFace   = {globalPosition + m_projInfo.znear * -forward, forward};
    frustum.farFace    = {globalPosition + m_projInfo.zfar * -forward, -forward};
    frustum.rightFace  = {globalPosition, -Vec3::cross(forward * m_projInfo.zfar + right * halfHSide, up)};
    frustum.leftFace   = {globalPosition, Vec3::cross(forward * m_projInfo.zfar + right * -halfHSide, up)};
    frustum.topFace    = {globalPosition, -Vec3::cross(forward * m_projInfo.zfar + up * -halfVSide, right)};
    frustum.bottomFace = {globalPosition, Vec3::cross(forward * m_projInfo.zfar + up * halfVSide, right)};

    return frustum;
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

    d = -m_projInfo.znear;

    frustum.nearFace = {(playerDist + d) * test, norm.normalized()};

    // Far clipping plane
    norm.x = m[0][3] - m[0][2];
    norm.y = m[1][3] - m[1][2];
    norm.z = m[2][3] - m[2][2];
    // d      = m[3][3] - m[3][2];
    d = m_projInfo.zfar;

    frustum.farFace = {(playerDist + d) * test, norm.normalized()};

    return frustum;
}*/

} // End of namespace GPE
