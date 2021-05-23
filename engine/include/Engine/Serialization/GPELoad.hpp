#pragma once

#include "xml/xmlLoader.hpp"

namespace GPE
{
union AmbiantComponent;
class Prefab;

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Prefab*& data, const rfk::Field& info);

template <>
void load(XmlLoader& context, Prefab*& data, const XmlLoader::LoadInfo& info);

} // namespace GPE