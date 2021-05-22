/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/Tools/BranchPrediction.hpp"
#include <functional>
#include <vector>

namespace GPE
{
class BehaviourComponent;

class BehaviourSystem
{
protected:
    std::vector<BehaviourComponent*> m_pBehaviours;
    std::vector<BehaviourComponent*> m_updateFunctions;
    std::vector<BehaviourComponent*> m_fixedUpdateFunctions;
    std::vector<BehaviourComponent*> m_onGUIFunctions;

public:
    std::function<void(const char*)> onGameAssert;

public:
    void addUpdate(BehaviourComponent& updateFunction) noexcept;
    void addFixedUpdate(BehaviourComponent& fixedUpdateFunction) noexcept;
    void addOnGUI(BehaviourComponent& fixedUpdateFunction) noexcept;

    void removeUpdate(BehaviourComponent& updateFunctionToRemove) noexcept;
    void removeFixedUpdate(BehaviourComponent& fixedUpdateFunctionToRemove) noexcept;
    void removeOnGUI(BehaviourComponent& updateFunctionToRemove) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addBehaviour(BehaviourComponent& behaviour) noexcept;
    void removeBehaviour(BehaviourComponent& behaviour) noexcept;

    void start() const noexcept;
    void onGUI() const noexcept;

    void fixedUpdate(double deltaTime) noexcept;

    void update(double deltaTime) const noexcept;

    /**
     * @brief Call game assert event if condition == null
     * @param condition
     * @param msg
     */
    void gameAssert(bool condition, const char* msg = "");
};

} /*namespace GPE*/
