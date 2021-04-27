#include <Engine/Resources/ParticleSystem/ParticleData.hpp>

#include <Engine/Core/Debug/Assert.hpp>

#include <utility>

using namespace GPE;
using namespace GPM;

void ParticleData::generate(size_t maxSize, uint8_t maskType)
{
    m_maskType   = maskType;
    m_count      = maxSize;
    m_countAlive = 0;

    if (m_maskType & EParam::POSITION)
        m_pos.reset(new Vec4[maxSize]);

    if (m_maskType & EParam::COLOR)
        m_col.reset(new Vec4[maxSize]);

    if (m_maskType & EParam::START_COLOR)
        m_startCol.reset(new Vec4[maxSize]);

    if (m_maskType & EParam::END_COLOR)
        m_endCol.reset(new Vec4[maxSize]);

    if (m_maskType & EParam::VELOCITY)
        m_vel.reset(new Vec4[maxSize]);

    if (m_maskType & EParam::ACCELERATION)
        m_acc.reset(new Vec4[maxSize]);

    if (m_maskType & EParam::TIME)
        m_time.reset(new Vec4[maxSize]);

    m_alive.reset(new bool[maxSize]);
}

bool ParticleData::isParamEnable(EParam param) const
{
    return m_maskType & param;
}

void ParticleData::addParam(EParam param)
{
    m_maskType |= param;
}

void ParticleData::removeParam(EParam param)
{
    m_maskType &= ~param;
}

void ParticleData::invertParamState(EParam param)
{
    m_maskType ^= param;
}

void ParticleData::kill(size_t id)
{
    // if (m_countAlive > 0) // maybe this if can be removed?
    {
        m_alive[id] = false;
        swapData(id, m_countAlive - 1);
        m_countAlive--;
    }
}

void ParticleData::wake(size_t id)
{
    // if (m_countAlive < m_count) // maybe this if can be removed?
    {
        m_alive[id] = true;
        // swapData(id, m_countAlive);
        m_countAlive++;
    }
}

void ParticleData::swapData(size_t a, size_t b)
{
    /*std::swap(m_pos[a], m_pos[b]);
    std::swap(m_col[a], m_col[b]);
    std::swap(m_startCol[a], m_startCol[b]);
    std::swap(m_endCol[a], m_endCol[b]);
    std::swap(m_vel[a], m_vel[b]);
    std::swap(m_acc[a], m_acc[b]);
    std::swap(m_time[a], m_time[b]);
    std::swap(m_alive[a], m_alive[b]);*/
    m_pos[a]      = m_pos[b];
    m_col[a]      = m_col[b];
    m_startCol[a] = m_startCol[b];
    m_endCol[a]   = m_endCol[b];
    m_vel[a]      = m_vel[b];
    m_acc[a]      = m_acc[b];
    m_time[a]     = m_time[b];
    // m_alive[a] = m_alive[b];*/
}

void ParticleData::copyOnlyAlive(const ParticleData* source, ParticleData* destination)
{
    GPE_ASSERT(source->m_count == destination->m_count, "source->m_count != destination->m_count");

    size_t id = 0;
    for (size_t i = 0; i < source->m_countAlive; ++i)
    {
        // if (source->m_alive[i])
        {
            destination->m_pos[id]      = source->m_pos[i];
            destination->m_col[id]      = source->m_col[i];
            destination->m_startCol[id] = source->m_startCol[i];
            destination->m_endCol[id]   = source->m_endCol[i];
            destination->m_vel[id]      = source->m_vel[i];
            destination->m_acc[id]      = source->m_acc[i];
            destination->m_time[id]     = source->m_time[i];
            destination->m_alive[id]    = true;
            id++;
        }
    }

    destination->m_countAlive = id;
}