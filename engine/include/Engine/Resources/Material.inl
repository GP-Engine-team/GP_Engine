namespace GPE
{
inline Material::Material(const CreateArg& arg)
    : m_comp(arg.comp), m_pDiffuseTexture(arg.pDiffuseTexture), m_pNormalMapTexture(arg.pNormalMapTexture),
      m_pShader(arg.pShader), m_ID{++materialCount}
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

inline void Material::setShader(Shader& newShader)
{
    m_pShader = &newShader;
    resetUniform();
}
} // namespace GPE