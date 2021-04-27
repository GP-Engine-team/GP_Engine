#include "Engine/Resources/ParticleSystem/ParticleUpdater.hpp"
#include <algorithm>
#include <assert.h>

using namespace GPE;
using namespace GPM;

void EulerUpdater::update(double dt, ParticleData* p)
{
    const Vec4  globalA(dt * m_globalAcceleration.x, dt * m_globalAcceleration.y, dt * m_globalAcceleration.z, 0.f);
    const float localDT = (float)dt;

    Vec4* acc = p->m_acc.get();
    Vec4* vel = p->m_vel.get();
    Vec4* pos = p->m_pos.get();

    const unsigned int endId = p->m_countAlive;
    for (size_t i = 0; i < endId; ++i)
    {
        acc[i] += globalA;
        vel[i] += acc[i] * localDT;
        pos[i] += vel[i] * localDT;
    }
}

uint8_t EulerUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::ACCELERATION | ParticleData::EParam::VELOCITY | ParticleData::EParam::POSITION;
}

void FloorUpdater::update(double dt, ParticleData* p)
{
    const float localDT = (float)dt;

    Vec4* acc = p->m_acc.get();
    Vec4* vel = p->m_vel.get();
    Vec4* pos = p->m_pos.get();

    const size_t endId = p->m_countAlive;
    for (size_t i = 0; i < endId; ++i)
    {
        if (pos[i].y < m_floorY)
        {
            Vec4  force        = p->m_acc[i];
            float normalFactor = force.dot(Vec4(0.0f, 1.0f, 0.0f, 0.0f));
            if (normalFactor < 0.0f)
                force -= Vec4(0.0f, 1.0f, 0.0f, 0.0f) * normalFactor;

            float velFactor = vel[i].dot(Vec4(0.0f, 1.0f, 0.0f, 0.0f));
            // if (velFactor < 0.0)
            vel[i] -= Vec4(0.0f, 1.0f, 0.0f, 0.0f) * (1.0f + m_bounceFactor) * velFactor;

            acc[i] = force;
        }
    }
}

uint8_t FloorUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::ACCELERATION | ParticleData::EParam::VELOCITY | ParticleData::EParam::POSITION;
}

void AttractorUpdater::update(double dt, ParticleData* p)
{
    const float localDT = (float)dt;

    Vec4* acc = p->m_acc.get();
    Vec4* vel = p->m_vel.get();
    Vec4* pos = p->m_pos.get();

    const size_t endId           = p->m_countAlive;
    const size_t countAttractors = m_attractors.size();
    Vec4         off;
    float        dist;
    size_t       a;
    for (size_t i = 0; i < endId; ++i)
    {
        for (a = 0; a < countAttractors; ++a)
        {
            off.x = m_attractors[a].x - pos[i].x;
            off.y = m_attractors[a].y - pos[i].y;
            off.z = m_attractors[a].z - pos[i].z;
            dist  = off.dot(off);

            // if (fabs(dist) > 0.00001)
            dist = m_attractors[a].w / dist;

            acc[i] += off * dist;
        }
    }
}

uint8_t AttractorUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::ACCELERATION | ParticleData::EParam::VELOCITY | ParticleData::EParam::POSITION;
}

void BasicColorUpdater::update(double dt, ParticleData* p)
{
    Vec4* col      = p->m_col.get();
    Vec4* startCol = p->m_startCol.get();
    Vec4* endCol   = p->m_endCol.get();
    Vec4* t        = p->m_time.get();

    const size_t endId = p->m_countAlive;
    for (size_t i = 0; i < endId; ++i)
        col[i] = startCol[i].lerp(endCol[i], t[i].z);
}

uint8_t BasicColorUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::COLOR | ParticleData::EParam::START_COLOR | ParticleData::EParam::END_COLOR |
           ParticleData::EParam::TIME;
}

void PosColorUpdater::update(double dt, ParticleData* p)
{
    Vec4* col      = p->m_col.get();
    Vec4* startCol = p->m_startCol.get();
    Vec4* endCol   = p->m_endCol.get();
    Vec4* t        = p->m_time.get();
    Vec4* pos      = p->m_pos.get();

    const int endId = (int)p->m_countAlive;
    float     scaler, scaleg, scaleb;
    float     diffr = m_maxPos.x - m_minPos.x;
    float     diffg = m_maxPos.y - m_minPos.y;
    float     diffb = m_maxPos.z - m_minPos.z;

    for (int i = 0; i < endId; ++i)
    {
        scaler   = (pos[i].x - m_minPos.x) / diffr;
        scaleg   = (pos[i].y - m_minPos.y) / diffg;
        scaleb   = (pos[i].z - m_minPos.z) / diffb;
        col[i].x = scaler; // glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
        col[i].y = scaleg; // glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
        col[i].z = scaleb; // glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
        col[i].w = 1.f;    // glm::mix(startCol[i].a, endCol[i].a, t[i].z);
    }
}

uint8_t PosColorUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::COLOR | ParticleData::EParam::START_COLOR | ParticleData::EParam::END_COLOR |
           ParticleData::EParam::TIME | ParticleData::EParam::POSITION;
}

void VelColorUpdater::update(double dt, ParticleData* p)
{
    Vec4* col      = p->m_col.get();
    Vec4* startCol = p->m_startCol.get();
    Vec4* endCol   = p->m_endCol.get();
    Vec4* t        = p->m_time.get();
    Vec4* vel      = p->m_vel.get();

    const size_t endId = p->m_countAlive;
    float        scaler, scaleg, scaleb;
    float        diffr = m_maxVel.x - m_minVel.x;
    float        diffg = m_maxVel.y - m_minVel.y;
    float        diffb = m_maxVel.z - m_minVel.z;
    for (size_t i = 0; i < endId; ++i)
    {
        scaler   = (vel[i].x - m_minVel.x) / diffr;
        scaleg   = (vel[i].y - m_minVel.y) / diffg;
        scaleb   = (vel[i].z - m_minVel.z) / diffb;
        col[i].x = scaler; // glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
        col[i].y = scaleg; // glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
        col[i].z = scaleb; // glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
        col[i].w = 1.f;    // glm::mix(startCol[i].a, endCol[i].a, t[i].z);
    }
}

uint8_t PosColorUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::COLOR | ParticleData::EParam::START_COLOR | ParticleData::EParam::END_COLOR |
           ParticleData::EParam::TIME | ParticleData::EParam::VELOCITY;
}

void BasicTimeUpdater::update(double dt, ParticleData* p)
{
    unsigned int endId   = p->m_countAlive;
    const float  localDT = (float)dt;

    Vec4* t = p->m_time.get();

    if (endId == 0)
        return;

    for (size_t i = 0; i < endId; ++i)
    {
        t[i].x -= localDT;
        // interpolation: from 0 (start of life) till 1 (end of life)
        t[i].z = (float)1.0 - (t[i].x * t[i].w); // .w is 1.0/max life time

        if (t[i].x < (float)0.0)
        {
            p->kill(i);
            endId = p->m_countAlive < p->m_count ? p->m_countAlive : p->m_count;
        }
    }
}

uint8_t BasicTimeUpdater::getRequiereConfig() const
{
    return ParticleData::EParam::TIME;
}