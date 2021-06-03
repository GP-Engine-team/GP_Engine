#include <SpawnManager.hpp>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <gpm/Calc.hpp>
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

    BehaviourComponent::onPostLoad();
}

void SpawnManager::start()
{
    m_player = Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    
    GAME_ASSERT(m_player, "Player not found");
    GAME_ASSERT(m_enemiesContainer.pData, "Missing container ref in SpawnManager");
    GAME_ASSERT(m_entitiesToSpawnInfo.size(), "Spawner without info");
    GAME_ASSERT(m_spawners.size(), "SpawnerManager without spawner");

    for (auto&& elem : m_spawners)
    {
        GAME_ASSERT(elem.go.pData, "null");
    }

    sqrTotalRadius = (m_spawnerZoneRadius + m_playerZoneRadius) * (m_spawnerZoneRadius + m_playerZoneRadius);

    if (m_difficultyInfo.empty())
    {
        autoGenerateLinearDifficulty(10, DifficultyLevel{3.f, 3u}, DifficultyLevel{0.5f, 30u});
    }
    GAME_ASSERT(m_difficultyInfo.size(), "No difficulty set");
    setDifficulty(m_currentDifficulty); // clamp
}

void SpawnManager::update(double deltaTime)
{
    if (m_enemiesContainer.pData->children.size() > m_difficultyInfo[m_currentDifficulty].m_maxEntity)
        return;

    m_delayCount += deltaTime;

    while (m_delayCount >= m_nextDelay)
    {

        m_delayCount -= m_nextDelay;
        // m_nextDelay     = m_spawnDelay + Random::ranged(-m_spawnDelayInterval, m_spawnDelayInterval);
        m_nextDelay = m_difficultyInfo[m_currentDifficulty].m_spawnfrequency;

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
            spawnerPosition = m_spawners[spawnerIndex].go.pData->getTransform().getGlobalPosition();
        } while ((posPlayer - spawnerPosition).sqrLength() < sqrTotalRadius && ++it < maxIt);

        if (m_debug)
        {
            drawDebugSphere(spawnerPosition, m_spawnerZoneRadius, RGBA{RGB::red(), 0.6f}, 1.f, true);
        }

        const Vec3 newPosition = {position2D.x + spawnerPosition.x, spawnerPosition.y,
                                  position2D.y + spawnerPosition.z};

        /*Spawn this entity*/
        // GameObject* spawnedEntity =
        // m_entitiesToSpawnInfo[indexEntityToSpawn].prefab->clone(*m_enemiesContainer.pData);
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
        GAME_ASSERT(elem.go.pData, "null");

        if (elem.go.pData)
            drawDebugSphere(elem.go.pData->getTransform().getGlobalPosition(), m_spawnerZoneRadius,
                            RGBA{RGB::green(), 0.2f}, 0.f, true);
    }
}

void SpawnManager::setDifficulty(unsigned int level)
{
    m_currentDifficulty = std::clamp(level, 0u, (unsigned int)(m_difficultyInfo.size() - 1));
    m_nextDelay         = m_difficultyInfo[m_currentDifficulty].m_spawnfrequency;
}

void SpawnManager::autoGenerateLinearDifficulty(unsigned int count, const DifficultyLevel& min,
                                                const DifficultyLevel& max)
{
    m_difficultyInfo.clear();

    for (unsigned int i = 0; i < count; ++i)
    {
        float t = i / ((float)count - 1.f);
        m_difficultyInfo.emplace_back(GPM::lerp(min.m_spawnfrequency, max.m_spawnfrequency, t),
                                      GPM::lerp(min.m_maxEntity, max.m_maxEntity, t));
    }
}