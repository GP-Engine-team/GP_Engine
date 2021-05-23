#pragma once

#include "xml/xmlSaver.hpp"

namespace GPE
{
union AmbiantComponent;
class Prefab;

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Prefab* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Prefab* const& data, const XmlSaver::SaveInfo& info);

} // namespace GPE