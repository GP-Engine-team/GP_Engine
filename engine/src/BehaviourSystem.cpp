#include "Engine/Intermediate/BehaviourSystem.hpp"

#include "Engine/Core/Tools/BranchPrediction.hpp"
#include "Engine/Intermediate/BehaviourComponent.hpp"

using namespace GPE;

BehaviourSystem* BehaviourSystem::m_pInstance{nullptr};

void BehaviourSystem::addUpdate(BehaviourComponent* updateFunction) noexcept
{
    m_updateFunctions.emplace_back(updateFunction);
}

void BehaviourSystem::addFixedUpdate(BehaviourComponent* fixedUpdateFunction) noexcept
{
    m_updateFunctions.emplace_back(fixedUpdateFunction);
}

void BehaviourSystem::removeUpdate(BehaviourComponent* updateFunctionToRemove) noexcept
{
    for (auto&& function : m_updateFunctions)
    {
        if (unlikely(function == updateFunctionToRemove))
        {
            std::swap(function, m_updateFunctions.back());
            m_updateFunctions.pop_back();
        }
    }
}

void BehaviourSystem::removeFixedUpdate(BehaviourComponent* fixedUpdateFunctionToRemove) noexcept
{
    for (auto&& function : m_updateFunctions)
    {
        if (unlikely(function == fixedUpdateFunctionToRemove))
        {
            std::swap(function, m_updateFunctions.back());
            m_updateFunctions.pop_back();
        }
    }
}

void BehaviourSystem::addBehaviour(BehaviourComponent* pBehaviour) noexcept
{
    m_pBehaviours.push_back(pBehaviour);
    pBehaviour->awake();
}

void BehaviourSystem::updateBehaviourPointer(BehaviourComponent* newPointorBehaviour,
                                             BehaviourComponent* exPointorBehaviour) noexcept
{
    for (std::vector<BehaviourComponent*>::iterator it = m_pBehaviours.begin(); it != m_pBehaviours.end(); it++)
    {
        if ((*it) == exPointorBehaviour)
        {
            *it = newPointorBehaviour;
            return;
        }
    }
}

void BehaviourSystem::removeBehaviour(BehaviourComponent* pBehaviour) noexcept
{
    for (std::vector<BehaviourComponent*>::iterator it = m_pBehaviours.begin(); it != m_pBehaviours.end(); it++)
    {
        if ((*it) == pBehaviour)
        {
            std::swap<BehaviourComponent*>(m_pBehaviours.back(), (*it));
            m_pBehaviours.pop_back();
            return;
        }
    }
}

void BehaviourSystem::start() const noexcept
{
    for (auto&& behaviour : m_pBehaviours)
    {
        behaviour->start();
    }
}

void BehaviourSystem::fixedUpdate(float deltaTime) noexcept
{
    for (auto&& behaviour : m_fixedUpdateFunctions)
    {
        behaviour->fixedUpdate(deltaTime);
    }
}

void BehaviourSystem::update(float deltaTime) const noexcept
{
    for (auto&& behaviour : m_updateFunctions)
    {
        behaviour->update(deltaTime);
    }
}