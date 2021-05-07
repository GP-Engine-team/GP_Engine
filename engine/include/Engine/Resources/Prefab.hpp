/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <memory>

#include <Engine/Serialization/Serialize.hpp>

#include "Generated/Prefab.rfk.h"

namespace GPE RFKNamespace()
{
    class GameObject;

    class RFKClass(Serialize()) Prefab
    {
    protected:
        std::unique_ptr<GameObject> m_pPrefab = nullptr;

    public:
        Prefab()
        {
        }

        ~Prefab() = default;

        void loadPrefabFromPath(const char* path);

        GameObject& clone(GameObject & parent);

        const GameObject* get() const
        {
            return m_pPrefab.get();
        }

        void reset()
        {
            m_pPrefab.reset();
        }

        Prefab_GENERATED
    };
} // namespace )