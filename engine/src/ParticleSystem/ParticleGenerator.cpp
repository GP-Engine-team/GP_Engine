#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>

#include "GPM/Random.hpp"
#include "GPM/Vector4.hpp"
#include "GPM/constants.hpp"
#include <Engine/Serialization/xml/xmlSaver.hpp>
#include <algorithm>
#include <assert.h>

// Generated
#include <Generated/ParticleGenerator.rfk.h>
File_GENERATED

    using namespace GPE;
using namespace GPM;

void BoxPosGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    Vec4 posMin{m_pos.x - m_maxStartPosOffset.x, m_pos.y - m_maxStartPosOffset.y, m_pos.z - m_maxStartPosOffset.z, 1.f};
    Vec4 posMax{m_pos.x + m_maxStartPosOffset.x, m_pos.y + m_maxStartPosOffset.y, m_pos.z + m_maxStartPosOffset.z, 1.f};

    for (size_t i = startId; i < endId; ++i)
    {
        p->m_pos[i].x = Random::ranged(posMin.x, posMax.x);
        p->m_pos[i].y = Random::ranged(posMin.y, posMax.y);
        p->m_pos[i].z = Random::ranged(posMin.z, posMax.z);
        p->m_pos[i].w = Random::ranged(posMin.w, posMax.w);
    }
}

U16BMask BoxPosGen::getRequiereConfig() const
{
    return ParticleData::EParam::POSITION;
}

void RoundPosGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        float ang   = Random::ranged(0.f, PI * 2.f);
        p->m_pos[i] = m_center + Vec4(m_radX * sin(ang), m_radY * cos(ang), 0.f, 1.f);
    }
}

U16BMask RoundPosGen::getRequiereConfig() const
{
    return ParticleData::EParam::POSITION;
}

void BasicColorGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        p->m_startCol[i].r = Random::ranged(m_minStartCol.r, m_maxStartCol.r);
        p->m_startCol[i].g = Random::ranged(m_minStartCol.g, m_maxStartCol.b);
        p->m_startCol[i].b = Random::ranged(m_minStartCol.b, m_maxStartCol.b);
        p->m_startCol[i].a = Random::ranged(m_minStartCol.a, m_maxStartCol.a);

        p->m_endCol[i].r = Random::ranged(m_minEndCol.r, m_maxEndCol.r);
        p->m_endCol[i].g = Random::ranged(m_minEndCol.g, m_maxEndCol.g);
        p->m_endCol[i].b = Random::ranged(m_minEndCol.b, m_maxEndCol.b);
        p->m_endCol[i].a = Random::ranged(m_minEndCol.a, m_maxEndCol.a);
    }
}

U16BMask BasicColorGen::getRequiereConfig() const
{
    return ParticleData::EParam::END_COLOR | ParticleData::EParam::START_COLOR;
}

void BasicVelGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        p->m_vel[i].x = Random::ranged(m_minStartVel.x, m_maxStartVel.x);
        p->m_vel[i].y = Random::ranged(m_minStartVel.y, m_maxStartVel.y);
        p->m_vel[i].z = Random::ranged(m_minStartVel.z, m_maxStartVel.z);
        p->m_vel[i].w = Random::ranged(m_minStartVel.w, m_maxStartVel.w);
    }
}

U16BMask BasicVelGen::getRequiereConfig() const
{
    return ParticleData::EParam::VELOCITY;
}

void SphereVelGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    float phi, theta, v, r;
    for (size_t i = startId; i < endId; ++i)
    {
        phi   = Random::ranged(-PI, PI);
        theta = Random::ranged(-PI, PI);
        v     = Random::ranged(m_minVel, m_maxVel);

        r             = v * sinf(phi);
        p->m_vel[i].z = v * cosf(phi);
        p->m_vel[i].x = r * cosf(theta);
        p->m_vel[i].y = r * sinf(theta);
    }
}

U16BMask SphereVelGen::getRequiereConfig() const
{
    return ParticleData::EParam::VELOCITY;
}

void VelFromPosGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        float scale = Random::ranged(m_minScale, m_maxScale);
        Vec4  vel   = (p->m_pos[i] - m_offset);
        p->m_vel[i] = vel * scale;
    }
}

U16BMask VelFromPosGen::getRequiereConfig() const
{
    return ParticleData::EParam::VELOCITY | ParticleData::EParam::POSITION;
}

void BasicTimeGen::generate(ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        p->m_time[i].x = p->m_time[i].y = Random::ranged(m_minTime, m_maxTime);
        p->m_time[i].z                  = 0.f;
        p->m_time[i].w                  = 1.f / p->m_time[i].x;
    }
}

U16BMask BasicTimeGen::getRequiereConfig() const
{
    return ParticleData::EParam::TIME;
}