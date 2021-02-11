#include "Engine/Resources/Camera.hpp"
#include "Engine/Core/Debug/Log.hpp"
#include <iostream>

using namespace Engine::Resources;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Debug;

Camera::Camera(const CameraPerspectiveCreateArg& arg)
    : GameObject(GameObjectCreateArg{arg.name, arg.position, arg.rotation, {1.0f, 1.0f, 1.0f}})
{
    GE_assert(arg.near > 0.f);

    ProjectionInfo info;

    info.name = arg.name;
    info.type = E_ProjectionType::PERSPECTIVE;
    info.aspect = arg.aspect;
    info.near = arg.near;
    info.far = arg.far;
    info.fovY = arg.fovY;
    info.fovX = arg.aspect * arg.fovY;
    info.left = arg.far * sinf(info.fovX / 2.f);
    info.right = -info.left;
    info.top = arg.far * sinf(info.fovY / 2.f);
    info.bottom = -info.top;

    projInfo_ = info;
    projection_ = Mat4::createPerspectiveMatrix(info.aspect, info.near, info.far, info.fovY);

    isDirty_ = true;
    update();

    SLog::log((std::string("Perspective projection add with name \"") + arg.name + "\"").c_str());
}

Camera::Camera(const CameraOrthographicCreateArg& arg)
    : GameObject(GameObjectCreateArg{arg.name, arg.position, arg.rotation, {1.0f, 1.0f, 1.0f}})
{
    GE_assert(arg.nearVal > 0.f);

    ProjectionInfo info;

    float distLF = abs(arg.left - arg.right);
    float distTB = abs(arg.top - arg.bottom);

    info.name = arg.name;
    info.type = E_ProjectionType::ORTHOGRAPHIC;
    info.aspect = distLF / distTB;
    info.near = arg.nearVal;
    info.far = arg.farVal;
    info.fovY = arg.farVal * asinf(distTB);
    info.fovX = arg.farVal * asinf(distLF);
    info.left = arg.left;
    info.right = arg.right;
    info.top = arg.top;
    info.bottom = arg.bottom;

    projInfo_ = info;
    projection_ = Mat4::createOrthoMatrix(info.left, info.right, info.bottom, info.top, info.near, info.far);

    isDirty_ = true;
    update();

    SLog::log((std::string("Orthographic projection add with name \"") + arg.name + "\"").c_str());
}

void Camera::update() noexcept
{
    if (!isDirty_)
        return;

    modelMat_ = Engine::Core::Maths::Mat4::createTRSMatrix(scale_, rotation_, position_);
    modelMat_.inverse(viewMatrix_);

    viewProjectionMatrix_ = projection_ * viewMatrix_;
    isDirty_ = false;
}

void Camera::update(const Engine::Core::Maths::Mat4& parentMeshMatrix) noexcept
{
    modelMat_ = Engine::Core::Maths::Mat4::createTRSMatrix(scale_, rotation_, position_);
    modelMat_.inverse(viewMatrix_);

    modelMat_ = parentMeshMatrix * modelMat_;
    viewProjectionMatrix_ = projection_ * viewMatrix_;
    isDirty_ = false;
}

void Camera::lookAt(const Engine::Core::Maths::Vec3& eye, const Engine::Core::Maths::Vec3& center,
                    const Engine::Core::Maths::Vec3& up) noexcept
{
    position_ = eye;
    viewMatrix_ = Mat4::createLookAtView(eye, center, up);
    viewProjectionMatrix_ = projection_ * viewMatrix_;
}

void Camera::setFovY(const float fovY) noexcept
{
    projInfo_.fovY = fovY;
    projInfo_.fovX = projInfo_.aspect * fovY;
    projInfo_.left = projInfo_.far * sinf(projInfo_.fovX / 2.f);
    projInfo_.right = -projInfo_.left;
    projInfo_.top = projInfo_.far * sinf(projInfo_.fovY / 2.f);
    projInfo_.bottom = -projInfo_.top;

    switch (projInfo_.type)
    {
    case E_ProjectionType::ORTHOGRAPHIC:
        projection_ = Mat4::createOrthoMatrix(projInfo_.left, projInfo_.right, projInfo_.bottom, projInfo_.top,
                                              projInfo_.near, projInfo_.far);
        break;

    case E_ProjectionType::PERSPECTIVE:
        projection_ = Mat4::createPerspectiveMatrix(projInfo_.aspect, projInfo_.near, projInfo_.far, projInfo_.fovY);
        break;

    default:
        functWarning("Other projection not implemented");
        break;
    }
}

void Camera::setAspect(const float newAspect) noexcept
{
    projInfo_.aspect = newAspect;
    projInfo_.fovX = projInfo_.aspect * projInfo_.fovY;
    projInfo_.left = projInfo_.far * sinf(projInfo_.fovX / 2.f);
    projInfo_.right = -projInfo_.left;

    switch (projInfo_.type)
    {
    case E_ProjectionType::ORTHOGRAPHIC:
        projection_ = Mat4::createOrthoMatrix(projInfo_.left, projInfo_.right, projInfo_.bottom, projInfo_.top,
                                              projInfo_.near, projInfo_.far);
        break;

    case E_ProjectionType::PERSPECTIVE:
        projection_ = Mat4::createPerspectiveMatrix(projInfo_.aspect, projInfo_.near, projInfo_.far, projInfo_.fovY);
        break;

    default:
        functWarning("Other projection not implemented");
        break;
    }
}

Camera* Camera::camToUse(nullptr);