/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "GPM/Transform.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Conversion.hpp"
#include <vector>

namespace Engine::Intermediate
{
class TransformComponent : public Component
{
protected:
    
    GPM::Transform m_transform;
    GPM::SplitTransform m_spaceAttribut;
    bool m_isDirty = false;
    

public:
    TransformComponent(GameObject& refGameObject) noexcept : Component(refGameObject)
    {
    }

    TransformComponent() noexcept                                = delete;
    TransformComponent(const TransformComponent& other) noexcept = delete;
    TransformComponent(TransformComponent&& other) noexcept      = default;
    virtual ~TransformComponent() noexcept                       = default;
    TransformComponent& operator=(TransformComponent const& other) noexcept = delete;
    TransformComponent& operator=(TransformComponent&& other) noexcept = default;

    [[nodiscard]] constexpr inline bool isDirty() const
    {
        return m_isDirty;
    }

            /**
     * @brief update Mesh matrix
     *
     */
    void update() noexcept
    {
        if (!m_isDirty)
            return;

        m_transform.model = GPM::toTransform(m_spaceAttribut);
        m_isDirty   = false;
    }

    /**
     * @brief update transform if it depend to parent Mesh view (use in inherance in scene graph)
     *
     * @param parentMeshMatrix : Mesh view matrix of parent
     */
    void update(const GPM::Mat4& parentMeshMatrix) noexcept
    {
        m_transform.model = parentMeshMatrix * GPM::toTransform(m_spaceAttribut).model;
        m_isDirty = false;
    }

    inline GPM::Transform& get()
    {
        return m_transform;
    }

    inline
    const GPM::Mat4& getModelMatrix() const 
    {
        return m_transform.model;
    }
};
} /*namespace Engine::Intermediate*/
