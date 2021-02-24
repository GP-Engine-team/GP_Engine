#include "Engine/Resources/Model.hpp"

bool Model::isOpaque() const noexcept
{
    return m_isOpaque;
}

Shader* Model::getpShader() noexcept
{
    return m_pShader;
}

const std::vector<Mesh>& Model::getSubMeshs() const noexcept
{
    return *m_pSubMeshs;
}