#include <Sun.hpp>

#include <Generated/Sun.rfk.h>
File_GENERATED

#include <Engine/Core/Debug/assert.hpp>
#include <gpm/Vector3.hpp>
#include <gpm/Vector4.hpp>

    using namespace GPE;
using namespace GPG;
using namespace GPM;

Sun::Sun(GameObject& owner) : BehaviourComponent(owner)
{
    enableUpdate(true);
}

void Sun::onPostLoad()
{
    enableUpdate(true);
    BehaviourComponent::onPostLoad();
}

void Sun::start()
{
    GAME_ASSERT(m_player, "Missing player ref");
    m_pSunDirectionnalLight = getOwner().getComponent<DirectionalLight>();
}

void Sun::update(double deltaTime)
{
    m_currentTime += deltaTime;

    if (m_currentTime > m_dayDuration + m_nightDuration)
    {
        m_currentTime -= m_dayDuration + m_nightDuration;
    }

    /*move light and sun*/
    Vec3 newDirection;

    float rot    = (m_currentTime - m_dayStart) * (PI * 2.f) / (m_dayDuration + m_nightDuration);
    newDirection = {cosf(rot), -sinf(rot), 0.f};

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