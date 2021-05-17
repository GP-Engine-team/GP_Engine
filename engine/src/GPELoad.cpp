#include "Engine/Serialization/GPELoad.hpp"

#include "Engine/Resources/Type.hpp"

namespace GPE
{

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info)
{
    GPE::load(context, data.rgbi, info);
}

} // namespace GPE