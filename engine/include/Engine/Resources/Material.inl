#include "Engine/Resources/Material.hpp"

Material::Material(const CreateArg& arg)
    : m_comp(arg.comp), m_pAmbianteTexture(arg.pAmbianteTexture), m_pDiffuseTexture(arg.pDiffuseTexture),
      m_pBaseColorTexture(arg.pBaseColorTexture), m_ID{++materialCount}
{
}

inline bool Material::isOpaque() const noexcept
{
    return !(m_comp.opacity < 1.f);
}

inline unsigned int Material::getID() const
{
    return m_ID;
}

inline void Material::generateNewMaterial()
{
    m_ID = ++materialCount;
}