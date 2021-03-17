/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
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

public:
    void addUpdate(BehaviourComponent& updateFunction) noexcept;
    void addFixedUpdate(BehaviourComponent& fixedUpdateFunction) noexcept;

    void removeUpdate(BehaviourComponent& updateFunctionToRemove) noexcept;
    void removeFixedUpdate(BehaviourComponent& fixedUpdateFunctionToRemove) noexcept;

    // TODO: Remove this shit and create variadic templated system
    void addBehaviour(BehaviourComponent* pBehaviour) noexcept;

    void updateBehaviourPointer(BehaviourComponent*       newPointerBehaviour,
                                const BehaviourComponent* exPointerBehaviour) noexcept;

    void removeBehaviour(BehaviourComponent* pBehaviour) noexcept;

    void start() const noexcept;

    void fixedUpdate(float deltaTime) noexcept;

    void update(float deltaTime) const noexcept;
};

} /*namespace GPE*/
