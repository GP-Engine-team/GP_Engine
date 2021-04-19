#include "Engine/ECS/Component/Light/DirectionalLight.hpp"

File_GENERATED

using namespace GPE;

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

void DirectionalLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back(
        {m_ambientComp, m_diffuseComp, m_specularComp, m_direction, 3.f, 0.f, 0.f, 0.f, 0.f, {0.f, 0.f, 0.f}, 0.f});
}
