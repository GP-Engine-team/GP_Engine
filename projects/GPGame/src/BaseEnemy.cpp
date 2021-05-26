#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>

#include <BaseEnemy.hpp>

#include <Generated/BaseEnemy.rfk.h>
File_GENERATED

    using namespace GPG;
using namespace GPM;
using namespace GPE;

BaseEnemy::BaseEnemy(GPE::GameObject& owner) noexcept : BaseCharacter(owner)
{
}

void BaseEnemy::start()
{
    BaseCharacter::start();
    enableUpdate(true);

    controller->setSpeed(0.5f);
    controller->setAngularSpeed(PI);

    GameObject* playerGO = Engine::getInstance()->sceneManager.getCurrentScene()->getGameObject("Player");
    GAME_ASSERT(playerGO, "Player not found");

    m_target = playerGO->getComponent<BaseCharacter>();
    GAME_ASSERT(m_target, "Player Base character component not found");
}

void BaseEnemy::onPostLoad()
{
    BaseCharacter::onPostLoad();
}

void BaseEnemy::update(double deltaTime)
{
    if (isDead())
    {
        m_animDeapthCounter += deltaTime;

        if (m_animDeapthCounter >= m_animDeapthCounterMax)
        {
            getOwner().destroy();
        }
    }
    else
    {
        const Vec3 targetPos = m_target->transform().getGlobalPosition();

        moveAndRotateToward(targetPos, deltaTime);

        if (m_radiusAttack * m_radiusAttack >
            (m_target->transform().getGlobalPosition() - transform().getGlobalPosition()).sqrLength())
        {
            m_attackCounter += deltaTime;

            if (m_attackCounter >= m_attackCounterMax)
            {
                m_attackCounter = 0.f;
                m_target->takeDamage(m_dammage);
            }
        }
        else
        {
            m_attackCounter = 0.f;
        }
    }
}