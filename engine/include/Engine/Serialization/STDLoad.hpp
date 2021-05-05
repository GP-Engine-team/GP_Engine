#pragma once

#include "Engine/Serialization/STDReflect.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
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
void load(XmlLoader& context, std::list<T>& inspected, const XmlLoader::LoadInfo& info);
template <typename T>
void load(class XmlLoader& context, std::list<T>& inspected, const rfk::Field& info);

template <typename T>
void load(class XmlLoader& context, std::list<T*>& inspected, const rfk::Field& info);

template <typename T>
void load(XmlLoader& context, std::list<T*>& inspected, const XmlLoader::LoadInfo& info);

template <typename T>
void load(XmlLoader& context, std::vector<T>& inspected, const XmlLoader::LoadInfo& info);

template <typename T, typename U>
void load(XmlLoader& context, std::pair<T, U>& inspected, const XmlLoader::LoadInfo& info);

template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const XmlLoader::LoadInfo& info);
template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info);

template <typename T>
void load(XmlLoader& context, std::unique_ptr<T>& loaded, const XmlLoader::LoadInfo& info);
}


#include "STDLoad.inl"