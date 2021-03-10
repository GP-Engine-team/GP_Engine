#include "Engine/Resources/Camera.hpp"

#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"
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
        m_projection = Transform::orthographic(m_projInfo.hSide * .5f, m_projInfo.vSide * .5f,
                                               m_projInfo.znear, m_projInfo.zfar);
        break;

    case EProjectionType::PERSPECTIVE:
        m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect,
                                              m_projInfo.znear, m_projInfo.zfar);
        break;

    default:
        FUNCT_WARNING("Other projection not implemented");
        break;
    }
}


Camera::Camera(GameObject& owner, const PerspectiveCreateArg& arg) noexcept
    : Component(owner)
{
    GPE_ASSERT(arg.znear > 0.f, "Near must be greater than 0");

    ProjectionInfo info;

    info.name   = arg.name;
    info.type   = EProjectionType::PERSPECTIVE;
    info.aspect = arg.aspect;
    info.znear   = arg.znear;
    info.zfar    = arg.zfar;
    info.fovY   = arg.fovY;
    info.fovX   = arg.aspect * arg.fovY;
    info.hSide  = arg.zfar * sinf(info.fovX * .5f);
    info.vSide  = arg.zfar * sinf(info.fovY * .5f);

    m_projInfo   = info;
    m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect,
                                          m_projInfo.znear, m_projInfo.zfar);

    RenderSystem::getInstance()->addCamera(this);

    Log::log((std::string("Perspective projection added with name \"") + arg.name + "\"").c_str());
}

Camera::Camera(GameObject& owner, const OrthographicCreateArg& arg) noexcept
    : Component(owner)
{
    GPE_ASSERT(arg.znear > 0.f, "Near must be greater than 0");

    ProjectionInfo info;

    info.name   = arg.name;
    info.type   = EProjectionType::ORTHOGRAPHIC;
    info.aspect = arg.hSide / arg.vSide;
    info.znear  = arg.znear;
    info.zfar   = arg.zfar;
    info.fovY   = arg.zfar * asinf(arg.vSide);
    info.fovX   = arg.zfar * asinf(arg.hSide);
    info.hSide  = arg.hSide;
    info.vSide  = arg.vSide;

    m_projInfo = info;
    m_projection = Transform::orthographic(m_projInfo.hSide * .5f, m_projInfo.vSide * .5f,
                                           m_projInfo.znear, m_projInfo.zfar);

    RenderSystem::getInstance()->addCamera(this);
    Log::log((std::string("Orthographic projection added with name \"") + arg.name + "\"").c_str());
}

void Camera::setFovY(const float fovY) noexcept
{
    m_projInfo.fovY  = fovY;
    m_projInfo.fovX  = m_projInfo.aspect * fovY;
    m_projInfo.hSide = m_projInfo.zfar * sinf(m_projInfo.fovX);
    m_projInfo.vSide = m_projInfo.zfar * sinf(m_projInfo.fovY);

    updateProjection();
}

void Camera::setAspect(const float newAspect) noexcept
{
    m_projInfo.aspect = newAspect;
    m_projInfo.fovX   = m_projInfo.aspect * m_projInfo.fovY;
    m_projInfo.hSide  = m_projInfo.zfar * sinf(m_projInfo.fovX);

    updateProjection();
}

Frustum Camera::getFrustum() const noexcept
{
    // TODO: Optimization with furstum matrix ??
    Frustum          frustum;
    const GPM::Vec3& forward        = -m_gameObject.getTransform().getVectorForward();
    const GPM::Vec3& right          = m_gameObject.getTransform().getVectorRight();
    const GPM::Vec3& up             = m_gameObject.getTransform().getVectorUp();
    const GPM::Vec3& globalPosition = m_gameObject.getTransform().getGlobalPosition();

    frustum.backFace  = {globalPosition + m_projInfo.znear * forward, forward};
    frustum.frontFace = {globalPosition + m_projInfo.zfar * forward, -forward};

    frustum.rightFace = {globalPosition,
                         GPM::Vec3::cross(forward * m_projInfo.zfar + right * (m_projInfo.hSide * .5f), up)};
    frustum.leftFace  = {globalPosition,
                        -GPM::Vec3::cross(forward * m_projInfo.zfar + right * (-m_projInfo.hSide * .5f), up)};

    frustum.topFace    = {globalPosition,
                          GPM::Vec3::cross(forward * m_projInfo.zfar + up * (m_projInfo.vSide * .5f), right)};
    frustum.bottomFace = {globalPosition,
                          -GPM::Vec3::cross(forward * m_projInfo.zfar + up * (-m_projInfo.vSide * .5f), right)};

    return frustum;
}

} // End of namespace GPE