#include "Engine/ECS/Component/Camera.hpp"

const Camera::ProjectionInfo& Camera::getProjectionInfo() const noexcept
{
    return m_projInfo;
}

const GPM::Mat4& Camera::getViewProjection() const noexcept
{
    return m_viewProjectionMatrix;
}

GPM::Mat4 Camera::getView() const noexcept // TODO: inversed matrix do not made each frame
{
    return m_gameObject.getTransform().getModelMatrix().inversed();
}

const GPM::Mat4& Camera::getProjection() const noexcept
{
    return m_projection;
}