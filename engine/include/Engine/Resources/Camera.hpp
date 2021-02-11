/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"

#include <string>

namespace Engine::Resources
{
    enum class EProjectionType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

   typedef struct S_ProjectionInfo
   {
        std::string name;
        EProjectionType type;

        float aspect;
        float near, far;
        float left, right, top, bottom;
        float fovY, fovX;

    } ProjectionInfo;

    struct CameraPerspectiveCreateArg
    {
        const Engine::Core::Maths::Vec3& position; 
        const Engine::Core::Maths::Vec3& rotation;
        float aspect; 
        float near; 
        float far; 
        float fovY = 70.f;
        const char* name = "Camera";     
    };

    struct CameraOrthographicCreateArg
    {
        const Engine::Core::Maths::Vec3& position; 
        const Engine::Core::Maths::Vec3& rotation;
        float left; 
        float right; 
        float bottom; 
        float top; 
        float nearVal; 
        float farVal;
        const char* name = "Camera";     
    };

    class Camera
        : public Engine::Ressources::GameObject
    {
        protected:

        ProjectionInfo              projInfo_;
        Engine::Core::Maths::Mat4   projection_;

        Engine::Core::Maths::Mat4   viewMatrix_;
        Engine::Core::Maths::Mat4   viewProjectionMatrix_;

        static Camera* camToUse; //pointor to be in nullptr by default

        public:

        constexpr inline
        Camera () noexcept 					            = default;

        constexpr inline
        Camera (const Camera& other) noexcept			= default;

        constexpr inline
        Camera (Camera&& other) noexcept				= default;

        inline
        ~Camera () noexcept				                = default;

        constexpr inline
        Camera& operator=(Camera const& other) noexcept		= default;

        constexpr inline
        Camera& operator=(Camera && other) noexcept			= default;

            /**
             * @brief Construct a new perspective camera object
             * 
             * @param position 
             * @param rotation 
             * @param aspect    : width/height
             * @param near      : must be > 0 
             * @param far 
             * @param fovY      : by default to 70 (human FovY)
             * @param name 
             */
            Camera (const CameraPerspectiveCreateArg& arg);

            /**
             * @brief Construct a new orthographic camera object
             * 
             * @param position 
             * @param rotation 
             * @param left 
             * @param right 
             * @param bottom 
             * @param top 
             * @param nearVal 
             * @param farVal 
             * @param name 
             */
            Camera (const CameraOrthographicCreateArg& arg);

            /**
             * @brief Use this camera to show the next obj drawed
             * 
             */
            void use () noexcept
            {
                camToUse = this;
            }

            /**
             * @brief update Mesh matrix 
             * 
             */
            void update () noexcept override;

            /**
             * @brief update transform if it depend to parent Mesh view (use in inherance in scene graph)
             * 
             * @param parentMeshMatrix : Mesh view matrix of parent
             */
            void update (const Engine::Core::Maths::Mat4& parentMeshMatrix) noexcept override;        

            /**
             * @brief construct view matrix with look at arg. 
             * 
             * @param eye    : Specifies the position of the eye point
             * @param center : Specifies the position of the reference point
             * @param up     : Specifies the direction of the up vector
             */
            void lookAt (const Engine::Core::Maths::Vec3& eye, const Engine::Core::Maths::Vec3& center, const Engine::Core::Maths::Vec3& up) noexcept;

            /**
             * @brief Set the Fov Y object
             * 
             * @param fovY 
             */
            void setFovY(const float fovY) noexcept;

            void setAspect(const float newAspect) noexcept;    

            static Camera* getCamUse() noexcept { return camToUse; }
            static void setCamUse(Camera* newCamToUse) { camToUse = newCamToUse; }
       
            /**
             * @brief Get the Projection Info in specifie index
             * 
             * @return const ProjectionInfo& 
             */
            const ProjectionInfo& getProjectionInfo() const noexcept { return projInfo_; }

            /**
             * @brief Get the view * projection matrix
             * 
             * @return const Engine::Core::Maths::Mat4& 
             */
            const Engine::Core::Maths::Mat4& getViewProjection() const noexcept { return viewProjectionMatrix_; }

            /**
             * @brief Get the view matrix
             * 
             * @return const Engine::Core::Maths::Mat4& 
             */
            const Engine::Core::Maths::Mat4& getView() const noexcept { return viewMatrix_; }

            /**
             * @brief Get the projection matrix
             * 
             * @return const Engine::Core::Maths::Mat4& 
             */
            const Engine::Core::Maths::Mat4& getProjection() const noexcept { return projection_; }
    };
} /*namespace Engine::Resources*/
