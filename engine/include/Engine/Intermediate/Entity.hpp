/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

#include "Engine/Intermediate/TransformComponent.hpp"

namespace Engine::Intermediate
{
typedef struct S_EntityCreateArg
{
    std::string name{""};
    // Engine::Physics::TransformCreateArg  transformArg  {Engine::Core::Maths::Vec3::zero,
    // Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::one};

} EntityCreateArg;

class Entity
{
protected:
    std::string m_name;
    TransformComponent m_transform;

public:
#pragma region constructor / destructor

    Entity()
        : // Transform   {Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::zero,
          // Engine::Core::Maths::Vec3::one},
          m_name("")
    {
    }

    Entity(const char* name)
        : // Transform   {Engine::Core::Maths::Vec3::zero, Engine::Core::Maths::Vec3::zero,
          // Engine::Core::Maths::Vec3::one},
          m_name{name}
    {
    }
    /*
                Entity (const Engine::Core::Maths::Vec3& position,
                        const Engine::Core::Maths::Vec3& rotation,
                        const Engine::Core::Maths::Vec3& scale,
                        const char* name)
                        :   Transform   {position, rotation, scale},
                            m_name       (name)
                        {}
     */
    Entity(const EntityCreateArg& arg)
        : // Transform   {arg.transformArg},
          m_name(arg.name)
    {
    }

    Entity(const Entity& other) = default;
    virtual ~Entity() = default;

#pragma endregion //! constructor/destructor

#pragma region methods

#pragma endregion //! methods

#pragma region accessor

    /**
     * @brief Get the Name object
     *
     * @return const char*
     */
    inline std::string getName() const noexcept
    {
        return m_name;
    }

#pragma endregion //! accessor

#pragma region mutator

    /**
     * @brief Set the Name object
     *
     * @param newName
     */
    inline void setName(const char* newName) noexcept
    {
        m_name = newName;
    }

#pragma endregion //! mutator
};
} // namespace Engine::Intermediate