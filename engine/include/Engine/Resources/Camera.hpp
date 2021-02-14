/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>

#include "Engine/Intermediate/Component.hpp"
#include "GPM/Matrix4.hpp"

namespace Engine::Resources
{
enum class EProjectionType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

struct ProjectionInfo
{
    std::string name;
    EProjectionType type;

    float aspect;
    float near, far;
    float hSide, vSide;
    float fovY, fovX;
};

struct CameraPerspectiveCreateArg
{
    float aspect = 16.f / 9.f;
    float near = 0.001f;
    float far = 10.f;
    float fovY = 70.f;
    const char* name = "Camera";
};

struct CameraOrthographicCreateArg
{
    float hSide = 1.f;
    float vSide = 1.f;
    float nearVal = 0.001f;
    float farVal = 10.f;
    const char* name = "Camera";
};

class Camera : public Engine::Intermediate::Component
{
protected:
    ProjectionInfo m_projInfo;
    GPM::Mat4 m_projection;

    GPM::Mat4 m_viewMatrix;
    GPM::Mat4 m_viewProjectionMatrix;

public:
    Camera() noexcept = delete;
    Camera(const Camera& other) noexcept = delete;
    Camera& operator=(Camera const& other) noexcept = delete;

    Camera(Camera&& other) noexcept = default;
    Camera& operator=(Camera&& other) noexcept = default;
    virtual ~Camera() noexcept = default;

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
    Camera(Engine::Intermediate::GameObject& owner, const CameraPerspectiveCreateArg& arg);

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
    Camera(Engine::Intermediate::GameObject& owner, const CameraOrthographicCreateArg& arg);

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
    const ProjectionInfo& getProjectionInfo() const noexcept
    {
        return m_projInfo;
    }

    /**
     * @brief Get the view * projection matrix
     *
     * @return const GPM::Mat4&
     */
    const GPM::Mat4& getViewProjection() const noexcept
    {
        return m_viewProjectionMatrix;
    }

    /**
     * @brief Get the view matrix
     *
     * @return const GPM::Mat4&
     */
    const GPM::Mat4& getView() const noexcept;

    /**
     * @brief Get the projection matrix
     *
     * @return const GPM::Mat4&
     */
    const GPM::Mat4& getProjection() const noexcept
    {
        return m_projection;
    }
};
} /*namespace Engine::Resources*/
