#include "Engine/Serialization/GPESave.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Resources/Type.hpp"
#include "Engine/Resources/Shader.hpp"

namespace GPE
{

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, data.rgbi, info);
}

template <>
void GPE::save(XmlSaver& context, Shader* const& inspected, const XmlSaver::SaveInfo& info)
{
    if (inspected == nullptr)
        return;

    if (const std::string* shaderName = GPE::Engine::getInstance()->resourceManager.getKey(inspected))
    {
        GPE::save(context, *shaderName, XmlSaver::SaveInfo{"pShader", "Shader*", 0});
    }
}

} // namespace GPE