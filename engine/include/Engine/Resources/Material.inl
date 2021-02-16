#include "Engine/Resources/Material.hpp"

Material::Material(const MaterialAttrib& arg)
    : m_comp{arg.ambient, arg.diffuse, arg.specular, arg.shininess},
      m_pDiffuseTexture(
          std::make_shared<Texture>((std::string(PATH_TEXTURE_RESOURCE) + arg.nameDiffuseTexture).c_str())),
      m_name(arg.id)
{
}

Material::Material(CreateArg& arg) : m_comp(arg.comp), m_pDiffuseTexture(std::move(arg.pTexture)), m_name(arg.name)
{
}

const MaterialComponent& Material::getMaterialComponent() const noexcept
{
    return m_comp;
}

Texture* Material::getPDiffuseTexture() noexcept
{
    return m_pDiffuseTexture.get();
}

const std::string& Material::getName() const noexcept
{
    return m_name;
}