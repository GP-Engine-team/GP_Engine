#include "Engine/ECS/Component/Light/Light.hpp"

Light::Light(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
             const SpecularComponent& specular)
    : Component{owner}, m_ambientComp(ambient), m_diffuseComp(diffuse), m_specularComp(specular)
{
    getOwner().pOwnerScene->sceneRenderer.addLight(this);
}

Light::Light(GameObject& owner, const CreateArg& arg)
    : Component{owner}, m_ambientComp(arg.ambient), m_diffuseComp(arg.diffuse), m_specularComp(arg.specular)
{
    getOwner().pOwnerScene->sceneRenderer.addLight(this);
}

Light::~Light()
{
    getOwner().pOwnerScene->sceneRenderer.removeLight(this);
}

Light& Light::operator=(Light&& other)
{
    m_ambientComp  = std::move(other.m_ambientComp);
    m_diffuseComp  = std::move(other.m_diffuseComp);
    m_specularComp = std::move(other.m_specularComp);

    getOwner().pOwnerScene->sceneRenderer.updateLightPointer(this, &other);

    Component::operator=(std::move(other));
    return *this;
}

const AmbiantComponent& Light::getAmbient() const noexcept
{
    return m_ambientComp;
}
const DiffuseComponent& Light::getDiffuse() const noexcept
{
    return m_diffuseComp;
}
const SpecularComponent& Light::getSpecular() const noexcept
{
    return m_specularComp;
}

void Light::setGlobalComponent(const ColorRGBA& newComponent) noexcept
{
    m_ambientComp.rgba  = newComponent;
    m_diffuseComp.rgba  = newComponent;
    m_specularComp.rgba = newComponent;
}

void Light::setGlobalComponent(const GPM::Vec4& newComponent) noexcept
{
    m_ambientComp.rgbi  = newComponent;
    m_diffuseComp.rgbi  = newComponent;
    m_specularComp.rgbi = newComponent;
}

void Light::setAmbient(const AmbiantComponent& newAmbient) noexcept
{
    m_ambientComp.rgba = newAmbient.rgba;
}

void Light::setDiffuse(const DiffuseComponent& newDiffuse) noexcept
{
    m_diffuseComp.rgba = newDiffuse.rgba;
}

void Light::setSpecular(const SpecularComponent& newSpecular) noexcept
{
    m_specularComp.rgba = newSpecular.rgba;
}

void Light::setAmbient(const GPM::Vec4& newAmbient) noexcept
{
    m_ambientComp.rgbi = newAmbient;
}

void Light::setDiffuse(const GPM::Vec4& newDiffuse) noexcept
{
    m_diffuseComp.rgbi = newDiffuse;
}

void Light::setSpecular(const GPM::Vec4& newSpecular) noexcept
{
    m_specularComp.rgbi = newSpecular;
}
