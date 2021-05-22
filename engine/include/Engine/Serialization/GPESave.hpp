#pragma once


#include "xml/xmlSaver.hpp"


namespace GPE
{
union AmbiantComponent;

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info);

} // namespace GPE