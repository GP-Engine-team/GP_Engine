/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <functional>
#include <vector>
#include "Engine/Core/Tools/BranchPrediction.hpp"

namespace GPE
{
class BehaviourComponent;


/**
 * @brief The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over. Thread safe singleton according with link below :
 * @see https://refactoring.guru/fr/design-patterns/singleton/cpp/example
 */
class BehaviourSystem
{
    /**
     * The Singleton's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
private:
    static BehaviourSystem* m_pInstance;

    protected:

    std::vector<BehaviourComponent*>  m_pBehaviours;
    std::vector<BehaviourComponent*>  m_updateFunctions;
    std::vector<BehaviourComponent*>  m_fixedUpdateFunctions;

protected:

    inline BehaviourSystem() noexcept = default;
    ~BehaviourSystem() noexcept       = default;

public:
    BehaviourSystem(const BehaviourSystem& other) noexcept = delete;
    BehaviourSystem(BehaviourSystem&& other) noexcept      = delete;
    BehaviourSystem& operator=(BehaviourSystem const& other) noexcept = delete;
    BehaviourSystem& operator=(BehaviourSystem&& other) noexcept = delete;

    void addUpdate(BehaviourComponent& updateFunction) noexcept;
    void addFixedUpdate(BehaviourComponent& fixedUpdateFunction) noexcept;
    
    void removeUpdate(BehaviourComponent& updateFunctionToRemove) noexcept;
    void removeFixedUpdate(BehaviourComponent& fixedUpdateFunctionToRemove) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addBehaviour(BehaviourComponent* pBehaviour) noexcept;

    void updateBehaviourPointer(BehaviourComponent * newPointerBehaviour, const BehaviourComponent* exPointerBehaviour) noexcept;

    void removeBehaviour(BehaviourComponent* pBehaviour) noexcept;

    void start() const noexcept;

    void fixedUpdate(float deltaTime) noexcept;

    void update(float deltaTime) const noexcept;

    /**
     * @brief This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     *
     * @param value
     * @return RenderSystem*
     */
    static BehaviourSystem* getInstance() noexcept
    {
        if (unlikely(m_pInstance == nullptr))
        {
            m_pInstance = new BehaviourSystem();
        }
        return m_pInstance;
    }
};

} /*namespace GPE*/
