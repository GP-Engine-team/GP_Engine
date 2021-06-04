#include <LootManager.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <gpm/Calc.hpp>
#include <gpm/Random.hpp>
#include <gpm/Vector3.hpp>

#include <Generated/LootManager.rfk.h>
File_GENERATED

    using namespace GPE;
using namespace GPG;
using namespace GPM;

LootManager::LootManager(GameObject& gameObject) : BehaviourComponent(gameObject)
{
    onPostLoad();
}

void LootManager::onPostLoad()
{
    enableUpdateEditor(true);

    BehaviourComponent::onPostLoad();
}

void LootManager::start()
{
    GAME_ASSERT(m_lootsContainer.pData, "No loot container");
    GAME_ASSERT(m_lootPoints.size(), "No loot point");
    GAME_ASSERT(lootPrefab, "Missing loot prefab");

    std::vector<LootSpawnPoint> spawnerUnused = m_lootPoints;

    for (size_t i = 0; i < m_lootNumbers; ++i)
    {
        unsigned int indexLootPoint = Random::ranged<int>(spawnerUnused.size());
        GAME_ASSERT(spawnerUnused[indexLootPoint].go.pData, "null");

        GameObject* spawnedEntity = lootPrefab->clone(*m_lootsContainer.pData);

        spawnedEntity->getTransform().setTranslation(
            spawnerUnused[indexLootPoint].go.pData->getTransform().getGlobalPosition());

        spawnerUnused.erase(spawnerUnused.begin() + indexLootPoint);
    }
}

void LootManager::updateEditor(double deltaTime)
{
    if (!m_debug)
        return;

    for (auto&& elem : m_lootPoints)
    {
        GAME_ASSERT(elem.go.pData, "null");

        if (elem.go.pData)
            drawDebugSphere(elem.go.pData->getTransform().getGlobalPosition(), 10.f, RGBA{RGB::yellow(), 0.2f}, 0.f,
                            true);
    }
}
