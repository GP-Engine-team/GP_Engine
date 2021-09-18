#include <BasePlayer.hpp>

#include <Generated/EvacuationPoint.cpp.h>

    using namespace GPG;
using namespace GPE;
using namespace GPM;

void EvacuationPoint::start()
{
    GAME_ASSERT(m_zoneParticle.pData, "null");

    GPE::GameObject* m_GO =
        GPE::Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(m_GO, "Player not found");

    m_player = m_GO->getComponent<BasePlayer>();
    GAME_ASSERT(m_player, "Base player not found");
}

void EvacuationPoint::update(double deltaTime)
{
    if ((m_player->getOwner().getTransform().getGlobalPosition() - transform().getGlobalPosition()).sqrLength() <
        m_zoneRadius * m_zoneRadius)
    {
        m_timeCount += (float)deltaTime;

        if (m_timeCount > m_timeDuration)
        {
            m_player->onWin();
            enableUpdate(false);
        }
    }
    else
    {
        m_timeCount = std::max(0.f, m_timeCount - (float)deltaTime);
    }
}

void EvacuationPoint::activeEvacutaitonPoint()
{
    m_zoneParticle.pData->start();
    enableUpdate(true);
}