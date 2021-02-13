#include "Engine/Resources/Material.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Debug;
using namespace Engine::Resurces;

Material::Material (const MaterialAndTextureCreateArg& arg)
    :   m_comp                           (arg.comp),
        m_pDiffuseTexturetexture         (),
        m_name                           (arg.name)
{
    if (arg.pathDiffuseTexture == nullptr)
    {
        m_pDiffuseTexturetexture = nullptr;
    }
    else
    {
        TextureCreateArg textArg;
        textArg.path            = arg.pathDiffuseTexture;
        textArg.filterType      = arg.filterType;
        textArg.loadInGPU       = arg.loadInGPU;
        textArg.wrapType        = arg.wrapType;
        textArg.flipTexture     = arg.flipTexture;
        
        m_pDiffuseTexturetexture = std::make_shared<Texture>(textArg);
    }
}

Material::Material (const MaterialAttrib& arg)
    :   m_comp                           {   arg.ambient, 
                                            arg.diffuse, 
                                            arg.specular,
                                            arg.shininess},
        m_pDiffuseTexturetexture         (std::make_shared<Texture>((std::string(PATH_TEXTURE_RESSORUCE) + arg.nameDiffuseTexture).c_str())),
        m_name                           (arg.id)
{}

Material::Material (MaterialCreateArg& arg)
    :   m_comp                           (arg.comp),
        m_pDiffuseTexturetexture         (std::move(arg.pTexture)),
        m_name                           (arg.name)
{}