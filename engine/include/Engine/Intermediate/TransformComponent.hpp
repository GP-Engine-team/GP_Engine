/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Component.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "GPM/Conversion.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Vector3.hpp"
#include "GPM/Vector4.hpp"
#include "GPM/Quaternion.hpp"
#include "GPM/Transform.hpp"
#include <vector>

namespace GPE
{
class TransformComponent : public Component
{
public:
    struct CreateArg
    {
        GPM::Vec3 position      = GPM::Vec3::zero();
        GPM::Vec3 eulerRotation = GPM::Vec3::zero();
        GPM::Vec3 scale         = GPM::Vec3::one();
    };

protected:
    GPM::SplitTransform m_spaceAttribut;
    GPM::Transform      m_transform = GPM::toTransform(m_spaceAttribut);
    bool                m_isDirty = false;

public:
    TransformComponent(GameObject& refGameObject, const CreateArg& arg = CreateArg{}) noexcept
        : Component(refGameObject), m_spaceAttribut{
                                                       GPM::toQuaternion(GPM::Transform::rotation(arg.eulerRotation)),
                                                       arg.position, arg.scale},
          m_transform{GPM::toTransform(m_spaceAttribut)}
    {}

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

    GPM::Vec3 getGlobalPosition() const noexcept
    {
        return m_transform.translation();
    }

    GPM::Vec3 getGlobalScale() const noexcept
    {
        return m_transform.scaling();
    }

    GPM::Quaternion getGlobalRotation() const noexcept
    {
        return toQuaternion(m_transform.rotation());
    }


    /**
     * @brief update Mesh matrix
     *
     */
    void update() noexcept
    {
        if (!m_isDirty)
            return;

        m_transform = GPM::toTransform(m_spaceAttribut);
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
        m_isDirty         = false;
    }

     void translate(const GPM::Vec3& translation) noexcept
    {
        m_spaceAttribut.position += translation;
        m_isDirty = true;
    }

     void scale(const GPM::Vec3& scale) noexcept
    {
        m_spaceAttribut.scale += scale;
        m_isDirty = true;
    }

     void setTranslation(const GPM::Vec3& translation) noexcept
    {
        m_spaceAttribut.position = translation;
        m_isDirty  = true;
    }

     void setScale(const GPM::Vec3& scale) noexcept
    {
        m_spaceAttribut.scale   = scale;
        m_isDirty = true;
    }

    GPM::Vec3 getVectorForward() const noexcept
    {
        return m_transform.forward().normalized();
    }
    GPM::Vec3 getVectorRight() const noexcept
    {
        return m_transform.right().normalized();
    }
    GPM::Vec3 getVectorUp() const noexcept
    {
        return m_transform.up().normalized();
    }

    inline GPM::SplitTransform& getSpacialAttribut()
    {
        return m_spaceAttribut;
    }

    inline GPM::Transform& get()
    {
        return m_transform;
    }

    inline const GPM::Mat4& getModelMatrix() const
    {
        return m_transform.model;
    }
};
} /*namespace GPE*/
