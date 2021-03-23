/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "GPM/Conversion.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Quaternion.hpp"
#include "GPM/Transform.hpp"
#include "GPM/Vector3.hpp"

namespace GPE
{

class GameObject;

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
    bool                m_isDirty   = false;

public:
    TransformComponent(GameObject& refGameObject, const CreateArg& arg = CreateArg{}) noexcept;

    TransformComponent() noexcept                                = delete;
    TransformComponent(const TransformComponent& other) noexcept = delete;
    TransformComponent(TransformComponent&& other) noexcept      = default;
    virtual ~TransformComponent() noexcept = default;
    TransformComponent& operator=(TransformComponent const& other) noexcept = delete;
    TransformComponent& operator                                            =(TransformComponent&& other);

    [[nodiscard]] constexpr inline bool isDirty() const;
    constexpr void                      setDirty();

    constexpr GPM::Vec3 getGlobalPosition() const noexcept;

    GPM::Vec3 getGlobalScale() const noexcept;

    GPM::Quaternion getGlobalRotation() const noexcept;

    /**
     * @brief update Mesh matrix
     *
     */
    void update() noexcept;

    /**
     * @brief update transform if it depend to parent Mesh view (use in inherance in scene graph)
     *
     * @param parentMeshMatrix : Mesh view matrix of parent
     */
    void update(const GPM::Mat4& parentMeshMatrix) noexcept;

    void translate(const GPM::Vec3& translation) noexcept;

    void scale(const GPM::Vec3& scale) noexcept;

    void setTranslation(const GPM::Vec3& translation) noexcept;

    void setScale(const GPM::Vec3& scale) noexcept;

    void setRotation(const GPM::Quaternion& q) noexcept;

    void setRotationZ(const float& z) noexcept;

    GPM::Vec3 getVectorForward() const noexcept;

    GPM::Vec3 getVectorRight() const noexcept;

    GPM::Vec3 getVectorUp() const noexcept;

    constexpr GPM::SplitTransform& getSpacialAttribut();

    constexpr GPM::Transform& get();

    constexpr const GPM::Mat4& getModelMatrix() const;

    constexpr const GPM::Vec3& getPosition() const noexcept;

    constexpr const GPM::Quaternion& getRotation() const noexcept;

    constexpr const GPM::Vec3& getScale() const noexcept;
};

#include "TransformComponent.inl"

} /*namespace GPE*/
