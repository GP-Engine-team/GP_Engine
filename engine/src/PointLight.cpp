#include "Engine/Intermediate/Light/PointLight.hpp"

using namespace Engine::Intermediate;
using namespace Engine::Resources;

PointLight::PointLight(GameObject& owner, const CreateArg& arg)
    : Light(owner, arg.ambient, arg.diffuse, arg.specular), m_constant(arg.constant), m_linear(arg.linear),
      m_quadratic(arg.quadratic)
{
}

PointLight::PointLight(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
                       const SpecularComponent& specular, float constant, float linear, float quadratic)
    : Light(owner, ambient, diffuse, specular), m_constant(constant), m_linear(linear), m_quadratic(quadratic)
{
}

void PointLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back({m_ambientComp,
                  m_diffuseComp,
                  m_specularComp,
                  getGameObject().getTransform().getGlobalPosition(),
                  1.f,
                  m_constant,
                  m_linear,
                  m_quadratic,
                  0.f,
                  {0.f, 0.f, 0.f},
                  0.f});
}