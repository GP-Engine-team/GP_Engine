#pragma once

#include "Engine/Serialization/STDReflect.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include <list>
#include <vector>
#include <unordered_map>

namespace rfk
{
class Field;
}

namespace GPE
{
template <typename T>
void save(XmlSaver& context, const std::list<T>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (const T& elem : inspected)
    {
        GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        i++;
    }

    context.pop();
}

template <typename T>
void save(class XmlSaver& context, const std::list<T>& inspected, const rfk::Field& info)
{
    GPE::save(context, inspected, fieldToSaveInfo(info));
}

template <typename T>
void load(XmlLoader& context, std::list<T>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        inspected.clear();

        for (size_t i = 0; i < size; i++)
        {
            inspected.emplace_back();
            GPE::load(context, inspected.back(), XmlLoader::LoadInfo{std::to_string(i), "T", info.typeId});
        }

        context.pop();
    }
}

template <typename T>
void load(class XmlLoader& context, std::list<T>& inspected, const rfk::Field& info)
{
    GPE::load(context, inspected, fieldToLoadInfo(info));
}

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

template <typename T, typename U>
void save(XmlSaver& context, const std::pair<T, U>& inspected, const XmlSaver::SaveInfo& info);
template <typename T, typename U>
void load(XmlLoader& context, std::pair<T, U>& inspected, const XmlLoader::LoadInfo& info);

template <typename KEY,  typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& inspected, const XmlSaver::SaveInfo& info);
template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info);

template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const XmlLoader::LoadInfo& info);
template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info);


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