#pragma once
#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/STDSave.hpp>
#include <Engine/Serialization/Serialize.hpp>
#include <Refureku/Refureku.h>
#include <memory>

// Generated
#include <Generated/RigidBodyBase.rfk.h>

namespace GPE RFKNamespace()
{
    enum class EShapeType : int
    {
        E_SPHERE = 0,
        E_BOX    = 1,
    };

    class RFKClass(Inspect(false), Serialize(false)) RigidBodyBase : rfk::Object
    {
    public:
        RigidBodyBase(GameObject & _owner) noexcept;

        RigidBodyBase() noexcept                           = default;
        RigidBodyBase(const RigidBodyBase& other) noexcept = delete;
        RigidBodyBase(RigidBodyBase && other) noexcept     = default;
        RigidBodyBase& operator=(RigidBodyBase const& other) noexcept = delete;
        RigidBodyBase& operator=(RigidBodyBase&& other) noexcept = delete;

        // virtual void updateTransform() = 0;
        virtual ~RigidBodyBase() noexcept = default;

    public:
        RFKField(Serialize()) GameObject*                    owner = nullptr;
        RFKField(Serialize(), Inspect("setType")) EShapeType type  = EShapeType::E_BOX;

        RFKField(Serialize(), Inspect()) std::unique_ptr<Collider> collider;

        virtual void updateShape(physx::PxShape & oldShape) = 0;

    protected:
        void setType(EShapeType & newType);

        RigidBodyBase_GENERATED
    };
} // namespace )

File_GENERATED