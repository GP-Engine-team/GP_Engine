#include <WorldGenerator.hpp>

#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/ResourcesManagerType.hpp>
#include <GPM/Random.hpp>

#include "Generated/WorldGenerator.rfk.h"
File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

void WorldGenerator::loadCircularCoordinate()
{
    const ResourceManagerType& rm = Engine::getInstance()->resourceManager;

    // Create object with random sizes,
    // positions and rotations and add them to the container
    for (unsigned int i = 0u; i < m_number; ++i)
    {
        GameObject* const newGO = m_prefab->clone(*m_container.pData);

        newGO->setName(("Object" + std::to_string(i)).c_str());

        const Vec2 pos2 = Random::circularCoordinate(Vec2::zero(), m_radius);
        newGO->getTransform().setTranslation(transform().getGlobalPosition() + Vec3{pos2.x, 0.f, pos2.y} -
                                             m_container.pData->getTransform().getGlobalPosition());

        Vec3 rotEuler = {Random::ranged<float>(m_minRot.x, m_maxRot.x), Random::ranged<float>(m_minRot.y, m_maxRot.y),
                         Random::ranged<float>(m_minRot.z, m_maxRot.z)};

        rotEuler *= PI / 180.f;
        newGO->getTransform().setRotation(Quat::fromEuler(rotEuler));

        newGO->getTransform().setScale({Random::ranged<float>(m_minScale.x, m_maxScale.x),
                                        Random::ranged<float>(m_minScale.y, m_maxScale.y),
                                        Random::ranged<float>(m_minScale.z, m_maxScale.z)});
    }
}

void WorldGenerator::onPostLoad()
{
    enableUpdateEditor(true);

    BehaviourComponent::onPostLoad();
}

void WorldGenerator::inspect(GPE::InspectContext& context)
{
    Component::inspect(context);

    DataInspector::inspect(context, m_prefab, "m_prefab");
    DataInspector::inspect(context, m_container, "m_container");
    ImGui::Separator();
    DataInspector::inspect(context, m_minScale, "m_minScale");
    DataInspector::inspect(context, m_maxScale, "m_maxScale");
    ImGui::Separator();
    DataInspector::inspect(context, m_minRot, "m_minRot");
    DataInspector::inspect(context, m_maxRot, "m_maxRot");
    ImGui::Separator();
    DataInspector::inspect(context, m_number, "m_number");
    DataInspector::inspect(context, m_radius, "m_radius");
    ImGui::Separator();
    DataInspector::inspect(context, m_debug, "m_debug");
    ImGui::Separator();

    ImGui::PushEnabled(m_prefab && m_container.pData);
    if (ImGui::Button("CircularCoordinate"))
    {
        loadCircularCoordinate();
    }
    ImGui::PopEnabled();

    ImGui::SameLine();

    ImGui::PushEnabled(m_container.pData);
    if (ImGui::Button("Clear"))
    {
        for (auto&& child : m_container.pData->children)
        {
            delete child;
        }
        m_container.pData->children.clear();
    }
    ImGui::PopEnabled();
}

void WorldGenerator::updateEditor(double deltaTime)
{
    if (!m_debug || !m_container.pData || !m_prefab)
        return;

    drawDebugSphere(transform().getGlobalPosition(), m_radius, RGBA{RGB::green(), 0.05f}, 0.f, true);
}