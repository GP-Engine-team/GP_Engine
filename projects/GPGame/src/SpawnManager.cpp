#include <SpawnManager.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <gpm/Random.hpp>
#include <gpm/Vector3.hpp>

#include <Generated/SpawnManager.rfk.h>
File_GENERATED

    using namespace GPE;
using namespace GPG;
using namespace GPM;

SpawnManager::SpawnManager(GameObject& gameObject) : BehaviourComponent(gameObject)
{
    enableUpdate(true);
}

void SpawnManager::onPostLoad()
{
    enableUpdate(true);
    BehaviourComponent::onPostLoad();
}

void SpawnManager::start()
{
    GAME_ASSERT(m_spawners.empty(), "Empty buffer");
    GAME_ASSERT(m_entitiesToSpawnInfo.empty(), "Empty buffer");

    m_player = Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(m_player, "Player not found");

    GAME_ASSERT(m_enemiesContainer.pGo, "Missing container ref in SpawnManager");
    m_nextDelay = m_spawnDelay + Random::ranged(-m_spawnDelayInterval, m_spawnDelayInterval);

    GAME_ASSERT(!m_entitiesToSpawnInfo.empty(), "Spawner without info");
    GAME_ASSERT(!m_spawners.empty(), "SpawnerManager without spawner");

    for (auto&& elem : m_spawners)
    {
        GAME_ASSERT(elem.go.pGo, "null");
    }
}

void SpawnManager::update(double deltaTime)
{
    m_delayCount += deltaTime;

    while (m_delayCount >= m_nextDelay)
    {

        m_delayCount -= m_nextDelay;
        m_nextDelay     = m_spawnDelay + Random::ranged(-m_spawnDelayInterval, m_spawnDelayInterval);
        Vec2 position2D = Random::circularCoordinate(
            {m_gameObject->getTransform().getGlobalPosition().x, m_gameObject->getTransform().getGlobalPosition().z},
            m_zoneRadius);

        /*Choose random entity*/
        unsigned int indexEntityToSpawn = Random::ranged<int>(m_entitiesToSpawnInfo.size() - 1);

        /*Choose spawner*/
        unsigned int spawnerIndex    = Random::ranged<int>(m_spawners.size() - 1);
        const Vec3   spawnerPosition = m_spawners[spawnerIndex].go.pGo->getTransform().getGlobalPosition();
        const Vec3   newPosition     = {position2D.x + spawnerPosition.x, spawnerPosition.y,
                                  position2D.y + spawnerPosition.z};

        /*Spawn this entity*/
        GameObject* spawnedEntity = m_entitiesToSpawnInfo[indexEntityToSpawn].prefab->clone(*m_enemiesContainer.pGo);
        spawnedEntity->getTransform().setTranslation(newPosition);
    }
}