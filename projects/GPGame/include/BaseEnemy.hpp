/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <BaseCharacter.hpp>

#include <Generated/BaseEnemy.rfk.h>

namespace GPE
{
class GameObject;
}

namespace GPG RFKNamespace()
{
    class Firearm;

    class RFKClass(Inspect(), ComponentGen, Serialize()) BaseEnemy : public BaseCharacter
    {
    protected:
        GPE::GameObject* m_target;

    public:
        BaseEnemy() noexcept = default;
        BaseEnemy(GPE::GameObject & owner) noexcept;
        BaseEnemy(const BaseEnemy& other) noexcept = delete;
        BaseEnemy(BaseEnemy && other) noexcept     = delete;
        virtual ~BaseEnemy() noexcept              = default;

        BaseEnemy& operator=(BaseEnemy const& other) noexcept = delete;
        BaseEnemy& operator=(BaseEnemy&& other) noexcept = delete;

    public:
        void start() final;
        void update(double deltaTime) final;
        void onPostLoad() final;

        BaseEnemy_GENERATED
    };

} // namespace )
