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
}

ParticleComponent::ParticleComponent(GameObject& owner, const CreateArg& arg) : Component(owner)
{
    owner.pOwnerScene->sceneRenderer.addParticleComponent(*this);
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

    if (ImGui::Combo(name, &itemCurrent, items.data(), items.size()))
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
                m_particles.generate(m_count, generator->getRequiereConfig() | m_particles.m_maskType);                \
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
                updater = static_cast<name*>(m_updaters.emplace_back(std::make_shared<name>()).get());                 \
                m_particles.generate(m_count, updater->getRequiereConfig() | m_particles.m_maskType);                  \
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
    DataInspector::inspect(context, m_emitRate, "EmitRate");

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
        start();
    }
}

void ParticleComponent::start()
{
    initialize(m_count);
}

bool ParticleComponent::initialize(size_t numParticles)
{
    //
    // particles
    //
    m_count = numParticles;
    m_particles.generate(numParticles, 0);

    for (size_t i = 0; i < numParticles; ++i)
        m_particles.m_alive[i] = false;

    //
    // emitter:
    //
    // m_emitters = std::make_shared<ParticleEmitter>();
    //{
    //    m_emitters->m_emitRate = (float)numParticles * 0.25f;

    //    // pos:
    //    auto m_posGenerator                 = std::make_shared<BoxPosGen>();
    //    m_posGenerator->m_pos               = Vec4{0.0, 0.0, 0.0, 0.0};
    //    m_posGenerator->m_maxStartPosOffset = Vec4{0.0, 0.0, 0.0, 0.0};
    //    m_emitters->addGenerator(m_posGenerator);

    //    auto m_colGenerator           = std::make_shared<BasicColorGen>();
    //    m_colGenerator->m_minStartCol = Vec4{0.7, 0.7, 0.7, 1.0};
    //    m_colGenerator->m_maxStartCol = Vec4{1.0, 1.0, 1.0, 1.0};
    //    m_colGenerator->m_minEndCol   = Vec4{0.5, 0.0, 0.6, 0.0};
    //    m_colGenerator->m_maxEndCol   = Vec4{0.7, 0.5, 1.0, 0.0};
    //    m_emitters->addGenerator(m_colGenerator);

    //    auto velGenerator           = std::make_shared<BasicVelGen>();
    //    velGenerator->m_minStartVel = Vec4{-5.f, 2.2f, -5.f, 0.0f};
    //    velGenerator->m_maxStartVel = Vec4{50.f, 25.f, 5.f, 0.0f};
    //    m_emitters->addGenerator(velGenerator);

    //    auto timeGenerator       = std::make_shared<BasicTimeGen>();
    //    timeGenerator->m_minTime = 3.0f;
    //    timeGenerator->m_maxTime = 4.0f;
    //    m_emitters->addGenerator(timeGenerator);
    //}

    // auto timeUpdater = std::make_shared<BasicTimeUpdater>();
    // m_updaters.emplace_back(timeUpdater);

    // auto colorUpdater = std::make_shared<BasicColorUpdater>();
    // m_updaters.emplace_back(colorUpdater);

    // auto m_eulerUpdater                  = std::make_shared<EulerUpdater>();
    // m_eulerUpdater->m_globalAcceleration = Vec4{0.0, -15.0, 0.0, 0.0};
    // m_updaters.emplace_back(m_eulerUpdater);

    // auto m_floorUpdater = std::make_shared<FloorUpdater>();
    // m_updaters.emplace_back(m_floorUpdater);

    return initializeRenderer();
}

bool ParticleComponent::initializeRenderer()
{
    m_renderer = RendererFactory::create("gl");
    m_renderer->generate(this, false);

    return true;
}

void ParticleComponent::reset()
{
    m_particles.m_countAlive = 0;
}

void ParticleComponent::clean()
{
    if (m_renderer)
        m_renderer->destroy();

    m_updaters.clear();
}

void ParticleComponent::update(double dt)
{
    if (!m_renderer)
        return;

    emit(dt);

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
        gen->generate(dt, &m_particles, startId, endId);

    for (size_t i = startId; i < endId; ++i) // << wake loop
        m_particles.wake(i);
}