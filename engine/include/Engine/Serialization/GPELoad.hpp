#pragma once

#include "xml/xmlLoader.hpp"

namespace GPE
{
union AmbiantComponent;

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info);

} // namespace GPE