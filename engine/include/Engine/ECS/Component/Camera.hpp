/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

#include "Component.hpp"

#include <Engine/Serialization/ComponentGen.h>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/InspectContext.hpp>

#include <GPM/Matrix4.hpp>
#include <GPM/Shape3D/Plane.hpp>

// Generated
#include <Generated/Camera.rfk.h>

namespace GPE RFKNamespace()
{
    class GameObject;
    class TransformComponent;

    // TODO: Furstum must be inside Camera but is forwarding in file RenderSystem. While camera do not own it's frustum,
    // frustum struct must be outside
    struct Frustum
    {
        GPM::Plane topFace;
        GPM::Plane bottomFace;

        GPM::Plane rightFace;
        GPM::Plane leftFace;

        GPM::Plane farFace;
        GPM::Plane nearFace;
    };

    class RFKClass(Inspect(), ComponentGen, Serialize()) Camera : public Component
    {
    public:
        enum class EProjectionType
        {
            PERSPECTIVE,
            ORTHOGRAPHIC,
            NONE
        };

        struct RFKStruct(Inspect(), Serialize()) ProjectionInfo
        {
            RFKField(Inspect(), Serialize())
            std::string     name = "";
            RFKField(Serialize())
            EProjectionType type = EProjectionType::NONE;

            RFKField(Inspect(), Serialize())
            float aspect = 16.f / 9.f;
            RFKField(Inspect(), Serialize())
            float znear  = 0.001f;
            RFKField(Inspect(), Serialize())
            float zfar   = 10.f;
            RFKField(Inspect(), Serialize())
            float hSide  = 1.f;
            RFKField(Inspect(), Serialize())
            float vSide  = 1.f;
            RFKField(Inspect(), Serialize())
            float fovY   = 70.f;
            RFKField(Inspect(), Serialize())
            float fovX   = 70.f;

            ProjectionInfo_GENERATED
        };

        struct PerspectiveCreateArg
        {
            const char* name    = "Camera";
            float       aspect  = 16.f / 9.f;
            float       nearVal = 0.001f;
            float       farVal  = 1000.f;
            float       fovY    = 70.f;
        };

        struct OrthographicCreateArg
        {
            const char* name    = "Camera";
            float       hSide   = 1.f;
            float       vSide   = 1.f;
            float       nearVal = 0.001f;
            float       farVal  = 1000.f;
        };

    protected:
        RFKField(Inspect(), Serialize())
        ProjectionInfo m_projInfo;
        RFKField(Serialize())
        GPM::Mat4      m_projection;

        RFKField(Serialize())
        GPM::Mat4 m_viewMatrix;
        RFKField(Serialize())
        GPM::Mat4 m_projectionViewMatrix;

        void updateProjection();

    public:
        static float computeAspect(int width, int height) noexcept;

    public:
        Camera() noexcept = default;

        /**
         * @brief Default constructor. Call perspective constructor by default
         * @param owner
         * @return
         */
        Camera(GameObject& owner) noexcept;

        /**
         * @brief Construct a new perspective camera object
         *
         * @param position
         * @param rotation
         * @param aspect    : width/height
         * @param znear     : must be > 0
         * @param zfar     * @param fovY      : by default to 70 (human FovY)
         * @param name
         */
        Camera(GameObject& owner, const PerspectiveCreateArg& arg) noexcept;

        /**
         * @brief Construct a new orthographic camera object
         *
         * @param owner
         * @param arg
         */
        Camera(GameObject& owner, const OrthographicCreateArg& arg) noexcept;

        Camera(const Camera& other)            noexcept = delete;

        Camera(Camera&& other)                 noexcept = default;

        virtual ~Camera()                      noexcept;

        Camera& operator=(Camera&& other)      noexcept;

        Camera& operator=(Camera const& other) noexcept = delete;

        void moveTowardScene(class Scene & newOwner) override;

        /**
         * @brief Update the view matrix in function of model matrix of it's parent
         */
        RFKMethod() void updateView();

        /**
         * @brief Set the Fov Y object
         *
         * @param fovY
         */
        void setFovY(const float fovY) noexcept;

        void setAspect(const float newAspect) noexcept;

        /**
         * @brief Get the Projection Info in specifie index
         *
         * @return const ProjectionInfo&
         */
        inline const ProjectionInfo& getProjectionInfo() const noexcept;

        /**
         * @brief Get the view * projection matrix
         *
         * @return const GPM::Mat4&
         */
        inline const GPM::Mat4& getProjectionView() const noexcept;

        /**
         * @brief Get the view matrix
         *
         * @return const GPM::Mat4&
         */
        inline const GPM::Mat4& getView() const noexcept;

        /**
         * @brief Get the projection matrix
         *
         * @return const GPM::Mat4&
         */
        inline const GPM::Mat4& getProjection() const noexcept;

        /**
         * @brief Return struct with all plane that composed the camera frustum
         * @return
         */
        Frustum getFrustum() const noexcept;

        /**
         * @brief Add or remove current component from it's system which have for effect to enable or disable it
         * @param newState
         * @return
         */
        void setActive(bool newState) noexcept override;

        virtual void awake() override;

        Camera_GENERATED
    };
} // namespace )

#include "Camera.inl"