namespace GPE
{
size_t ParticleComponent::numAllParticles() const
{
    return m_particles.m_count;
}

size_t ParticleComponent::numAliveParticles() const
{
    return m_particles.m_countAlive;
}

const ParticleData* ParticleComponent::getData() const
{
    return &m_particles;
}

unsigned int ParticleComponent::getMeshID()
{
    return m_renderer->getID();
}

Shader* ParticleComponent::getShader()
{
    return m_shader;
}

Texture* ParticleComponent::getTexture()
{
    return m_diffuseTexture;
}

template <typename T, typename... TArg>
void ParticleComponent::addUpdater(TArg&&... arg)
{
    for (auto&& up : m_updaters)
    {
        if (dynamic_cast<T*>(up.get())) // if Already exist
            return;
    }
    m_updaters.emplace_back(arg...);
}

template <typename T>
T* ParticleComponent::getUpdater()
{
    T* rst = nullptr;
    for (auto&& up : m_updaters)
    {
        if (rst = dynamic_cast<T*>(up.get()))
            return rst;
    }
    return rst;
}

template <typename T>
void ParticleComponent::removeUpdater(T& updaterToRemove)
{
    for (auto&& it = m_updaters.begin(); it != m_updaters.end(); ++it)
    {
        if (it->get() == &updaterToRemove)
        {
            m_updaters.erase(it);
            return;
        }
    }
}

template <typename T, typename... TArg>
void ParticleComponent::addGenerator(TArg&&... arg)
{
    for (auto&& generator : m_generators)
    {
        if (dynamic_cast<T*>(generator.get())) // Already exist
            return;
    }
    m_generators.emplace_back(arg...);
}

template <typename T>
void ParticleComponent::removeGenerator(T& genToRemove)
{
    for (auto&& it = m_generators.begin(); it != m_generators.end(); ++it)
    {
        if (it->get() == &genToRemove)
        {
            m_generators.erase(it);
            return;
        }
    }
}

template <typename T>
T* ParticleComponent::getGenerator()
{
    T* rst = nullptr;
    for (auto&& up : m_generators)
    {
        if (rst = dynamic_cast<T*>(up.get()))
            return rst;
    }
    return rst;
}
} // namespace GPE