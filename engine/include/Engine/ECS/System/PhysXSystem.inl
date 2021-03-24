#include <Engine/ECS/System/PhysXSystem.hpp>

size_t PhysXSystem::addComponent(RigidbodyStatic* rigidbody) noexcept
{
    rigidbodyStatics.push_back(rigidbody);
    scene->addActor(*rigidbody->rigidbody);

    return rigidbodyStatics.size();
}

void PhysXSystem::removeComponent(RigidbodyStatic* rigidbody) noexcept
{
    for (std::vector<RigidbodyStatic*>::iterator it = rigidbodyStatics.begin(); it != rigidbodyStatics.end(); it++)
    {
        if ((*it) == rigidbody)
        {
            std::swap<RigidbodyStatic*>(rigidbodyStatics.back(), (*it));
            rigidbodyStatics.pop_back();
            return;
        }
    }
}

size_t PhysXSystem::addComponent(RigidbodyDynamic* rigidbody) noexcept
{
    rigidbodyDynamics.push_back(rigidbody);
    scene->addActor(*rigidbody->rigidbody);

    return rigidbodyDynamics.size();
}

void PhysXSystem::removeComponent(RigidbodyDynamic* rigidbody) noexcept
{
    for (std::vector<RigidbodyDynamic*>::iterator it = rigidbodyDynamics.begin(); it != rigidbodyDynamics.end(); it++)
    {
        if ((*it) == rigidbody)
        {
            std::swap<RigidbodyDynamic*>(rigidbodyDynamics.back(), (*it));
            rigidbodyDynamics.pop_back();
            return;
        }
    }
}
