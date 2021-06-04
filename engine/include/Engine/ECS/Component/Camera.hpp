/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

#include "Component.hpp"

#include <Engine/Core/Tools/ClassUtility.hpp>
#include <Engine/Serialization/ComponentGen.h>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/InspectContext.hpp>

#include <GPM/Matrix4.hpp>
#include <GPM/Shape3D/Plane.hpp>
#include <gpm/constants.hpp>

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

    class RFKClass(Serialize(), ComponentGen) Camera : public Component
    {
    public:
        enum class EProjectionType
        {
            PERSPECTIVE,
            ORTHOGRAPHIC,
            NONE
        };

        struct ProjectionInfo
        {
            std::string     name = "";
            EProjectionType type = EProjectionType::PERSPECTIVE;

            float aspect = 16.f / 9.f;
            float znear  = 0.001f;
            float zfar   = 1000.f;
            float hSide  = 1.f;
            float vSide  = 1.f;
            float fovY   = 7.f * PI / 18.f; // 70deg
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

    public:
        struct RFKStruct(Serialize()) FogParameter
        {
            RFKField(Serialize()) RGB   color       = RGB::white();
            RFKField(Serialize()) float linearStart = 0.f;
            RFKField(Serialize()) float linearEnd   = 0.f;
            RFKField(Serialize()) float density     = 1.f;

            RFKField(Serialize()) int  equation  = 0;
            RFKField(Serialize()) bool isEnabled = false;
            ;
            RFKField(Serialize()) bool isStartFogEnable = false;
            RFKField(Serialize()) bool isEndFogEnable   = false;

            FogParameter_GENERATED
        };

    protected:
        RFKField(Serialize()) std::string     name = "";
        RFKField(Serialize()) EProjectionType type = EProjectionType::PERSPECTIVE;

        RFKField(Serialize()) float aspect = 16.f / 9.f;
        RFKField(Serialize()) float znear  = 0.001f;
        RFKField(Serialize()) float zfar   = 1000.f;
        RFKField(Serialize()) float hSide  = 1.f;
        RFKField(Serialize()) float vSide  = 1.f;

        RFKField(Serialize()) float fovY = 7.f * PI / 18.f; // 70deg

        RFKField(Serialize()) GPM::Mat4 m_projection;

        RFKField(Serialize()) GPM::Mat4    m_viewMatrix;
        RFKField(Serialize()) GPM::Mat4    m_projectionViewMatrix;
        RFKField(Serialize()) FogParameter m_fogParam;

        void updateProjection();

        virtual void updateToSystem() noexcept override;

    protected:
        /**
         * @brief Update the camera if the FovY change
         * @param owner
         * @return
         */
        void updateFovY();
        void updateAspect();

    public:
        static float computeAspect(int width, int height) noexcept;

    public:
        Camera() noexcept;

        /**
         * @brief Default constructor. Call perspective constructor by default
         * @param owner
         * @return
         */
        Camera(GameObject & owner) noexcept;

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
        Camera(GameObject & owner, const PerspectiveCreateArg& arg) noexcept;

        /**
         * @brief Construct a new orthographic camera object
         *
         * @param owner
         * @param arg
         */
        Camera(GameObject & owner, const OrthographicCreateArg& arg) noexcept;

        Camera(const Camera& other) noexcept = delete;

        virtual ~Camera() noexcept;

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
        void setNear(const float newNear) noexcept;
        void setFar(const float newFar) noexcept;

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

        GETTER_BY_REF(FogParameter, m_fogParam)
        GETTER_BY_VALUE(Far, zfar)
        GETTER_BY_VALUE(Near, znear)

        /**
         * @brief Return struct with all plane that composed the camera frustum
         * @return
         */
        Frustum getFrustum() const noexcept;

        virtual void onPostLoad() override;

        virtual void inspect(InspectContext & context);

        Camera_GENERATED
    };
} // namespace )

#include "Camera.inl"