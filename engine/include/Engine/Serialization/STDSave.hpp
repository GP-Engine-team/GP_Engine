#pragma once

#include "Engine/Serialization/STDReflect.hpp"
#include "Engine/Serialization/xml/xmlLoader.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"
#include <list>

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
void save(XmlSaver& context, const std::list<T*>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (T* elem : inspected)
    {
        if constexpr (std::is_base_of_v<rfk::Object, T>)
        {
            GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", elem->getArchetype().id});
        }
        else 
        {
            GPE::save(context, elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});
        }
        i++;
    }

    context.pop();
}

template <typename T>
void load(XmlLoader& context, std::list<T*>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        for (T* elem : inspected)
        {
            // delete elem;
            elem->destroy();
        }

        inspected.clear();

        for (size_t i = 0; i < size; i++)
        {
            // T* elem = new T(); // TODO : Share with context
            // GPE::load(context, *elem, XmlSaver::SaveInfo{std::to_string(i), "T", 0});

            T* elem;
            GPE::load(context, elem, XmlLoader::LoadInfo{std::to_string(i), "T", info.typeId});
            inspected.emplace_back(elem);
        }

        context.pop();
    }
}

} // namespace GPE

#include "STDSave.inl"