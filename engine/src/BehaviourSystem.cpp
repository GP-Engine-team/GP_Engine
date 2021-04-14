
#include "Engine/ECS/System/BehaviourSystem.hpp"
#include "Engine/Core/Tools/BranchPrediction.hpp"
#include "Engine/ECS/Component/BehaviourComponent.hpp"

using namespace GPE;

void BehaviourSystem::addUpdate(BehaviourComponent& updateFunction) noexcept
{
    m_updateFunctions.emplace_back(&updateFunction);
}

void BehaviourSystem::addFixedUpdate(BehaviourComponent& fixedUpdateFunction) noexcept
{
    m_fixedUpdateFunctions.emplace_back(&fixedUpdateFunction);
}

void BehaviourSystem::removeUpdate(BehaviourComponent& updateFunctionToRemove) noexcept
{
    for (auto&& function : m_updateFunctions)
    {
        if (unlikely(function == &updateFunctionToRemove))
        {
            std::swap(function, m_updateFunctions.back());
            m_updateFunctions.pop_back();
            return;
        }
    }
}

void BehaviourSystem::removeFixedUpdate(BehaviourComponent& fixedUpdateFunctionToRemove) noexcept
{
    for (auto&& function : m_fixedUpdateFunctions)
    {
        if (unlikely(function == &fixedUpdateFunctionToRemove))
        {
            std::swap(function, m_fixedUpdateFunctions.back());
            m_fixedUpdateFunctions.pop_back();
            return;
        }
    }
}

void BehaviourSystem::addOnGUI(BehaviourComponent& fixedUpdateFunction) noexcept
{
    m_onGUIFunctions.emplace_back(&fixedUpdateFunction);
}

void BehaviourSystem::removeOnGUI(BehaviourComponent& functionToRemove) noexcept
{
    for (auto&& function : m_onGUIFunctions)
    {
        if (unlikely(function == &functionToRemove))
        {
            std::swap(function, m_onGUIFunctions.back());
            m_onGUIFunctions.pop_back();
            return;
        }
    }
}

void BehaviourSystem::addBehaviour(BehaviourComponent* pBehaviour) noexcept
{
    m_pBehaviours.push_back(pBehaviour);
}

void BehaviourSystem::updateBehaviourPointer(BehaviourComponent*       newPointorBehaviour,
                                             const BehaviourComponent* exPointorBehaviour) noexcept
{
    const std::vector<BehaviourComponent*>::iterator end = m_pBehaviours.end();
    for (std::vector<BehaviourComponent*>::iterator it = m_pBehaviours.begin(); it != end; ++it)
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
    const std::vector<BehaviourComponent*>::iterator end = m_pBehaviours.end();
    for (std::vector<BehaviourComponent*>::iterator it = m_pBehaviours.begin(); it != end; ++it)
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

void BehaviourSystem::onGUI() const noexcept
{
    for (auto&& behaviour : m_onGUIFunctions)
    {
        behaviour->onGUI();
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
