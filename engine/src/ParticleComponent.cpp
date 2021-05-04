#include <Engine/ECS/Component/ParticleComponent.hpp>
File_GENERATED

#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Resources/Shader.hpp>

#include <filesystem>
#include <imgui.h>

using namespace GPE;
using namespace GPM;

ParticleComponent::ParticleComponent(GameObject& owner) : Component(owner)
{
    owner.pOwnerScene->sceneRenderer.addParticleComponent(*this);
    initializeDefaultSetting();
}

ParticleComponent::ParticleComponent(GameObject& owner, const CreateArg& arg) : Component(owner)
{
    owner.pOwnerScene->sceneRenderer.addParticleComponent(*this);
    initializeDefaultSetting();
}

ParticleComponent::~ParticleComponent()
{
    getOwner().pOwnerScene->sceneRenderer.removeParticleComponent(*this);
}

void ParticleComponent::moveTowardScene(Scene& newOwner)
{
}

template <typename T>
void renderResourceExplorer(const char* name, T*& inRes)
{
    auto& resourceContainer = GPE::Engine::getInstance()->resourceManager.getAll<T>();

    std::vector<const char*> items;
    items.reserve(resourceContainer.size());

    for (auto&& res : resourceContainer)
    {
        items.emplace_back(res.first.c_str());
    }

    // Init position of the combo box cursor
    int itemCurrent;
    if (inRes == nullptr)
    {
        itemCurrent = -1;
    }
    else
    {
        itemCurrent = 0;
        for (auto&& it = resourceContainer.begin(); &it->second != inRes; ++itemCurrent, ++it)
            ;
    }

    if (ImGui::Combo(name, &itemCurrent, items.data(), int(items.size())))
    {
        auto&& it = resourceContainer.begin();
        for (int i = 0; i < itemCurrent; ++i, ++it)
            ;

        inRes = &it->second;
    }
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

    DataInspector::inspect(context, m_isInGlobalPosition, "Is in Global Position");

    ImGui::PushEnabled(!isDurationUsed);
    DataInspector::inspect(context, m_emitRate, "EmitRate");
    ImGui::PopEnabled();

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

    if (DataInspector::inspect(context, m_count, "Count"))
    {
        m_particles.generate(m_count, m_particles.m_maskType);
    }

    ImGui::TextUnformatted("Generator");
    PARTICLE_GENERATOR_INSPECT(BoxPosGen)
    PARTICLE_GENERATOR_INSPECT(RoundPosGen)
    PARTICLE_GENERATOR_INSPECT(BasicColorGen)
    PARTICLE_GENERATOR_INSPECT(BasicVelGen)
    PARTICLE_GENERATOR_INSPECT(SphereVelGen)
    PARTICLE_GENERATOR_INSPECT(VelFromPosGen)
    PARTICLE_GENERATOR_INSPECT(BasicTimeGen)

    ImGui::TextUnformatted("Updater");
    PARTICLE_UPDATER_INSPECT(EulerUpdater)
    PARTICLE_UPDATER_INSPECT(FloorUpdater)
    PARTICLE_UPDATER_INSPECT(AttractorUpdater)
    PARTICLE_UPDATER_INSPECT(BasicColorUpdater)
    PARTICLE_UPDATER_INSPECT(PosColorUpdater)
    PARTICLE_UPDATER_INSPECT(VelColorUpdater)
    PARTICLE_UPDATER_INSPECT(BasicTimeUpdater)

    // Shader
    {
        renderResourceExplorer<Shader>("Shader", m_shader);

        // Drop
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_SHADER_EXTENSION))
            {
                IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

                if (Shader* pShader = Engine::getInstance()->resourceManager.get<Shader>(path.string().c_str()))
                {
                    m_shader = pShader;
                }
                else
                {
                    if (const std::string* str = Engine::getInstance()->resourceManager.getKey(m_shader))
                        getOwner().pOwnerScene->removeLoadedResourcePath(str->c_str());

                    m_shader = loadShaderFile(path.string().c_str());
                    getOwner().pOwnerScene->addLoadedResourcePath(path.string().c_str());
                }
            }
        }
    }

    if (ImGui::Button("Start"))
    {
        generate();
        start();
    }
}

void ParticleComponent::generate()
{
    if (!std::isinf(m_duration))
        m_emitRate = m_count / m_duration;

    m_particles.generate(m_count, 0);

    initializeRenderer();
}

void ParticleComponent::start()
{
    reset();
    m_canEmit = true;
}

void ParticleComponent::emit(unsigned int count)
{
    const size_t startId = m_particles.m_countAlive;
    const size_t endId   = std::min(startId + count, m_particles.m_count - 1);

    for (auto& gen : m_generators) // << gen loop
        gen->generate(&m_particles, startId, endId);

    if (m_isInGlobalPosition)
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
        m_posGenerator->m_pos               = Vec4{.0f, .0f, .0f, .0f};
        m_posGenerator->m_maxStartPosOffset = Vec4{.0f, .0f, .0f, .0f};
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
        m_durationCount += dt * !std::isinf(m_duration);
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
        for (size_t i = 0; i < m_count; ++i)
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
    const size_t endId           = std::min(startId + maxNewParticles, m_particles.m_count - 1);

    for (auto& gen : m_generators) // << gen loop
        gen->generate(&m_particles, startId, endId);

    if (m_isInGlobalPosition)
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

    for (size_t i = startId; i < endId; ++i) // << wake loop
        m_particles.wake(i);
}

void ParticleComponent::setActive(bool newState) noexcept
{
    if (m_isActivated == newState)
        return;

    m_isActivated = newState;
    if (m_isActivated)
    {
        getOwner().pOwnerScene->sceneRenderer.addParticleComponent(*this);
    }
    else
    {

        getOwner().pOwnerScene->sceneRenderer.removeParticleComponent(*this);
    }
}