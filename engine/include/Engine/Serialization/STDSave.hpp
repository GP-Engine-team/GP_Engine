#pragma once

#include "Engine/Serialization/STDReflect.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include <list>
#include <vector>

namespace rfk
{
class Field;
}

namespace GPE
{
template <typename T>
void save(class XmlSaver& context, const std::list<T*>& inspected, const rfk::Field& info);


template <typename T>
void load(class XmlLoader& context, std::list<T*>& inspected, const rfk::Field& info);

template <typename T>
void save(XmlSaver& context, const std::list<T*>& inspected, const XmlSaver::SaveInfo& info);

template <typename T>
void load(XmlLoader& context, std::list<T*>& inspected, const XmlLoader::LoadInfo& info);

template<typename T>
void save(XmlSaver& context, const std::vector<T>& inspected, const XmlSaver::SaveInfo& info);

template <typename T>
void load(XmlLoader& context, std::vector<T>& inspected, const XmlLoader::LoadInfo& info);


template <typename T>
void save(XmlSaver& context, const std::unique_ptr<T>& saved, const XmlSaver::SaveInfo& info)
{
    GPE::save(context, saved.get(), info);
}

template <typename T>
void load(XmlLoader& context, std::unique_ptr<T>& loaded, const XmlLoader::LoadInfo& info)
{

    GPE::load(context, loaded.get(), info);
}

} // namespace GPE

#include "STDSave.inl"