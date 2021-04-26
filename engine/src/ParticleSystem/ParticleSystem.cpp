#include <Engine/Resources/ParticleSystem/ParticleSystem.hpp>

#include <GPM/Vector3.hpp>

using namespace GPE;
using namespace GPM;

ParticleSystem::ParticleSystem(size_t maxCount)
{
    m_count = maxCount;
    m_particles.generate(maxCount, 0);

    for (size_t i = 0; i < maxCount; ++i)
        m_particles.m_alive[i] = false;
}

void ParticleSystem::update(double dt)
{
    for (auto& em : m_emitters)
    {
        em->emit(dt, &m_particles);
    }

    for (size_t i = 0; i < m_count; ++i)
    {
        m_particles.m_acc[i] = Vec4(0.0f);
    }

    for (auto& up : m_updaters)
    {
        up->update(dt, &m_particles);
    }
}

void ParticleSystem::reset()
{
    m_particles.m_countAlive = 0;
}