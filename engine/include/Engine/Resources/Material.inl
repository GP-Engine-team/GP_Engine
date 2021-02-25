#include "Engine/Resources/Material.hpp"

Material::Material(const CreateArg& arg) : m_comp(arg.comp), m_pDiffuseTexture(arg.pTexture), m_name(arg.name)
{
}

const MaterialComponent& Material::getMaterialComponent() const noexcept
{
    return m_comp;
}

const Texture* Material::getDiffuseTexture() const noexcept
{
    return m_pDiffuseTexture;
}

const std::string& Material::getName() const noexcept
{
    return m_name;
}

inline bool Material::isOpaque() const noexcept
{
    return !(m_comp.opacity < 1.f);
}