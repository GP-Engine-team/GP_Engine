#include <Engine/ECS/Component/ParticleComponent.hpp>
File_GENERATED

#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Resources/Shader.hpp>
#include <GPM/Matrix3.hpp>
#include <GPM/conversion.hpp>

#include <filesystem>
#include <imgui.h>

    using namespace GPE;
using namespace GPM;

ParticleComponent::ParticleComponent(GameObject& owner) : Component(owner)
{
    updateToSystem();
    initializeDefaultSetting();
}

ParticleComponent::ParticleComponent() : Component()
{
    initializeDefaultSetting();
}

ParticleComponent::ParticleComponent(GameObject& owner, const CreateArg& arg) : Component(owner)
{
    updateToSystem();
    initializeDefaultSetting();
}

ParticleComponent::~ParticleComponent()
{
    setActive(false);
}

void ParticleComponent::moveTowardScene(Scene& newOwner)
{
}

#define PARTICLE_GENERATOR_INSPECT(name)                                                                               \
    {                                                                                                                  \
        name* generator = getGenerator<name>();                                                                        \
        bool  flag      = generator;                                                                                   \
                                                                                                                       \
        if (ImGui::Checkbox("##" #name, &flag))                                                                        \
        {                                                                                                              \
            if (flag)                                                                                                  \
            {                                                                                                          \
                generator = static_cast<name*>(m_generators.emplace_back(std::make_unique<name>()).get());             \
                m_particles.generate(m_count, generator->getRequiereConfig().get() | m_particles.m_maskType.get());    \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                removeGenerator(*generator);                                                                           \
                ImGui::SetNextItemOpen(false);                                                                         \
            }                                                                                                          \
        }                                                                                                              \
        ImGui::SameLine();                                                                                             \
        ImGui::PushEnabled(flag);                                                                                      \
        if (ImGui::CollapsingHeader(#name))                                                                            \
        {                                                                                                              \
            DataInspector::inspect(context, *generator);                                                               \
        }                                                                                                              \
        ImGui::PopEnabled();                                                                                           \
    }

#define PARTICLE_UPDATER_INSPECT(name)                                                                                 \
    {                                                                                                                  \
        name* updater = getUpdater<name>();                                                                            \
        bool  flag    = updater;                                                                                       \
                                                                                                                       \
        if (ImGui::Checkbox("##" #name, &flag))                                                                        \
        {                                                                                                              \
            if (flag)                                                                                                  \
            {                                                                                                          \
                updater = static_cast<name*>(m_updaters.emplace_back(std::make_unique<name>()).get());                 \
                m_particles.generate(m_count, updater->getRequiereConfig().get() | m_particles.m_maskType.get());      \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                removeUpdater(*updater);                                                                               \
                ImGui::SetNextItemOpen(false);                                                                         \
            }                                                                                                          \
        }                                                                                                              \
        ImGui::SameLine();                                                                                             \
        ImGui::PushEnabled(flag);                                                                                      \
        if (ImGui::CollapsingHeader(#name))                                                                            \
        {                                                                                                              \
            DataInspector::inspect(context, *updater);                                                                 \
        }                                                                                                              \
        ImGui::PopEnabled();                                                                                           \
    }

void ParticleComponent::inspect(InspectContext& context)
{
    Component::inspect(context);

    bool isDurationUsed = !std::isinf(m_duration);

    ImGui::PushEnabled(!m_useGlobalPosition);
    DataInspector::inspect(context, m_useGameObjectTransform, "Use transform");
    ImGui::PopEnabled();

    ImGui::PushEnabled(!m_useGameObjectTransform);
    DataInspector::inspect(context, m_useGlobalPosition, "Use global position");
    ImGui::PopEnabled();

    DataInspector::inspect(context, isTransparente, "Is transparent");

    DataInspector::inspect(context, m_emitRate, "EmitRate");

    if (ImGui::Checkbox("##isDurationUsed", &isDurationUsed))
    {
        if (isDurationUsed)
            m_duration = 3.0f;
        else
            m_duration = std::numeric_limits<float>::infinity();
    }
    ImGui::SameLine();
    ImGui::PushEnabled(isDurationUsed);
    DataInspector::inspect(context, m_duration, "Duration");
    ImGui::PopEnabled();

    DataInspector::inspect(context, m_count, "Count");
    if (context.wasLastDirty())
    {
        m_particles.generate(m_count, m_particles.m_maskType);
    }

    ImGui::TextUnformatted("Generator");
    PARTICLE_GENERATOR_INSPECT(BoxPosGen)
    PARTICLE_GENERATOR_INSPECT(RoundPosGen)
    PARTICLE_GENERATOR_INSPECT(SizeGen)
    PARTICLE_GENERATOR_INSPECT(BasicColorGen)
    PARTICLE_GENERATOR_INSPECT(BasicVelGen)
    PARTICLE_GENERATOR_INSPECT(ConeVelGen)
    PARTICLE_GENERATOR_INSPECT(SphereVelGen)
    PARTICLE_GENERATOR_INSPECT(VelFromPosGen)
    PARTICLE_GENERATOR_INSPECT(BasicTimeGen)

    ImGui::TextUnformatted("Updater");
    PARTICLE_UPDATER_INSPECT(EulerUpdater)
    PARTICLE_UPDATER_INSPECT(SizeUpdater)
    PARTICLE_UPDATER_INSPECT(FloorUpdater)
    PARTICLE_UPDATER_INSPECT(AttractorUpdater)
    PARTICLE_UPDATER_INSPECT(BasicColorUpdater)
    PARTICLE_UPDATER_INSPECT(PosColorUpdater)
    PARTICLE_UPDATER_INSPECT(VelColorUpdater)
    PARTICLE_UPDATER_INSPECT(BasicTimeUpdater)

    // Shader
    DataInspector::inspect(context, m_shader, "Shader");
    DataInspector::inspect(context, m_diffuseTexture, "Optinal Texture");

    if (ImGui::Button("Start"))
    {
        start();
    }

    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        reset();
    }
}

void ParticleComponent::generate()
{
    if (!std::isinf(m_duration))
        m_emitRate = m_count / m_duration;

    U16BMask mask;
    for (auto&& updater : m_updaters)
        mask.add(updater->getRequiereConfig().get());

    for (auto&& generator : m_generators)
        mask.add(generator->getRequiereConfig().get());

    m_particles.generate(m_count, mask);

    initializeRenderer();
}

void ParticleComponent::start()
{
    m_durationCount          = 0.f;
    m_particles.m_countAlive = 0;
    m_canEmit                = false;

    if (!m_renderer)
    {
        generate();
    }
    else
    {
        for (size_t i = 0; i < m_count; ++i)
            m_particles.m_alive[i] = false;
    }

    m_canEmit = true;
}

void ParticleComponent::emit(unsigned int count)
{
    if (m_useGameObjectTransform)
    {
        emitAt(getOwner().getTransform().getModelMatrix(), count);
    }
    else if (m_useGlobalPosition)
    {
        emitAt(getOwner().getTransform().getGlobalPosition(), count);
    }
}

void ParticleComponent::emitAt(const GPM::Vec3& position, unsigned int count)
{
    if (!m_renderer)
        generate();

    const size_t startId = m_particles.m_countAlive;
    const size_t endId   = std::min(startId + count, m_particles.m_count - 1);

    for (auto& gen : m_generators) // << gen loop
        gen->generate(&m_particles, startId, endId);

    if (m_particles.m_pos)
        for (size_t i = startId; i < endId; ++i) // << set GlobalPosition loop
            m_particles.m_pos[i] = Vec4{position, 0.f} + m_particles.m_pos[i];

    for (size_t i = startId; i < endId; ++i) // << wake loop
        m_particles.wake(i);
}

void ParticleComponent::emitAt(const GPM::Mat4& model, unsigned int count)
{
    if (!m_renderer)
        generate();

    const size_t startId = m_particles.m_countAlive;
    const size_t endId   = std::min(startId + count, m_particles.m_count - 1);

    for (auto& gen : m_generators) // << gen loop
        gen->generate(&m_particles, startId, endId);

    if (m_particles.m_pos)
        for (size_t i = startId; i < endId; ++i) // << set GlobalPosition loop
            m_particles.m_pos[i] = model * m_particles.m_pos[i];

    if (m_particles.m_vel)
    {
        Mat3 rotScaleMat = toMatrix3(model);
        for (size_t i = startId; i < endId; ++i) // << set GlobalPosition loop
            m_particles.m_vel[i].xyz = rotScaleMat * m_particles.m_vel[i].xyz;
    }

    for (size_t i = startId; i < endId; ++i) // << wake loop
        m_particles.wake(i);
}

void ParticleComponent::initializeDefaultSetting()
{
    m_count    = 10000;
    m_emitRate = (float)m_count * 0.25f;

    {
        // pos:
        auto m_posGenerator                 = std::make_unique<BoxPosGen>();
        m_posGenerator->m_pos               = Vec3{.0f, .0f, .0f};
        m_posGenerator->m_maxStartPosOffset = Vec3{.0f, .0f, .0f};
        m_generators.emplace_back(std::move(m_posGenerator));

        auto m_colGenerator           = std::make_unique<BasicColorGen>();
        m_colGenerator->m_minStartCol = RGBA{0.7f, 0.7f, 0.7f, 1.0f};
        m_colGenerator->m_maxStartCol = RGBA{1.0f, 1.0f, 1.0f, 1.0f};
        m_colGenerator->m_minEndCol   = RGBA{0.5f, 0.0f, 0.6f, 0.0f};
        m_colGenerator->m_maxEndCol   = RGBA{0.7f, 0.5f, 1.0f, 0.0f};
        m_generators.emplace_back(std::move(m_colGenerator));

        auto velGenerator           = std::make_unique<BasicVelGen>();
        velGenerator->m_minStartVel = Vec4{0.f, 0.f, -50.f, 0.0f};
        velGenerator->m_maxStartVel = Vec4{25.f, 25.f, 5.f, 0.0f};
        m_generators.emplace_back(std::move(velGenerator));

        auto timeGenerator       = std::make_unique<BasicTimeGen>();
        timeGenerator->m_minTime = 3.0f;
        timeGenerator->m_maxTime = 4.0f;
        m_generators.emplace_back(std::move(timeGenerator));
    }

    auto timeUpdater = std::make_unique<BasicTimeUpdater>();
    m_updaters.emplace_back(std::move(timeUpdater));

    auto colorUpdater = std::make_unique<BasicColorUpdater>();
    m_updaters.emplace_back(std::move(colorUpdater));

    auto m_eulerUpdater                  = std::make_unique<EulerUpdater>();
    m_eulerUpdater->m_globalAcceleration = Vec4{0.0f, -15.0f, 0.0f, 0.0f};
    m_updaters.emplace_back(std::move(m_eulerUpdater));

    auto m_floorUpdater = std::make_unique<FloorUpdater>();
    m_updaters.emplace_back(std::move(m_floorUpdater));

    U16BMask mask;

    for (auto&& gen : m_generators)
        mask.add(gen->getRequiereConfig().get());

    for (auto&& up : m_updaters)
        mask.add(up->getRequiereConfig().get());

    m_particles.generate(m_count, mask);
}

void ParticleComponent::initializeRenderer()
{
    m_renderer = std::make_unique<ParticleRenderer>();
    m_renderer->generate(this, false);
}

void ParticleComponent::reset()
{
    m_durationCount          = 0.f;
    m_particles.m_countAlive = 0;
    m_canEmit                = false;

    for (size_t i = 0; i < m_count; ++i)
        m_particles.m_alive[i] = false;
}

void ParticleComponent::update(double dt)
{
    if (!m_renderer)
        return;

    if (m_canEmit)
    {
        m_durationCount += float(dt) * !std::isinf(m_duration);
        if (m_durationCount >= m_duration)
        {
            m_canEmit = false;
            dt -= m_durationCount - m_duration;
            m_durationCount = 0.f;
        }

        emit(dt);
    }

    // TODO: Add acceleration updater
    if (m_particles.m_acc)
    {
        for (size_t i = 0u; i < m_count; ++i)
        {
            m_particles.m_acc[i] = Vec4(0.0f);
        }
    }

    for (auto& up : m_updaters)
    {
        up->update(dt, &m_particles);
    }

    m_renderer->update();
}

void ParticleComponent::emit(double dt)
{
    const size_t maxNewParticles = static_cast<size_t>(dt * m_emitRate);
    const size_t startId         = m_particles.m_countAlive;
    const size_t endId           = std::min(startId + maxNewParticles, m_particles.m_count - 1u);

    for (auto& gen : m_generators) // << gen loop
        gen->generate(&m_particles, startId, endId);

    if (m_useGameObjectTransform)
    {
        if (m_particles.m_pos)
            for (size_t i = startId; i < endId; ++i) // << set GlobalPosition loop
                m_particles.m_pos[i] = getOwner().getTransform().getModelMatrix() * m_particles.m_pos[i];

        if (m_particles.m_vel)
        {
            Mat3 rotScaleMat = toMatrix3(getOwner().getTransform().getModelMatrix());
            for (size_t i = startId; i < endId; ++i) // << set GlobalPosition loop
                m_particles.m_vel[i].xyz = rotScaleMat * m_particles.m_vel[i].xyz;
        }
    }
    else if (m_useGlobalPosition)
    {
        if (m_particles.m_pos)
            for (size_t i = startId; i < endId; ++i) // << set GlobalPosition loop
                m_particles.m_pos[i] = Vec4{getOwner().getTransform().getGlobalPosition(), 0.f} + m_particles.m_pos[i];
    }

    for (size_t i = startId; i < endId; ++i) // << wake loop
        m_particles.wake(i);
}

void ParticleComponent::updateToSystem() noexcept
{
    if (m_isActivated)
    {
        getOwner().pOwnerScene->sceneRenderer.addParticleComponent(*this);
    }
    else
    {
        if (getOwner().pOwnerScene)
            getOwner().pOwnerScene->sceneRenderer.removeParticleComponent(*this);
    }
}