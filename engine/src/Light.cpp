#include "Engine/Intermediate/Light/Light.hpp"

using namespace GPM;
using namespace Engine::Intermediate;
using namespace Engine::Resources;

Light::Light (Engine::Intermediate::GameObject &      owner, 
                const AmbiantComponent&             ambient, 
                const DiffuseComponent&             diffuse, 
                const SpecularComponent&            specular,
                bool isEnable)
    :   Component          {owner},
        m_ambientComp       (ambient),
        m_diffuseComp       (diffuse),
        m_specularComp      (specular),
        m_isEnable          (!isEnable)
{
    enable (isEnable);
}

Light::Light (Engine::Intermediate::GameObject &owner, const LightCreateArg&    arg)
    :   Component          {owner},
        m_ambientComp       (arg.ambient),
        m_diffuseComp       (arg.diffuse),
        m_specularComp      (arg.specular),
        m_isEnable          (!arg.isEnable)
{
    enable (arg.isEnable);
}

Light::~Light()
{
    if (m_isEnable)
    {
        enable(false);
    }
}

void Light::enable (bool flag) throw() 
{
    if(m_isEnable == flag)
        return;
    else    
        m_isEnable = flag;
}