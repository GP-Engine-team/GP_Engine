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
    enableUpdate(true);
}

void BaseEnemy::start()
{
    BaseCharacter::start();

    controller->setSpeed(0.5f);
    controller->setAngularSpeed(PI);

    m_target = Engine::getInstance()->sceneManager.getCurrentScene()->getGameObject("Player");
    GAME_ASSERT(m_target, "Player not found");
}

void BaseEnemy::onPostLoad()
{
    BaseCharacter::onPostLoad();
}

void BaseEnemy::update(double deltaTime)
{
    const Vec3 targetPos = m_target->getTransform().getGlobalPosition();

    moveAndRotateToward(targetPos, deltaTime);
}