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

    m_target = Engine::getInstance()->sceneManager.getCurrentScene()->getGameObject("Player");
    GAME_ASSERT(m_target, "Player not found");
}

void BaseEnemy::onPostLoad()
{
    BaseCharacter::onPostLoad();
}

void BaseEnemy::update(double deltaTime)
{
    rotateYToward(m_target->getTransform().getGlobalPosition(), deltaTime);
}