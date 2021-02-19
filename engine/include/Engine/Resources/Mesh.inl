#include "Engine/Resources/Mesh.hpp"

Mesh::~Mesh() noexcept
{
    if (m_isLoadInGPU)
        unloadFromGPU();
}

const GLuint* Mesh::getVAOId() noexcept
{
    return &m_indexVAO;
}

const std::vector<GPM::Vec3>& Mesh::getVertices() const noexcept
{
    return m_vBuffer;
}

const std::vector<GPM::Vec2>& Mesh::getUV() const noexcept
{
    return m_vtBuffer;
}

const std::vector<GPM::Vec3>& Mesh::getNormals() const noexcept
{
    return m_vnBuffer;
}

const std::vector<std::vector<Indice>>& Mesh::getIndices() const noexcept
{
    return m_iBuffer;
}

const std::vector<std::string>& Mesh::getIdMaterials() const noexcept
{
    return m_idMaterial;
}