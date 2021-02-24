#include "Engine/Resources/Mesh.hpp"

const GLuint* Mesh::getID() const noexcept
{
    return &m_indexVAO;
}

Mesh::BoundingVolume Mesh::getBoundingVolumeType() const noexcept
{
    return m_boundingVolumeType;
}

const GPM::Volume* Mesh::getBoundingVolume() const noexcept
{
    return m_boundingVolume.get();
}