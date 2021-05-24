#include <CircularEntitiesSpawner.hpp>

#include <Engine/Intermediate/GameObject.hpp>
#include <gpm/Random.hpp>
#include <gpm/Vector3.hpp>

#include <Generated/CircularEntitiesSpawner.rfk.h>
File_GENERATED

    using namespace GPE;
using namespace GPG;
using namespace GPM;

CircularEntitiesSpawner::CircularEntitiesSpawner(GameObject& gameObject) : BehaviourComponent(gameObject)
{
    enableUpdate(true);
}

void CircularEntitiesSpawner::onPostLoad()
{
    enableUpdate(true);
    BehaviourComponent::onPostLoad();
}

void CircularEntitiesSpawner::start()
{
    GAME_ASSERT(m_container, "Missing container ref in CircularEntitiesSpawner");
    m_nextDelay = m_spawnDelay + Random::ranged(-m_spawnDelayInterval, m_spawnDelayInterval);

    for (auto&& elem : m_entitiesToSpawnInfo)
        GAME_ASSERT(elem.prefab, "null");
}

void CircularEntitiesSpawner::update(double deltaTime)
{
    if (m_entitiesToSpawnInfo.empty())
        return;

    m_delayCount += deltaTime;

    while (m_delayCount >= m_nextDelay)
    {

        m_delayCount -= m_nextDelay;
        m_nextDelay     = m_spawnDelay + Random::ranged(-m_spawnDelayInterval, m_spawnDelayInterval);
        Vec2 position2D = Random::circularCoordinate(
            {m_gameObject->getTransform().getGlobalPosition().x, m_gameObject->getTransform().getGlobalPosition().z},
            m_zoneRadius);
        Vec3 newPosition = {position2D.x, m_gameObject->getTransform().getGlobalPosition().y, position2D.y};

        /*Choose random entity*/
        unsigned int indexEntityToSpawn = Random::ranged<int>(m_entitiesToSpawnInfo.size() - 1);

        /*Spawn this entity*/
        GameObject* spawnedEntity = m_entitiesToSpawnInfo[indexEntityToSpawn].prefab->clone(*m_container);
        spawnedEntity->getTransform().setTranslation(newPosition);

        // if (spawnedEntity->getComponent<EnnemyController>())
        //    spawnedEntity->getComponent<EnnemyController>()->setCheckpoint(m_checkpoint);

        /*Remove this entity. If all the entities of a type were generated, remove this entity's type*/
        if (--m_entitiesToSpawnInfo[indexEntityToSpawn].numberEntity == 0)
        {
            m_entitiesToSpawnInfo.erase(m_entitiesToSpawnInfo.begin() + indexEntityToSpawn);
        }
    }
}