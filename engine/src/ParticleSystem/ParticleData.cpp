#include <Engine/Resources/ParticleSystem/ParticleData.hpp>

#include <Engine/Core/Debug/Assert.hpp>

#include <utility>

using namespace GPE;
using namespace GPM;

void ParticleData::generate(size_t maxSize, U16BMask maskType)
{
    m_maskType   = maskType;
    m_count      = maxSize;
    m_countAlive = 0;

    if (m_maskType.isSet(EParam::POSITION))
        m_pos.reset(new Vec4[maxSize]);

    if (m_maskType.isSet(EParam::COLOR))
        m_col.reset(new ColorRGBA[maxSize]);

    if (m_maskType.isSet(EParam::START_COLOR))
        m_startCol.reset(new ColorRGBA[maxSize]);

    if (m_maskType.isSet(EParam::END_COLOR))
        m_endCol.reset(new ColorRGBA[maxSize]);

    if (m_maskType.isSet(EParam::VELOCITY))
        m_vel.reset(new Vec4[maxSize]);

    if (m_maskType.isSet(EParam::ACCELERATION))
        m_acc.reset(new Vec4[maxSize]);

    if (m_maskType.isSet(EParam::TIME))
        m_time.reset(new Vec4[maxSize]);

    if (m_maskType.isSet(EParam::START_SIZE))
        m_startSize.reset(new float[maxSize]);

    if (m_maskType.isSet(EParam::END_SIZE))
        m_endSize.reset(new float[maxSize]);

    m_alive.reset(new bool[maxSize]);
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
    if (m_maskType.isSet(EParam::POSITION))
        m_pos[a] = m_pos[b];

    if (m_maskType.isSet(EParam::COLOR))
        m_col[a] = m_col[b];

    if (m_maskType.isSet(EParam::START_COLOR))
        m_startCol[a] = m_startCol[b];

    if (m_maskType.isSet(EParam::END_COLOR))
        m_endCol[a] = m_endCol[b];

    if (m_maskType.isSet(EParam::VELOCITY))
        m_vel[a] = m_vel[b];

    if (m_maskType.isSet(EParam::ACCELERATION))
        m_acc[a] = m_acc[b];

    if (m_maskType.isSet(EParam::TIME))
        m_time[a] = m_time[b];

    if (m_maskType.isSet(EParam::START_SIZE))
        m_startSize[a] = m_startSize[b];

    if (m_maskType.isSet(EParam::END_SIZE))
        m_endSize[a] = m_endSize[b];

    // m_alive[a] = m_alive[b];*/
}