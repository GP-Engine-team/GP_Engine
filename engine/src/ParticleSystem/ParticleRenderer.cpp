#include <Engine/Resources/ParticleSystem/GLParticleRenderer.hpp>
#include <Engine/Resources/ParticleSystem/ParticleRenderer.hpp>

#include <string>

using namespace GPE;

std::shared_ptr<IParticleRenderer> RendererFactory::create(const char* name)
{
    std::string renderer{name};

    if (renderer == "gl")
        return std::make_shared<GLParticleRenderer>();

    return nullptr;
}