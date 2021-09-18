#include <Generated/DirectionalLight.cpp.h>

    using namespace GPE;
using namespace GPM;

DirectionalLight::~DirectionalLight()
{
}

DirectionalLight::DirectionalLight(GameObject& owner) noexcept : DirectionalLight(owner, CreateArg{})
{
}

DirectionalLight::DirectionalLight(GameObject& owner, const CreateArg& arg) noexcept
    : Light{owner, arg.ambient, arg.diffuse, arg.specular}, m_direction{arg.direction.normalized()}
{
}

Mat4 DirectionalLight::getLightSpaceMatrix() noexcept
{
    const Mat4 lightProjection =
        Transform::orthographic(m_shadowProperties.size, -m_shadowProperties.size, m_shadowProperties.size,
                                -m_shadowProperties.size, m_shadowProperties.near, m_shadowProperties.far);
    const Vec3 globalPos = getOwner().getTransform().getGlobalPosition();
    const Mat4 lightView =
        GPM::Transform::lookAt(globalPos, globalPos + m_direction, getOwner().getTransform().getVectorRight());
    return lightProjection * lightView.inversed();
}

void DirectionalLight::addToLightToUseBuffer(std::vector<LightData>& lb) noexcept
{
    lb.push_back(
        {m_ambientComp, m_diffuseComp, m_specularComp, m_direction, 3.f, 0.f, 0.f, 0.f, 0.f, {0.f, 0.f, 0.f}, 0.f});
}

void DirectionalLight::inspect(GPE::InspectContext& context)
{
    Light::inspect(context);

    DataInspector::inspect(context, m_direction, "Direction");
    if (context.isDirty())
    {
        m_direction.normalize();
    }
}