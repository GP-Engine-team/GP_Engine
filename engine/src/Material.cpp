#include "Engine/Resources/Material.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace GPE;

Material::Material(const CreateArgWithTexture& arg) : m_comp(arg.comp), m_pDiffuseTexture(), m_name(arg.name)
{
    if (arg.pathDiffuseTexture == nullptr)
    {
        m_pDiffuseTexture = nullptr;
    }
    else
    {
        Texture::CreateArg textArg;
        textArg.path        = arg.pathDiffuseTexture;
        textArg.filterType  = arg.filterType;
        textArg.loadInGPU   = arg.loadInGPU;
        textArg.wrapType    = arg.wrapType;
        textArg.flipTexture = arg.flipTexture;

        m_pDiffuseTexture = std::make_shared<Texture>(textArg);
    }
}