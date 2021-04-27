#include <Engine/Resources/ParticleSystem/ParticleEmitter.hpp>

using namespace GPE;

void ParticleEmitter::emit(double dt, ParticleData* p)
{
    const size_t maxNewParticles = static_cast<size_t>(dt * m_emitRate);
    const size_t startId         = p->m_countAlive;
    const size_t endId           = std::min(startId + maxNewParticles, p->m_count - 1);

    for (auto& gen : m_generators) // << gen loop
        gen->generate(dt, p, startId, endId);

    for (size_t i = startId; i < endId; ++i) // << wake loop
        p->wake(i);
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, ParticleEmitter& inspected)
{
    inspect(context, inspected.m_emitRate);
}