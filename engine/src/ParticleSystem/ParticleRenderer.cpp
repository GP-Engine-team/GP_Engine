#include <Engine/Resources/ParticleSystem/ParticleRenderer.hpp>

#include <Engine/ECS/Component/ParticleComponent.hpp>

using namespace GPE;

#include <algorithm>
#include <assert.h>
#include <glad/glad.h>

void ParticleRenderer::generate(ParticleComponent* sys, bool)
{
    assert(sys != nullptr);

    m_system = sys;

    const size_t count = sys->numAllParticles();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_bufPos);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * count, nullptr, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (4) * sizeof(float), (void*)((0) * sizeof(float)));

    glGenBuffers(1, &m_bufCol);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * count, nullptr, GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (4) * sizeof(float), (void*)((0) * sizeof(float)));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleRenderer::destroy()
{
    if (m_bufPos != 0)
    {
        glDeleteBuffers(1, &m_bufPos);
        m_bufPos = 0;
    }

    if (m_bufCol != 0)
    {
        glDeleteBuffers(1, &m_bufCol);
        m_bufCol = 0;
    }
}

void ParticleRenderer::update()
{
    assert(m_system != nullptr);
    assert(m_bufPos > 0 && m_bufCol > 0);

    const size_t count = m_system->numAliveParticles();

    if (count > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufPos);
        const float* ptr = (const float*)(m_system->getData()->m_pos.get());
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float) * 4, ptr);

        glBindBuffer(GL_ARRAY_BUFFER, m_bufCol);
        ptr = (const float*)(m_system->getData()->m_col.get());
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float) * 4, ptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}