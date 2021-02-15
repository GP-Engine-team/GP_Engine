#include "Engine/Intermediate/Light/Light.hpp"
#include "Engine/Intermediate/RenderSystem.hpp"

using namespace GPM;
using namespace Engine::Intermediate;
using namespace Engine::Resources;

Light::Light(Engine::Intermediate::GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
             const SpecularComponent& specular)
    : Component{owner}, m_ambientComp(ambient), m_diffuseComp(diffuse), m_specularComp(specular)
{
    RenderSystem::getInstance()->addLight(this);
}

Light::Light(Engine::Intermediate::GameObject& owner, const CreateArg& arg)
    : Component{owner}, m_ambientComp(arg.ambient), m_diffuseComp(arg.diffuse), m_specularComp(arg.specular)
{
    RenderSystem::getInstance()->addLight(this);
}

Light::~Light()
{
    RenderSystem::getInstance()->removeLight(this);
}