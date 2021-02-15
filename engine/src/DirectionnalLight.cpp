#include "Engine/Intermediate/Light/DirectionnalLight.hpp"

#include <string>

using namespace Engine::Intermediate;
using namespace GPM;
using namespace Engine::Resources;


DirectionnalLight::DirectionnalLight (GameObject & owner, const DirectionnalLightCreateArg& arg)
            :   Light               {owner, arg.ambient, arg.diffuse, arg.specular},
                m_direction          {arg.direction.normalized()}
{}

            
DirectionnalLight::DirectionnalLight ( GameObject &                       owner,
                                        const GPM::Vec3&   direction,
                                        const AmbiantComponent&            ambient, 
                                        const DiffuseComponent&            diffuse, 
                                        const SpecularComponent&           specular)
:   Light               {owner, ambient, diffuse, specular}, m_direction{direction.normalized()}
{}

void DirectionnalLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back({  m_ambientComp, 
                    m_diffuseComp, 
                    m_specularComp, 
                    m_direction, 3.f,
                    0.f, 0.f, 0.f, 0.f,
                    {0.f, 0.f, 0.f}, 0.f});
}