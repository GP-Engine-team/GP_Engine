#include "Engine/Resources/Camera.hpp"

#include <iostream>

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"
#include "GPM/Transform.hpp"

using namespace Engine::Resources;
using namespace GPM;
using namespace Engine::Core::Debug;
using namespace Engine::Intermediate;

Camera::Camera(GameObject& owner, const CameraPerspectiveCreateArg& arg) : Component(owner)
{
    GPE_ASSERT(arg.near > 0.f, "Near must be greater than 0");

    ProjectionInfo info;

    info.name   = arg.name;
    info.type   = EProjectionType::PERSPECTIVE;
    info.aspect = arg.aspect;
    info.near   = arg.near;
    info.far    = arg.far;
    info.fovY   = arg.fovY;
    info.fovX   = arg.aspect * arg.fovY;
    info.hSide  = arg.far * sinf(info.fovX / 2.f);
    info.vSide  = arg.far * sinf(info.fovY / 2.f);

    m_projInfo   = info;
    m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect, m_projInfo.near, m_projInfo.far);

    RenderSystem::getInstance()->addCamera(this);

    Log::log((std::string("Perspective projection add with name \"") + arg.name + "\"").c_str());
}

Camera::Camera(GameObject& owner, const CameraOrthographicCreateArg& arg) : Component(owner)
{
    GPE_ASSERT(arg.nearVal > 0.f, "Near must be greater than 0");

    ProjectionInfo info;

    info.name   = arg.name;
    info.type   = EProjectionType::ORTHOGRAPHIC;
    info.aspect = arg.hSide / arg.vSide;
    info.near   = arg.nearVal;
    info.far    = arg.farVal;
    info.fovY   = arg.farVal * asinf(arg.vSide);
    info.fovX   = arg.farVal * asinf(arg.hSide);
    info.hSide  = arg.hSide;
    info.vSide  = arg.vSide;

    m_projInfo = info;
    m_projection =
        Transform::orthographic(m_projInfo.hSide / 2.f, m_projInfo.vSide / 2.f, m_projInfo.near, m_projInfo.far);

    RenderSystem::getInstance()->addCamera(this);
    Log::log((std::string("Orthographic projection add with name \"") + arg.name + "\"").c_str());
}

void Camera::setFovY(const float fovY) noexcept
{
    m_projInfo.fovY  = fovY;
    m_projInfo.fovX  = m_projInfo.aspect * fovY;
    m_projInfo.hSide = m_projInfo.far * sinf(m_projInfo.fovX);
    m_projInfo.vSide = m_projInfo.far * sinf(m_projInfo.fovY);

    switch (m_projInfo.type)
    {
    case EProjectionType::ORTHOGRAPHIC:
        m_projection =
            Transform::orthographic(m_projInfo.hSide / 2.f, m_projInfo.vSide / 2.f, m_projInfo.near, m_projInfo.far);
        break;

    case EProjectionType::PERSPECTIVE:
        m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect, m_projInfo.near, m_projInfo.far);
        break;

    default:
        FUNCT_WARNING("Other projection not implemented");
        break;
    }
}

void Camera::setAspect(const float newAspect) noexcept
{
    m_projInfo.aspect = newAspect;
    m_projInfo.fovX   = m_projInfo.aspect * m_projInfo.fovY;
    m_projInfo.hSide  = m_projInfo.far * sinf(m_projInfo.fovX);

    switch (m_projInfo.type)
    {
    case EProjectionType::ORTHOGRAPHIC:
        m_projection =
            Transform::orthographic(m_projInfo.hSide / 2.f, m_projInfo.vSide / 2.f, m_projInfo.near, m_projInfo.far);
        break;

    case EProjectionType::PERSPECTIVE:
        m_projection = Transform::perspective(m_projInfo.fovY, m_projInfo.aspect, m_projInfo.near, m_projInfo.far);
        break;

    default:
        FUNCT_WARNING("Other projection not implemented");
        break;
    }
}

GPM::Mat4 Camera::getView() const noexcept
{
    return m_gameObject->getTransform().getModelMatrix().inversed();
}