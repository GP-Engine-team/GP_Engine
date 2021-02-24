#include "Engine/Resources/Mesh.hpp"

unsigned int Mesh::getID() const noexcept
{
    return static_cast<unsigned int>(m_indexVAO);
}

Mesh::BoundingVolume Mesh::getBoundingVolumeType() const noexcept
{
    return m_boundingVolumeType;
}

const GPM::Volume* Mesh::getBoundingVolume() const noexcept
{
    return m_boundingVolume.get();
}

unsigned int Mesh::getVerticesCount() const noexcept
{
    return m_verticesCount;
}