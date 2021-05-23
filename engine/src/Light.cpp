#include "Engine/ECS/Component/Light/Light.hpp"
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/ECS/System/RenderSystem.hpp>

using namespace GPE;
using namespace GPM;

Mat4 Light::getLightSpaceMatrix() noexcept
{
    return Mat4::identity();
}

void Light::inspect(InspectContext& context)
{
    Component::inspect(context);

    DataInspector::inspect(context, m_ambientComp, "AmbientComp");
    DataInspector::inspect(context, m_diffuseComp, "DiffuseComp");
    DataInspector::inspect(context, m_specularComp, "SpecularComp");

    if (ImGui::Checkbox("##isShadowEnable", &m_shadowProperties.isEnable))
    {
        if (m_shadowProperties.isEnable)
        {
            getOwner().pOwnerScene->sceneRenderer.addShadowMap(*this);
        }
        else
        {
            getOwner().pOwnerScene->sceneRenderer.removeShadowMap(*this);
            ImGui::SetNextItemOpen(false);
        }
    }
    ImGui::SameLine();
    ImGui::PushEnabled(m_shadowProperties.isEnable);
    if (ImGui::CollapsingHeader("Shadow"))
    {
        if (DataInspector::inspect(context, m_shadowProperties.shadowMapSampleScale, "SampleScale"))
        {
            // Remove previouse
            getOwner().pOwnerScene->sceneRenderer.removeShadowMap(*this);

            // Re create it with new size
            getOwner().pOwnerScene->sceneRenderer.addShadowMap(*this);
        }
        DataInspector::inspect(context, m_shadowProperties.PCF, "PCF");
        // DataInspector::inspect(context, m_shadowProperties.bias, "Bias");
        DataInspector::inspect(context, m_shadowProperties.size, "Size");
    }
    ImGui::PopEnabled();
}

void Light::setShadowActive(bool newState) noexcept
{
    if (m_shadowProperties.isEnable == newState)
        return;

    m_shadowProperties.isEnable = newState;
    if (m_shadowProperties.isEnable)
        getOwner().pOwnerScene->sceneRenderer.addShadowMap(*this);
    else
        getOwner().pOwnerScene->sceneRenderer.removeShadowMap(*this);
}

void Light::updateToSystem() noexcept
{
    if (m_isActivated)
    {
        if (m_shadowProperties.isEnable)
            getOwner().pOwnerScene->sceneRenderer.addShadowMap(*this);

        getOwner().pOwnerScene->sceneRenderer.addLight(*this);
    }
    else
    {
        if (m_shadowProperties.isEnable)
            getOwner().pOwnerScene->sceneRenderer.removeShadowMap(*this);

        getOwner().pOwnerScene->sceneRenderer.removeLight(*this);
    }
}

File_GENERATED
