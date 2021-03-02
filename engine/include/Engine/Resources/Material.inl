#include "Engine/Resources/Material.hpp"

Material::Material(const CreateArg& arg) : m_comp(arg.comp), m_pDiffuseTexture(arg.pTexture), m_name(arg.name)
{
}

inline bool Material::isOpaque() const noexcept
{
    return !(m_comp.opacity < 1.f);
}
