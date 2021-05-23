#pragma once


#include "xml/xmlSaver.hpp"


namespace GPE
{
union AmbiantComponent;

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Shader* const& inspected, const XmlSaver::SaveInfo& info);

} // namespace GPE