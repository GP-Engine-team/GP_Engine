#include "Engine/Serialization/GPELoad.hpp"

#include "Engine/Engine.hpp"
#include "Engine/Resources/Type.hpp"
#include "Engine/Resources/Shader.hpp"
#include "Engine/Resources/Importer/Importer.hpp"

namespace GPE
{

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, data.rgbi, info);
}

template <>
void GPE::load(XmlLoader& context, Shader*& inspected, const XmlLoader::LoadInfo& info)
{
    std::string shaderName;
    GPE::load(context, shaderName, XmlLoader::LoadInfo{"pShader", "Shader*", 0});
    if (!(inspected = Engine::getInstance()->resourceManager.get<GPE::Shader>(shaderName)))
    {
        inspected = loadShaderFile(shaderName.c_str());
    }
}


} // namespace GPE