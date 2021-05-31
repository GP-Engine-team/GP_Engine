#include <Engine/ECS/Component/TransformComponent.hpp>

#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Serialization/Inspect.hpp>

#include <Generated/TransformComponent.rfk.h>

File_GENERATED

using namespace GPE;
using namespace GPM;

TransformComponent::TransformComponent(GameObject& refGameObject, const TransformComponent::CreateArg& arg) noexcept
    : Component(refGameObject),
      m_spaceAttribut{GPM::toQuaternion(GPM::Transform::rotation(arg.eulerRotation)), arg.position, arg.scale},
      m_transform{GPM::toTransform(m_spaceAttribut)}
{
    updateToSystem();
}

TransformComponent::~TransformComponent() noexcept
{
    setActive(false);
}

void TransformComponent::setVecForward(const Vec3& newForward, const Vec3& up) noexcept
{
    const GPM::Transform toGO{
        GPM::Transform::lookAt(m_spaceAttribut.position, m_spaceAttribut.position + newForward, up)};
    m_spaceAttribut.rotation = GPM::toQuaternion(toGO.rotation());

    m_isDirty = true;
}

void TransformComponent::setVecRight(const Vec3& newRight) noexcept
{
    m_transform.setVectorRight(newRight.normalized());
    m_transform.setVectorForward(m_transform.right().cross(m_transform.up()).normalized());
    m_transform.setVectorUp(m_transform.forward().cross(m_transform.right()).normalized());
    m_transform.model *= Transform::scaling(m_spaceAttribut.scale);
}

void TransformComponent::setVecUp(const Vec3& newUp) noexcept
{
    m_transform.setVectorUp(newUp.normalized());
    m_transform.setVectorForward(m_transform.right().cross(m_transform.up()).normalized());
    m_transform.setVectorRight(m_transform.forward().cross(m_transform.up()).normalized());
    m_transform.model *= Transform::scaling(m_spaceAttribut.scale);
}

void TransformComponent::inspect(GPE::InspectContext& context)
{
    if (GPE::DataInspector::inspect(context, m_spaceAttribut, "Transform"))
    {
        m_isDirty = true;
    }
}

void TransformComponent::onPostLoad()
{
    Component::onPostLoad();
    setDirty();
}

void TransformComponent::updateToSystem() noexcept
{
}