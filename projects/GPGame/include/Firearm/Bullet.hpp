/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

// NOTE: shouldn't the following headers be in Bullet.rfk.h?
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/InspectContext.hpp>
#include <Engine/Serialization/xml/xmlLoader.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>
// NOTE END
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/Serialize.hpp>

#include <Engine/Core/Tools/ClassUtility.hpp>

#include <Generated/Bullet.rfk.h>

namespace GPG RFKNamespace()
{
    class RFKClass(Inspect(), Serialize()) BaseBullet
    {
    public:
        virtual void triggered() = 0;

        BaseBullet_GENERATED
    };

    class RFKClass(Inspect(), Serialize()) Bullet : public BaseBullet
    {
    protected:
        RFKField(Serialize(), Inspect()) float m_dammage = 5.f;

    public:
        Bullet(float dammage) noexcept;
        Bullet() = default;

        void triggered() override;

        GETTER_SETTER_BY_VALUE(Dammage, m_dammage);

        Bullet_GENERATED
    };
} // namespace )