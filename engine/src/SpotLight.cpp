
#include "Engine/Intermediate/Light/SpotLight.hpp"
#include "GPM/Constants.hpp"

using namespace Engine::Intermediate;
using namespace Engine::Resources;

SpotLight::SpotLight ( GameObject &            owner,
            const AmbiantComponent&            ambient, 
            const DiffuseComponent&            diffuse, 
            const SpecularComponent&           specular,
            float                              constant, 
            float                              linear, 
            float                              quadratic,
            float                              cutOff,
            float                              cutOffExponent, 
            bool                               isEnable)

:   PointLight          (owner, ambient, diffuse, specular, constant, linear, quadratic, isEnable),
    m_cutOff             (cosf(cutOff * PI / 180.f)),
    m_cutOffExponent     (cosf(cutOffExponent * PI / 180.f))
{}

SpotLight::SpotLight (GameObject & owner, SpotLightCreateArg arg)
:   PointLight          (owner, arg.ambient, arg.diffuse, arg.specular, arg.constant, arg.linear, arg.quadratic, arg.isEnable),
    m_cutOff             (cosf(arg.cutOff * PI / 180.f)),
    m_cutOffExponent     (cosf(arg.cutOffExponent * PI / 180.f))
{}

void SpotLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back({  m_ambientComp, 
                    m_diffuseComp, 
                    m_specularComp,
                    getGameObject().getGlobalPosition(), 2.f,
                    m_constant, m_linear, m_quadratic, m_cutOffExponent,
                    m_gameObject.getVecForward(), m_cutOff});
}