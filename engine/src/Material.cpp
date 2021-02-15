#include "Engine/Resources/Material.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Debug;
using namespace Engine::Resources;

Material::Material(const CreateArgWithTexture& arg)
    : m_comp(arg.comp), m_pDiffuseTexture(), m_name(arg.name)
{
    if (arg.pathDiffuseTexture == nullptr)
    {
        m_pDiffuseTexture = nullptr;
    }
    else
    {
        Texture::CreateArg textArg;
        textArg.path = arg.pathDiffuseTexture;
        textArg.filterType = arg.filterType;
        textArg.loadInGPU = arg.loadInGPU;
        textArg.wrapType = arg.wrapType;
        textArg.flipTexture = arg.flipTexture;

        m_pDiffuseTexture = std::make_shared<Texture>(textArg);
    }
}

Material::Material(const MaterialAttrib& arg)
    : m_comp{arg.ambient, arg.diffuse, arg.specular, arg.shininess},
      m_pDiffuseTexture(
          std::make_shared<Texture>((std::string(PATH_TEXTURE_RESOURCE) + arg.nameDiffuseTexture).c_str())),
      m_name(arg.id)
{
}

Material::Material(CreateArg& arg)
    : m_comp(arg.comp), m_pDiffuseTexture(std::move(arg.pTexture)), m_name(arg.name)
{
}