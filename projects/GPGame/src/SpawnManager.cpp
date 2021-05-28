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
    onPostLoad();
}

void SpawnManager::onPostLoad()
{
    enableUpdate(true);
    enableUpdateEditor(true);

    m_player = Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(m_player, "Player not found");

    BehaviourComponent::onPostLoad();
}

void SpawnManager::start()
{
    GAME_ASSERT(m_enemiesContainer.pGo, "Missing container ref in SpawnManager");
    m_nextDelay = m_spawnDelay + Random::ranged(-m_spawnDelayInterval, m_spawnDelayInterval);

    GAME_ASSERT(m_entitiesToSpawnInfo.size(), "Spawner without info");
    GAME_ASSERT(m_spawners.size(), "SpawnerManager without spawner");

    for (auto&& elem : m_spawners)
    {
        GAME_ASSERT(elem.go.pGo, "null");
    }

    sqrTotalRadius = (m_spawnerZoneRadius + m_playerZoneRadius) * (m_spawnerZoneRadius + m_playerZoneRadius);
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
            m_spawnerZoneRadius);

        /*Choose random entity*/
        unsigned int indexEntityToSpawn = Random::ranged<int>(m_entitiesToSpawnInfo.size() - 1);

        /*Choose spawner*/
        const int    maxIt        = 10;
        int          it           = 0;
        unsigned int spawnerIndex = 0;
        const Vec3   posPlayer    = m_player->getTransform().getGlobalPosition();
        Vec3         spawnerPosition;
        do
        {
            spawnerIndex    = Random::ranged<int>(m_spawners.size() - 1);
            spawnerPosition = m_spawners[spawnerIndex].go.pGo->getTransform().getGlobalPosition();
        } while ((posPlayer - spawnerPosition).sqrLength() < sqrTotalRadius && ++it < maxIt);

        if (m_debug)
        {
            drawDebugSphere(spawnerPosition, m_spawnerZoneRadius, RGBA{RGB::red(), 0.6f}, 1.f, true);
        }

        const Vec3 newPosition = {position2D.x + spawnerPosition.x, spawnerPosition.y,
                                  position2D.y + spawnerPosition.z};

        /*Spawn this entity*/
        // GameObject* spawnedEntity = m_entitiesToSpawnInfo[indexEntityToSpawn].prefab->clone(*m_enemiesContainer.pGo);
        // spawnedEntity->getTransform().setTranslation(newPosition);
    }
}

void SpawnManager::updateEditor(double deltaTime)
{
    if (!m_debug)
        return;

    drawDebugSphere(m_player->getTransform().getGlobalPosition(), m_playerZoneRadius, RGBA{RGB::blue(), 0.6f}, 0.f,
                    true);

    for (auto&& elem : m_spawners)
    {
        GAME_ASSERT(elem.go.pGo, "null");

        if (elem.go.pGo)
            drawDebugSphere(elem.go.pGo->getTransform().getGlobalPosition(), m_spawnerZoneRadius,
                            RGBA{RGB::green(), 0.2f}, 0.f, true);
    }
}