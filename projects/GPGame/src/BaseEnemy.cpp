#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/ECS/Component/AnimationComponent.hpp>
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/BehaviourComponent.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Wave.hpp>

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

    controller->setAngularSpeed(PI);

    GameObject* playerGO = Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(playerGO, "Player not found");

    m_target = &playerGO->getOrCreateComponent<BaseCharacter>();
    GAME_ASSERT(m_target, "Player Base character component not found");

    m_animComps = m_gameObject->getComponents<GPE::AnimationComponent>();

    m_controller = &m_gameObject->getOrCreateComponent<GPE::CharacterController>();
    GAME_ASSERT(m_controller, "Null");

    m_source->playSound("Zombie", true);

    if (m_walkAnimation != nullptr)
    {
        for (GPE::AnimationComponent* animComp : m_animComps)
        {
            if (animComp != nullptr)
                animComp->playAnimation(m_walkAnimation);
        }
    }
}

void BaseEnemy::onPostLoad()
{
    m_source = &getOwner().getOrCreateComponent<GPE::AudioComponent>();

    GPE::SourceSettings sourceSettings;
    sourceSettings.pitch = 1.f;
    sourceSettings.gain *= 100.f;
    sourceSettings.spatialized   = AL_TRUE;
    sourceSettings.relative      = AL_FALSE;
    sourceSettings.loop          = AL_TRUE;
    sourceSettings.position      = getOwner().getTransform().getGlobalPosition();
    sourceSettings.rollOffFactor = 70;

    GPE::Wave testSound3("./resources/sounds/zombie.wav", "Zombie", sourceSettings.spatialized);

    m_source->setSound("Zombie", "Zombie", sourceSettings);

    BaseCharacter::onPostLoad();
}

void BaseEnemy::update(double deltaTime)
{
    if (isDead())
    {
        m_animDeathCounter += float(deltaTime);

        if (m_animDeathCounter >= m_animDeathCounterMax * .5f)
        {
            m_gameObject->getTransform().translate({.0f, -m_disappearanceSpeed * float(deltaTime), .0f});

            if (transform().getGlobalPosition().y < -std::abs(m_maxHeightBeforDestroying))
                getOwner().destroy();
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

void BaseEnemy::onDeath()
{
    BaseCharacter::onDeath();

    //const char* src = "resources\\Animations\\ZombieDeath.GPAnimation";

    //GPE::Animation* anim = GPE::Engine::getInstance()->animResourcesManager.get<GPE::Animation>(src);
    //if (anim == nullptr)
    //{
    //    anim = &Engine::getInstance()->animResourcesManager.add<Animation>(src, readAnimationFile(src));
    //}

    if (m_deathAnimation != nullptr)
    {
        for (GPE::AnimationComponent* animComp : m_animComps)
        {
            if (animComp != nullptr)
            {
                animComp->playAnimation(m_deathAnimation);
                animComp->shouldLoop  = false;
                m_animDeathCounterMax  = m_deathAnimation->getDuration();

            }
        }
    }

    m_controller->setActive(false);
    m_source->setActive(false);
}