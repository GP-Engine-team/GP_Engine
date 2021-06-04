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
        std::unique_ptr<rapidxml::xml_document<>> m_pPrefabBluePrint = nullptr;
        std::unique_ptr<char[]>                   m_pData            = nullptr;
        std::string                               m_name             = "";

    protected:
        void reset()
        {
            m_pPrefabBluePrint.reset();
            m_pData.reset();
            m_name = "";
        }

    public:
        void loadPrefabFromPath(const char* path);

        Prefab(const char* path)
        {
            loadPrefabFromPath(path);
        }

        Prefab()  = default;
        ~Prefab() = default;

        GameObject* clone(GameObject & parent);

        const char* getName() const;

        Prefab_GENERATED
    };

    struct SharedPrefab
    {
        Prefab pref;
        size_t instanceCounter = 0;

        SharedPrefab(const char* path) : pref(path)
        {
        }
    };
} // namespace )