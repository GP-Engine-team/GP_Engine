#include "Engine/Resources/Material.hpp"
#include "Engine/Core/Debug/Log.hpp"

using namespace GPE;

Material::Material(const CreateArgWithTexture& arg) : m_comp(arg.comp), m_pDiffuseTexture(), m_name(arg.name)
{
	m_pDiffuseTexture = std::make_shared<Texture>(arg.diffuseTextureArg);
}