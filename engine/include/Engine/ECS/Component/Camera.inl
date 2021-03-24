#include "Engine/ECS/Component/Camera.hpp"

inline const Camera::ProjectionInfo& Camera::getProjectionInfo() const noexcept
{
    return m_projInfo;
}

inline const GPM::Mat4& Camera::getViewProjection() const noexcept
{
    return m_viewProjectionMatrix;
}

inline const GPM::Mat4& Camera::getView() const noexcept
{
    return m_viewMatrix;
}

inline const GPM::Mat4& Camera::getProjection() const noexcept
{
    return m_projection;
}