#include <Engine/Core/Debug/assert.hpp>
#include <Engine/Engine.hpp>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

#include <Generated/Sun.cpp.h>

    using namespace GPE;
using namespace GPG;
using namespace GPM;

Sun::Sun(GameObject& owner) : BehaviourComponent(owner)
{
}

void Sun::onPostLoad()
{
    BehaviourComponent::onPostLoad();
    enableUpdate(true);
}

void Sun::start()
{
    m_player = GPE::Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().getGameObject("Player");
    GAME_ASSERT(m_player, "Player not found");

    m_pSunDirectionnalLight = &getOwner().getOrCreateComponent<DirectionalLight>();

    m_midDay   = m_dayStart + m_dayDuration / 2.f;
    m_midNight = m_dayStart + m_nightDuration / 2.f;
}

void Sun::update(double deltaTime)
{
    m_currentTime += float(deltaTime);

    if (m_currentTime > m_dayDuration + m_nightDuration)
    {
        m_currentTime -= m_dayDuration + m_nightDuration;
    }

    /*move light and sun*/
    float rot          = m_currentTime / (m_dayDuration + m_nightDuration) * TWO_PI;
    Vec3  newDirection = {0.f, -std::abs(sinf(rot)), cosf(rot)};
    newDirection.normalize();

    getOwner().getTransform().setTranslation(m_player->getTransform().getGlobalPosition() -
                                             newDirection * m_sunDistance);
    m_pSunDirectionnalLight->setDirection(newDirection);

    /*Manage luminosity and color temperatur*/
    if (m_currentTime < m_dayStart - m_sunRiseDuration) /*night*/
    {
        m_pSunDirectionnalLight->setAmbient(nightColorAmbiante);
        m_pSunDirectionnalLight->setDiffuse(nightColorDiffuse);
        m_pSunDirectionnalLight->setSpecular(nightColorSpecular);
    }
    else if (m_currentTime < m_dayStart) /*sunRise*/
    {
        float t = (m_currentTime - (m_dayStart - m_sunRiseDuration)) / m_sunRiseDuration;
        m_pSunDirectionnalLight->setAmbient(nightColorAmbiante.rgbi.lerp(morningSunLightColorAmbiante.rgbi, t));
        m_pSunDirectionnalLight->setDiffuse(nightColorDiffuse.rgbi.lerp(morningSunLightColorDiffuse.rgbi, t));
        m_pSunDirectionnalLight->setSpecular(nightColorSpecular.rgbi.lerp(morningSunLightColorSpecular.rgbi, t));
    }
    else if (m_currentTime < m_midDay) /*light increase*/
    {
        float t = (m_currentTime - m_dayStart) / (m_midDay - m_dayStart);
        m_pSunDirectionnalLight->setAmbient(
            morningSunLightColorAmbiante.rgbi.lerp(midDaySunLightColorAmbiante.rgbi, t));
        m_pSunDirectionnalLight->setDiffuse(morningSunLightColorDiffuse.rgbi.lerp(midDaySunLightColorDiffuse.rgbi, t));
        m_pSunDirectionnalLight->setSpecular(
            morningSunLightColorSpecular.rgbi.lerp(midDaySunLightColorSpecular.rgbi, t));
    }
    else if (m_currentTime < m_nightStart) /*light deacrease*/
    {
        float t = (m_currentTime - m_midDay) / (m_nightStart - m_midDay);
        m_pSunDirectionnalLight->setAmbient(
            midDaySunLightColorAmbiante.rgbi.lerp(eveningSunLightColorAmbiante.rgbi, t));
        m_pSunDirectionnalLight->setDiffuse(midDaySunLightColorDiffuse.rgbi.lerp(eveningSunLightColorDiffuse.rgbi, t));
        m_pSunDirectionnalLight->setSpecular(
            midDaySunLightColorSpecular.rgbi.lerp(eveningSunLightColorSpecular.rgbi, t));
    }
    else if (m_currentTime < m_nightStart + m_sunSetDuration) /*sunset*/
    {
        float t = (m_currentTime - m_nightStart) / m_sunSetDuration;
        m_pSunDirectionnalLight->setAmbient(eveningSunLightColorAmbiante.rgbi.lerp(nightColorAmbiante.rgbi, t));
        m_pSunDirectionnalLight->setDiffuse(eveningSunLightColorDiffuse.rgbi.lerp(nightColorDiffuse.rgbi, t));
        m_pSunDirectionnalLight->setSpecular(eveningSunLightColorSpecular.rgbi.lerp(nightColorSpecular.rgbi, t));
    }
    else /*night*/
    {
        m_pSunDirectionnalLight->setAmbient(nightColorAmbiante);
        m_pSunDirectionnalLight->setDiffuse(nightColorDiffuse);
        m_pSunDirectionnalLight->setSpecular(nightColorSpecular);
    }
}