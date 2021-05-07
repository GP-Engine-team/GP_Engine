#include "Engine/Serialization/GPESave.hpp"

#include "Engine/Resources/Type.hpp"

namespace GPE
{

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, data.rgbi, info);
}

} // namespace GPE