#include "Engine/Resources/Mesh.hpp"

const GPM::Volume* Mesh::getBoundingVolume() const noexcept
{
    return m_boundingVolume.get();
}
