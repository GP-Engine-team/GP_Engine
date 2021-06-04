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

void WorldGenerator::loadDisk()
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

void WorldGenerator::loadCircularTowardCenter()
{
    const ResourceManagerType& rm = Engine::getInstance()->resourceManager;

    // Create object with random sizes,
    // positions and rotations and add them to the container
    float step = TWO_PI / m_number;
    for (unsigned int i = 0u; i < m_number; ++i)
    {
        GameObject* const newGO = m_prefab->clone(*m_container.pData);

        newGO->setName(("Object" + std::to_string(i)).c_str());

        Vec3 pos = Vec3{m_radius * cosf(i * step), 0.f, m_radius * sinf(i * step)};
        newGO->getTransform().setTranslation(transform().getGlobalPosition() + pos -
                                             m_container.pData->getTransform().getGlobalPosition());

        newGO->getTransform().setRotation(toQuaternion(Transform::lookAt(pos, transform().getGlobalPosition())));

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

    defaultInspect(context);

    ImGui::PushEnabled(m_prefab && m_container.pData);
    if (ImGui::Button("Disk"))
    {
        loadDisk();
    }

    if (ImGui::Button("Circular toward center"))
    {
        loadCircularTowardCenter();
    }
    ImGui::PopEnabled();

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