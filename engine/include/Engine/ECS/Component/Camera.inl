namespace GPE
{
inline const GPM::Mat4& Camera::getProjectionView() const noexcept
{
    return m_projectionViewMatrix;
}

inline const GPM::Mat4& Camera::getView() const noexcept
{
    return m_viewMatrix;
}

inline const GPM::Mat4& Camera::getProjection() const noexcept
{
    return m_projection;
}
} // namespace GPE