#pragma once

#include "Engine/Serialization/STDReflect.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include "Refureku/TypeInfo/Variables/Field.h"
#include <imgui.h>
#include <list>
#include <unordered_map>
#include <vector>

namespace rfk
{
class Field;
}

namespace GPE
{
template <typename T>
void save(XmlSaver& context, const std::list<T>& saved, const XmlSaver::SaveInfo& info);
template <typename T>
void save(class XmlSaver& context, const std::list<T>& saved, const rfk::Field& info);

template <typename T>
void save(class XmlSaver& context, const std::list<T*>& saved, const rfk::Field& info);

template <typename T>
void save(XmlSaver& context, const std::list<T*>& saved, const XmlSaver::SaveInfo& info);

template <typename T>
void save(XmlSaver& context, const std::vector<T>& saved, const rfk::Field& info);

template <typename T>
void save(XmlSaver& context, const std::vector<T>& saved, const XmlSaver::SaveInfo& info);

template <typename T, typename U>
void save(XmlSaver& context, const std::pair<T, U>& saved, const XmlSaver::SaveInfo& info);

template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& saved, const XmlSaver::SaveInfo& info);
template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& saved, const rfk::Field& info);

template <typename T>
void save(XmlSaver& context, const std::unique_ptr<T>& saved, const XmlSaver::SaveInfo& info);

} // namespace GPE

#include "STDSave.inl"