#include <Engine/Resources/ParticleSystem/ParticleGenerator.hpp>

File_GENERATED

#include "GPM/Random.hpp"
#include "GPM/Vector4.hpp"
#include "GPM/constants.hpp"
#include <algorithm>
#include <assert.h>

    using namespace GPE;
using namespace GPM;

void BoxPosGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
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

uint16_t BoxPosGen::getRequiereConfig() const
{
    return ParticleData::EParam::POSITION;
}

void RoundPosGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        float ang   = Random::ranged(0.f, PI * 2.f);
        p->m_pos[i] = m_center + Vec4(m_radX * sin(ang), m_radY * cos(ang), 0.f, 1.f);
    }
}

uint16_t RoundPosGen::getRequiereConfig() const
{
    return ParticleData::EParam::POSITION;
}

void BasicColorGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        p->m_startCol[i].x = Random::ranged(m_minStartCol.x, m_maxStartCol.x);
        p->m_startCol[i].y = Random::ranged(m_minStartCol.y, m_maxStartCol.y);
        p->m_startCol[i].z = Random::ranged(m_minStartCol.z, m_maxStartCol.z);
        p->m_startCol[i].w = Random::ranged(m_minStartCol.w, m_maxStartCol.w);

        p->m_endCol[i].x = Random::ranged(m_minEndCol.x, m_maxEndCol.x);
        p->m_endCol[i].y = Random::ranged(m_minEndCol.y, m_maxEndCol.y);
        p->m_endCol[i].z = Random::ranged(m_minEndCol.z, m_maxEndCol.z);
        p->m_endCol[i].w = Random::ranged(m_minEndCol.w, m_maxEndCol.w);
    }
}

uint16_t BasicColorGen::getRequiereConfig() const
{
    return ParticleData::EParam::END_COLOR | ParticleData::EParam::START_COLOR;
}

void BasicVelGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        p->m_vel[i].x = Random::ranged(m_minStartVel.x, m_maxStartVel.x);
        p->m_vel[i].y = Random::ranged(m_minStartVel.y, m_maxStartVel.y);
        p->m_vel[i].z = Random::ranged(m_minStartVel.z, m_maxStartVel.z);
        p->m_vel[i].w = Random::ranged(m_minStartVel.w, m_maxStartVel.w);
    }
}

uint16_t BasicVelGen::getRequiereConfig() const
{
    return ParticleData::EParam::VELOCITY;
}

void SphereVelGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
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

uint16_t SphereVelGen::getRequiereConfig() const
{
    return ParticleData::EParam::VELOCITY;
}

void VelFromPosGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        float scale = Random::ranged(m_minScale, m_maxScale);
        Vec4  vel   = (p->m_pos[i] - m_offset);
        p->m_vel[i] = vel * scale;
    }
}

uint16_t VelFromPosGen::getRequiereConfig() const
{
    return ParticleData::EParam::VELOCITY | ParticleData::EParam::POSITION;
}

void BasicTimeGen::generate(double dt, ParticleData* p, size_t startId, size_t endId)
{
    for (size_t i = startId; i < endId; ++i)
    {
        p->m_time[i].x = p->m_time[i].y = Random::ranged(m_minTime, m_maxTime);
        p->m_time[i].z                  = 0.f;
        p->m_time[i].w                  = 1.f / p->m_time[i].x;
    }
}

uint16_t BasicTimeGen::getRequiereConfig() const
{
    return ParticleData::EParam::TIME;
}