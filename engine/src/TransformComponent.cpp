#include "Engine/ECS/Component/TransformComponent.hpp"
#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{

TransformComponent::TransformComponent(GameObject& refGameObject, const TransformComponent::CreateArg& arg) noexcept
    : Component(refGameObject), m_spaceAttribut{GPM::toQuaternion(GPM::Transform::rotation(arg.eulerRotation)),
                                                arg.position, arg.scale},
      m_transform{GPM::toTransform(m_spaceAttribut)}
{
}

TransformComponent& TransformComponent::operator=(TransformComponent&& other)
{
    m_spaceAttribut = std::move(other.m_spaceAttribut);
    m_transform     = std::move(other.m_transform);
    m_isDirty       = std::move(other.m_isDirty);

    return static_cast<TransformComponent&>(Component::operator=(std::move(other)));
}

} // End of namespace GPE