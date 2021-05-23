#pragma once

#include "xml/xmlLoader.hpp"

namespace GPE
{
union AmbiantComponent;

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Shader*& inspected, const XmlLoader::LoadInfo& info);

} // namespace GPE