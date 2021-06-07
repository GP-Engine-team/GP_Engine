#include <Loot.hpp>

#include <Generated/Loot.rfk.h>
File_GENERATED

#include <Engine/Core/Debug/assert.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <algorithm>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

using namespace GPE;
using namespace GPG;
using namespace GPM;

Loot::Loot(GameObject& owner) : BehaviourComponent(owner)
{
}

void Loot::onPostLoad()
{
    enableUpdate(true);

    BehaviourComponent::onPostLoad();
}

void Loot::start()
{
    GPE::GameObject* m_GO =
        GPE::Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(m_GO, "Player not found");

    m_player = m_GO->getComponent<BasePlayer>();
    GAME_ASSERT(m_player, "Base player not found");

    m_GO = GPE::Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("SpawnerManager");
    GAME_ASSERT(m_GO, "SpawnerManager gameObject not found");

    m_spawnerManager = m_GO->getComponent<SpawnManager>();
    GAME_ASSERT(m_spawnerManager, "SpawnerManager not found");

    sqrTotalRadius = (m_radius + m_player->radiusLootCollection) * (m_radius + m_player->radiusLootCollection);
}

void Loot::update(double deltaTime)
{
    // Add levitation effect
    m_delay += float(deltaTime) * m_speed;

    if (m_delay > PI * 2.f)
    {
        m_delay -= PI * 2.f;
    }

    transform().setTranslation(
        {transform().getPosition().x, m_initialY + cosf(m_delay) * m_heightIntensity, transform().getPosition().z});

    // Check if loot is collected
    if ((m_player->getOwner().getTransform().getGlobalPosition() - transform().getGlobalPosition()).sqrLength() <
        sqrTotalRadius)
    {
        m_player->collectLoot(*this);
        m_spawnerManager->increaseDifficulty();
        getOwner().destroy();
    }
}