#include <Engine/ECS/Component/Physics/Collisions/SphereCollider.hpp>

using namespace GPE;
using namespace physx;

SphereCollider::SphereCollider(GameObject& owner) noexcept : CollisionComponent(owner), m_center(0, 0, 0), m_radius(1)
{
}
