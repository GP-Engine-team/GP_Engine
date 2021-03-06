﻿
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

void BehaviourSystem::addUpdateEditor(BehaviourComponent& fixedUpdateFunction) noexcept
{
    m_updateEditorFunctions.emplace_back(&fixedUpdateFunction);
}

void BehaviourSystem::removeUpdateEditor(BehaviourComponent& functionToRemove) noexcept
{
    for (auto&& function : m_updateEditorFunctions)
    {
        if (unlikely(function == &functionToRemove))
        {
            std::swap(function, m_updateEditorFunctions.back());
            m_updateEditorFunctions.pop_back();
            return;
        }
    }
}

void BehaviourSystem::addBehaviour(BehaviourComponent& behaviour) noexcept
{
    m_pBehaviours.push_back(&behaviour);

    if (startOnBehaviourAdd)
        behaviour.start();
}

void BehaviourSystem::removeBehaviour(BehaviourComponent& behaviour) noexcept
{
    if (behaviour.isFixedUpdateEnable())
        removeFixedUpdate(behaviour);

    if (behaviour.isUpdateEnable())
        removeUpdate(behaviour);

    if (behaviour.isOnGUIEnable())
        removeOnGUI(behaviour);

    if (behaviour.isUpdateEditorEnable())
        removeUpdateEditor(behaviour);

    for (auto&& it = m_pBehaviours.begin(); it != m_pBehaviours.end(); ++it)
    {
        if ((*it) == &behaviour)
        {
            std::swap<BehaviourComponent*>(m_pBehaviours.back(), (*it));
            m_pBehaviours.pop_back();
            return;
        }
    }
}

void BehaviourSystem::start() noexcept
{
    startOnBehaviourAdd = true;

    // Use basic loop because user can emplace map into this loop
    for (unsigned int i = 0; i < m_pBehaviours.size(); ++i)
    {
        m_pBehaviours[i]->start();
    }
}

void BehaviourSystem::onGUI() const noexcept
{
    // Use basic loop because user can emplace map into this loop
    for (unsigned int i = 0; i < m_onGUIFunctions.size(); ++i)
    {
        m_onGUIFunctions[i]->onGUI();
    }
}

void BehaviourSystem::fixedUpdate(double deltaTime) const noexcept
{
    // Use basic loop because user can emplace map into this loop
    for (unsigned int i = 0; i < m_fixedUpdateFunctions.size(); ++i)
    {
        m_fixedUpdateFunctions[i]->fixedUpdate(deltaTime);
    }
}

void BehaviourSystem::updateEditor(double deltaTime) const noexcept
{
    // Use basic loop because user can emplace map into this loop
    for (unsigned int i = 0; i < m_updateEditorFunctions.size(); ++i)
    {
        m_updateEditorFunctions[i]->updateEditor(deltaTime);
    }
}

void BehaviourSystem::update(double deltaTime) const noexcept
{
    // Use basic loop because user can emplace map into this loop
    for (unsigned int i = 0; i < m_updateFunctions.size(); ++i)
    {
        m_updateFunctions[i]->update(deltaTime);
    }
}

void BehaviourSystem::gameAssert(bool condition, const char* msg)
{
    if (!condition && onGameAssert)
        onGameAssert(msg);
}