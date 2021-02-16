
#include "Engine/Resources/Light/SpotLight.hpp"
#include "GPM/Constants.hpp"

using namespace GPE;

SpotLight::SpotLight(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
                     const SpecularComponent& specular, float constant, float linear, float quadratic, float cutOff,
                     float cutOffExponent)

    : PointLight(owner, ambient, diffuse, specular, constant, linear, quadratic), m_cutOff(cosf(cutOff * PI / 180.f)),
      m_cutOffExponent(cosf(cutOffExponent * PI / 180.f))
{
}

SpotLight::SpotLight(GameObject& owner, CreateArg arg)
    : PointLight(owner, arg.ambient, arg.diffuse, arg.specular, arg.constant, arg.linear, arg.quadratic),
      m_cutOff(cosf(arg.cutOff * PI / 180.f)), m_cutOffExponent(cosf(arg.cutOffExponent * PI / 180.f))
{
}

void SpotLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back({m_ambientComp, m_diffuseComp, m_specularComp, getGameObject().getTransform().getGlobalPosition(), 2.f,
                  m_constant, m_linear, m_quadratic, m_cutOffExponent,
                  getGameObject().getTransform().getVectorForward(), m_cutOff});
}