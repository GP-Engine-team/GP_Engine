﻿namespace GPE
{
Light::Light(GameObject& owner, const AmbiantComponent& ambient, const DiffuseComponent& diffuse,
             const SpecularComponent& specular)
    : Component{owner}, m_ambientComp(ambient), m_diffuseComp(diffuse), m_specularComp(specular)
{
    updateToSystem();
}

Light::Light(GameObject& owner, const CreateArg& arg)
    : Component{owner}, m_ambientComp(arg.ambient), m_diffuseComp(arg.diffuse), m_specularComp(arg.specular)
{
    updateToSystem();
}

Light::~Light()
{
    if (getOwner().pOwnerScene && m_isActivated)
    {
        if (m_shadowProperties.isEnable)
            getOwner().pOwnerScene->sceneRenderer.removeShadowMap(*this);

        getOwner().pOwnerScene->sceneRenderer.removeLight(*this);
    }
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

const Light::ShadowProperties& Light::getShadowProperties() const noexcept
{
    return m_shadowProperties;
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
} // namespace GPE