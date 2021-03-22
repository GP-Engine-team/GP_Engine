inline constexpr bool TransformComponent::isDirty() const
{
    return m_isDirty;
}

inline constexpr void
    TransformComponent::setDirty()
{
    m_isDirty = true;
}

inline constexpr GPM::Vec3 TransformComponent::getGlobalPosition() const noexcept
{
    return m_transform.translation();
}


inline GPM::Vec3 TransformComponent::getGlobalScale() const noexcept
{
    return m_transform.scaling();
}


inline GPM::Quaternion TransformComponent::getGlobalRotation() const noexcept
{
    return toQuaternion(m_transform.rotation());
}


inline void TransformComponent::update() noexcept
{
    if (!m_isDirty)
        return;

    m_transform = GPM::toTransform(m_spaceAttribut);
    m_isDirty   = false;
}


inline void TransformComponent::update(const GPM::Mat4& parentMeshMatrix) noexcept
{
    m_transform.model = parentMeshMatrix * GPM::toTransform(m_spaceAttribut).model;
    m_isDirty         = false;
}


inline void TransformComponent::translate(const GPM::Vec3& translation) noexcept
{
    m_spaceAttribut.position += translation;
    m_isDirty = true;
}


inline void TransformComponent::scale(const GPM::Vec3& scale) noexcept
{
    m_spaceAttribut.scale += scale;
    m_isDirty = true;
}


inline void TransformComponent::setTranslation(const GPM::Vec3& translation) noexcept
{
    m_spaceAttribut.position = translation;
    m_isDirty  = true;
}


inline void TransformComponent::setScale(const GPM::Vec3& scale) noexcept
{
    m_spaceAttribut.scale   = scale;
    m_isDirty = true;
}


inline void TransformComponent::setRotation(const GPM::Quaternion& q) noexcept
{
    m_spaceAttribut.rotation = q;
    m_isDirty = true;
}


inline void TransformComponent::setRotationZ(const float& z) noexcept
{
    m_spaceAttribut.rotation.z = z;
    m_isDirty = true;
}


inline GPM::Vec3 TransformComponent::getVectorForward() const noexcept
{
    return m_transform.forward().normalized();
}


inline GPM::Vec3 TransformComponent::getVectorRight() const noexcept
{
    return m_transform.right().normalized();
}


inline GPM::Vec3 TransformComponent::getVectorUp() const noexcept
{
    return m_transform.up().normalized();
}


inline constexpr GPM::SplitTransform& TransformComponent::getSpacialAttribut()
{
    return m_spaceAttribut;
}


inline constexpr GPM::Transform& TransformComponent::get()
{
    return m_transform;
}


inline constexpr const GPM::Mat4& TransformComponent::getModelMatrix() const
{
    return m_transform.model;
}


inline constexpr const GPM::Vec3& TransformComponent::getPosition() const noexcept
{
    return m_spaceAttribut.position;
}


inline constexpr const GPM::Quaternion& TransformComponent::getRotation() const noexcept
{
    return m_spaceAttribut.rotation;
}


inline constexpr const GPM::Vec3& TransformComponent::getScale() const noexcept
{
    return m_spaceAttribut.scale;
}