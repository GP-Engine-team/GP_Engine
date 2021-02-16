#include "Engine/Resources/Light/DirectionalLight.hpp"

DirectionalLight::DirectionalLight(GameObject& owner, const CreateArg& arg) noexcept
    : Light{owner, arg.ambient, arg.diffuse, arg.specular}, m_direction{arg.direction.normalized()}
{
}

DirectionalLight::DirectionalLight(GameObject& owner, const GPM::Vec3& direction, const AmbiantComponent& ambient,
                                   const DiffuseComponent& diffuse, const SpecularComponent& specular) noexcept
    : Light{owner, ambient, diffuse, specular}, m_direction{direction.normalized()}
{
}

void DirectionalLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back(
        {m_ambientComp, m_diffuseComp, m_specularComp, m_direction, 3.f, 0.f, 0.f, 0.f, 0.f, {0.f, 0.f, 0.f}, 0.f});
}

void DirectionalLight::setDirection(const GPM::Vec3& newDirection) noexcept
{
    m_direction = newDirection.normalized();
}