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
    const float near_plane = 0.1f, far_plane = 1000.f;
    const Mat4  lightProjection =
        Transform::orthographic(m_shadowProterties.size, -m_shadowProterties.size, m_shadowProterties.size,
                                -m_shadowProterties.size, near_plane, far_plane);
    const Vec3 globalPos = getOwner().getTransform().getGlobalPosition();
    const Mat4 lightView = GPM::Transform::lookAt(globalPos, globalPos + m_direction.normalized(), Vec3::up());
    return lightProjection * lightView.inversed();
}

void DirectionalLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back({m_ambientComp,
                  m_diffuseComp,
                  m_specularComp,
                  m_direction.normalized(),
                  3.f,
                  0.f,
                  0.f,
                  0.f,
                  0.f,
                  {0.f, 0.f, 0.f},
                  0.f});
}
