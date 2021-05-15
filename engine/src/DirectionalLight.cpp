#include "Engine/ECS/Component/Light/DirectionalLight.hpp"

File_GENERATED

    using namespace GPE;
using namespace GPM;

DirectionalLight::~DirectionalLight()
{
}

DirectionalLight::DirectionalLight(GameObject& owner) noexcept : DirectionalLight(owner, CreateArg{})
{
}

DirectionalLight::DirectionalLight(GameObject& owner, const CreateArg& arg) noexcept
    : Light{owner, arg.ambient, arg.diffuse, arg.specular}, m_direction{arg.direction.normalized()}
{
}

Mat4 DirectionalLight::getLightSpaceMatrix() noexcept
{
    const float near_plane = 0.001f, far_plane = 10000.f;
    const Mat4  lightProjection = Transform::orthographic(10000.0f, 10000.0f, near_plane, far_plane);
    const Mat4  lightView       = GPM::Transform::lookAt(Vec3::zero(), m_direction, Vec3::up());
    return lightProjection * lightView;
}

void DirectionalLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back(
        {m_ambientComp, m_diffuseComp, m_specularComp, m_direction, 3.f, 0.f, 0.f, 0.f, 0.f, {0.f, 0.f, 0.f}, 0.f});
}
