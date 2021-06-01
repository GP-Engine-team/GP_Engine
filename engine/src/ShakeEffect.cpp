#include <Engine/ECS/Component/ShakeEffect.hpp>

#include <GPM/Calc.hpp>
#include <GPM/Quaternion.hpp>
#include <GPM/Random.hpp>
#include <GPM/Vector3.hpp>

#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/ECS/Component/TransformComponent.hpp>

// Generated
#include "Generated/ShakeEffect.rfk.h"
File_GENERATED

    using namespace GPE;
using namespace GPM;

void ShakeEffect::start()
{
    if (m_isRunning)
        return;

    m_isRunning     = true;
    m_basePosition  = transform().getPosition();
    m_durationCount = shakeDuration;

    enableUpdate(true);
}

void ShakeEffect::stop()
{
    if (!m_isRunning)
        return;

    m_isRunning = false;
    transform().setTranslation(m_basePosition);
    enableUpdate(false);
}

void ShakeEffect::update(double deltaTime)
{
    Vec3 moveStrength = Random::unitPeripheralSphericalCoordonate() * shakeStrength;

    moveStrength.z = 0.f;
    transform().setTranslation(m_basePosition + moveStrength); // Set the local rotation the be the rotation amount.

    m_durationCount -= deltaTime;
    if (m_durationCount < 0.f)
    {
        stop();
    }
}

void ShakeEffect::updateEditor(double deltaTime)
{
    Vec3 moveStrength = Random::unitPeripheralSphericalCoordonate() * shakeStrength;

    moveStrength.z = 0.f;
    transform().setTranslation(m_basePosition + moveStrength); // Set the local rotation the be the rotation amount.

    m_durationCount -= deltaTime;
    if (m_durationCount < 0.f)
    {
        m_isRunning = false;
        transform().setTranslation(m_basePosition);
    }
}

void ShakeEffect::inspect(InspectContext& context)
{
    Component::inspect(context);

    DataInspector::inspect(context, shakeStrength, "Shake amount");
    DataInspector::inspect(context, shakeDuration, "Shake duration");

    if (ImGui::Button("Start"))
    {
        if (m_isRunning)
            return;

        m_isRunning     = true;
        m_basePosition  = transform().getPosition();
        m_durationCount = shakeDuration;

        enableUpdateEditor(true);
    }

    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        if (!m_isRunning)
            return;

        m_isRunning = false;
        transform().setTranslation(m_basePosition);

        enableUpdateEditor(false);
    }
}