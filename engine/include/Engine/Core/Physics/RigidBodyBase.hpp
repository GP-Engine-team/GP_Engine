#pragma once
#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Refureku/Refureku.h>
#include <memory>

// Generated
#include <Generated/RigidBodyBase.rfk.h>

namespace GPE RFKNamespace()
{
    enum class EShapeType
    {
        E_SPHERE,
        E_BOX,
    };

    class RFKClass(Inspect(), Serialize()) RigidBodyBase
    {
    public:
        RigidBodyBase(EShapeType _type) noexcept;

        RigidBodyBase() noexcept                           = default;
        RigidBodyBase(const RigidBodyBase& other) noexcept = delete;
        RigidBodyBase(RigidBodyBase && other) noexcept     = default;
        RigidBodyBase& operator=(RigidBodyBase const& other) noexcept = delete;
        RigidBodyBase& operator=(RigidBodyBase&& other) noexcept = delete;

        // virtual void updateTransform() = 0;
        virtual ~RigidBodyBase() noexcept = default;

    public:
        EShapeType                type;
        std::unique_ptr<Collider> collider;

        RigidBodyBase_GENERATED
    };
} // namespace )