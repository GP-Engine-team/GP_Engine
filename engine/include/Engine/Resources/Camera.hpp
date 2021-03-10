/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

#include "Engine/Intermediate/Component.hpp"
#include "GPM/Matrix4.hpp"
#include "GPM/Shape3D/Plane.hpp"

// in inl
#include "Engine/Intermediate/GameObject.hpp"

namespace GPE
{

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

class Camera : public Component
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
        EProjectionType type = EProjectionType::NONE;

        float aspect = 16.f / 9.f;
        float znear  = 0.001f;
        float zfar   = 10.f;
        float hSide  = 1.f;
        float vSide  = 1.f;
        float fovY   = 70.f;
        float fovX   = 70.f;
    };

    struct PerspectiveCreateArg
    {
        float       aspect = 16.f / 9.f;
        float       znear  = 0.001f;
        float       zfar   = 10.f;
        float       fovY   = 70.f;
        const char* name   = "Camera";
    };

    struct OrthographicCreateArg
    {
        float       hSide   = 1.f;
        float       vSide   = 1.f;
        float       znear   = 0.001f;
        float       zfar    = 10.f;
        const char* name    = "Camera";
    };

protected:
    ProjectionInfo m_projInfo;
    GPM::Mat4      m_projection;

    GPM::Mat4 m_viewMatrix;
    GPM::Mat4 m_viewProjectionMatrix;

    void updateProjection();

public:
    Camera() noexcept                    = delete;
    Camera(const Camera& other) noexcept = delete;
    Camera& operator=(Camera const& other) noexcept = delete;

    Camera(Camera&& other) noexcept = default;
    Camera& operator=(Camera&& other) noexcept = default;
    virtual ~Camera() noexcept                 = default;

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
     * @param position
     * @param rotation
     * @param left
     * @param right
     * @param bottom
     * @param top
     * @param znear
     * @param zfar
     * @param name
     */
    Camera(GameObject& owner, const OrthographicCreateArg& arg) noexcept;

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
    inline const GPM::Mat4& getViewProjection() const noexcept;

    /**
     * @brief Get the view matrix
     *
     * @return const GPM::Mat4&
     */
    inline GPM::Mat4 getView() const noexcept;

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
};

#include "Camera.inl"

} /*namespace GPE*/
