#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/ECS/Component/AnimationComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Wave.hpp>
#include <Engine/Resources/Importer/Importer.hpp>

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

    GameObject* playerGO = Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(playerGO, "Player not found");

    m_target = playerGO->getComponent<BaseCharacter>();
    GAME_ASSERT(m_target, "Player Base character component not found");

    source->playSound("Zombie", true);
}

void BaseEnemy::onPostLoad()
{
    BaseCharacter::onPostLoad();

    source = &getOwner().getOrCreateComponent<GPE::AudioComponent>();

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.gain *= 100.f;
    sourceSettings.spatialized   = AL_TRUE;
    sourceSettings.relative      = AL_FALSE;
    sourceSettings.loop          = AL_TRUE;
    sourceSettings.position      = getOwner().getTransform().getGlobalPosition();
    sourceSettings.rollOffFactor = 7;

    GPE::Wave testSound3("./resources/sounds/zombie.wav", "Zombie", sourceSettings.spatialized);

    source->setSound("Zombie", "Zombie", sourceSettings);
}

void BaseEnemy::update(double deltaTime)
{
    if (isDead())
    {
        if (m_animDeathCounter <= 0.0f)
        {
            GPE::AnimationComponent* animComp = m_gameObject->getComponent<GPE::AnimationComponent>();
            if (animComp != nullptr)
            {
                auto& a = GPE::Engine::getInstance()->animResourcesManager.getAll<GPE::Animation>();
                const char* src = "resources\\Animations\\ZombieDeath.GPAnimation";
                Engine::getInstance()->animResourcesManager.add<Animation>(src, readAnimationFile(src));
                GPE::Animation* anim = GPE::Engine::getInstance()->animResourcesManager.get<GPE::Animation>(src);
                if (anim)
                {
                    animComp->playAnimation(anim);
                    m_animDeathCounterMax = anim->getDuration();
                }
            }
        }

        m_animDeathCounter += float(deltaTime);

        if (m_animDeathCounter >= m_animDeathCounterMax)
        {
            //getOwner().destroy();
            GPE::AnimationComponent* animComp = m_gameObject->getComponent<GPE::AnimationComponent>();
            if (animComp != nullptr)
            {
                animComp->setActive(false);
            }
        }
    }
    else
    {
        const Vec3 targetPos = m_target->transform().getGlobalPosition();

        moveAndRotateToward(targetPos, float(deltaTime));

        if (m_radiusAttack * m_radiusAttack >
            (m_target->transform().getGlobalPosition() - transform().getGlobalPosition()).sqrLength())
        {
            m_attackCounter += float(deltaTime);

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
