namespace GPE
{

const GPM::Volume* Mesh::getBoundingVolume() const noexcept
{
    return m_boundingVolume.get();
}

} // namespace GPE