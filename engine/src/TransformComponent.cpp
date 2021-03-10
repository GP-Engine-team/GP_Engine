#include "Engine/Intermediate/TransformComponent.hpp"
#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{

TransformComponent::TransformComponent(GameObject& refGameObject, const TransformComponent::CreateArg& arg) noexcept
    : Component(refGameObject),
      m_spaceAttribut{GPM::toQuaternion(GPM::Transform::rotation(arg.eulerRotation)),
                      arg.position, arg.scale},
      m_transform{GPM::toTransform(m_spaceAttribut)}
{}

} // End of namespace GPE