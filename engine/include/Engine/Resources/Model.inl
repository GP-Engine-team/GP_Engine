#include "Engine/Resources/Model.hpp"

bool Model::isOpaque() const noexcept
{
    return m_isOpaque;
}

Shader* Model::getpShader() noexcept
{
    return m_pShader;
}

Mesh* Model::getpMesh() noexcept
{
    return m_pMesh;
}

bool Model::isLoadInGPU() const noexcept
{
    return m_isLoadInGPU;
}