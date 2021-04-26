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
    owner.pOwnerScene->sceneRenderer.addParticleComponent(this);
    initialize(1000000);
}

ParticleComponent::ParticleComponent(GameObject& owner, const CreateArg& arg) : Component(owner)
{
    owner.pOwnerScene->sceneRenderer.addParticleComponent(this);
    initialize(1000000);
}

ParticleComponent::~ParticleComponent()
{
    getOwner().pOwnerScene->sceneRenderer.removeParticleComponent(this);
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

void ParticleComponent::inspect(InspectContext& context)
{
    bool flag = m_particles.isParamEnable(ParticleData::EParam::POSITION);
    if (ImGui::Checkbox("##POSITION", &flag))
    {
        m_particles.invertParamState(ParticleData::EParam::POSITION);
    }
    ImGui::SameLine();
    ImGui::PushEnabled(flag);
    ImGui::CollapsingHeader("Position");
    ImGui::PopEnabled();

    flag = m_particles.isParamEnable(ParticleData::EParam::COLOR_INTERPOLATION);
    if (ImGui::Checkbox("##COLOR_INTERPOLATION", &flag))
    {
        m_particles.invertParamState(ParticleData::EParam::COLOR_INTERPOLATION);
    }
    ImGui::SameLine();
    ImGui::PushEnabled(flag);
    ImGui::CollapsingHeader("Color interpolation");
    ImGui::PopEnabled();

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

bool ParticleComponent::initialize(size_t numParticles)
{
    clean();

    //
    // particles
    //
    m_count = numParticles;
    m_particles.generate(numParticles, ParticleData::EParam::COLOR_INTERPOLATION | ParticleData::EParam::VELOCITY |
                                           ParticleData::EParam::POSITION | ParticleData::EParam::TIME);

    for (size_t i = 0; i < numParticles; ++i)
        m_particles.m_alive[i] = false;

    //
    // emitter:
    //
    auto particleEmitter = std::make_shared<ParticleEmitter>();
    {
        particleEmitter->m_emitRate = (float)numParticles * 0.25f;

        // pos:
        auto m_posGenerator                 = std::make_shared<BoxPosGen>();
        m_posGenerator->m_pos               = Vec4{0.0, 0.0, 0.0, 0.0};
        m_posGenerator->m_maxStartPosOffset = Vec4{0.0, 0.0, 0.0, 0.0};
        particleEmitter->addGenerator(m_posGenerator);

        auto m_colGenerator           = std::make_shared<BasicColorGen>();
        m_colGenerator->m_minStartCol = Vec4{0.7, 0.7, 0.7, 1.0};
        m_colGenerator->m_maxStartCol = Vec4{1.0, 1.0, 1.0, 1.0};
        m_colGenerator->m_minEndCol   = Vec4{0.5, 0.0, 0.6, 0.0};
        m_colGenerator->m_maxEndCol   = Vec4{0.7, 0.5, 1.0, 0.0};
        particleEmitter->addGenerator(m_colGenerator);

        auto velGenerator           = std::make_shared<BasicVelGen>();
        velGenerator->m_minStartVel = Vec4{-5.f, 2.2f, -5.f, 0.0f};
        velGenerator->m_maxStartVel = Vec4{50.f, 25.f, 5.f, 0.0f};
        particleEmitter->addGenerator(velGenerator);

        auto timeGenerator       = std::make_shared<BasicTimeGen>();
        timeGenerator->m_minTime = 3.0f;
        timeGenerator->m_maxTime = 4.0f;
        particleEmitter->addGenerator(timeGenerator);
    }
    m_emitters.emplace_back(particleEmitter);

    auto timeUpdater = std::make_shared<BasicTimeUpdater>();
    m_updaters.emplace_back(timeUpdater);

    auto colorUpdater = std::make_shared<BasicColorUpdater>();
    m_updaters.emplace_back(colorUpdater);

    auto m_eulerUpdater                  = std::make_shared<EulerUpdater>();
    m_eulerUpdater->m_globalAcceleration = Vec4{0.0, -15.0, 0.0, 0.0};
    m_updaters.emplace_back(m_eulerUpdater);

    auto m_floorUpdater = std::make_shared<FloorUpdater>();
    m_updaters.emplace_back(m_floorUpdater);

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

    m_emitters.clear();
    m_updaters.clear();
}

void ParticleComponent::update(double dt)
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

    m_renderer->update();
}