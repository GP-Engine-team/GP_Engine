#include <imgui.h>
#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
template <typename T>
void save(class XmlSaver& context, const std::list<T*>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, fieldToSaveInfo(info));
}

template <typename T>
void load(XmlLoader& context, std::list<T*>& loaded, const rfk::Field& info)
{
    GPE::load(context, loaded, fieldToLoadInfo(info));
}

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

template<typename T>
void save(XmlSaver& context, const std::vector<T>& inspected, const XmlSaver::SaveInfo& info)
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
void load(XmlLoader& context, std::vector<T>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        inspected.clear();
        inspected.reserve(size);

        for (size_t i = 0; i < size; i++)
        {
            inspected.emplace_back();
            GPE::load(context, inspected.back(), XmlLoader::LoadInfo{std::to_string(i), "T", info.typeId});
        }

        context.pop();
    }
}

template <typename T, typename U>
void save(XmlSaver& context, const std::pair<T, U>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    GPE::save(context, inspected.first, XmlSaver::SaveInfo{"key", "unknown", 0});
    GPE::save(context, inspected.second, XmlSaver::SaveInfo{"value", "unknown", 0});

    context.pop();
}
template <typename T, typename U>
void load(XmlLoader& context, std::pair<T, U>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        GPE::load(context, inspected.first, XmlLoader::LoadInfo{"key", "unknown", 0});
        GPE::load(context, inspected.second, XmlLoader::LoadInfo{"value", "unknown", 0});

        context.pop();
    }
}


template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.appendAttribute(context.top(), "size", std::to_string(inspected.size()));

    size_t i = 0;
    for (const std::pair<const KEY, VALUE>& pair : inspected)
    {
        GPE::save(context, pair, XmlSaver::SaveInfo{std::to_string(i), "std::pair", 0});
        
        i++;
    }

    context.pop();
}
template <typename KEY, typename VALUE>
void save(XmlSaver& context, const std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info)
{
    GPE::save(context, inspected, XmlSaver::SaveInfo{info.name, "unknown", 0});
}

template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t size = std::stoll(findAttribValue(context.top(), "size"));

        inspected.clear();
        inspected.reserve(size);

        for (size_t i = 0; i < size; i++)
        {
            std::pair<KEY, VALUE> pair;
            GPE::load(context, pair, XmlLoader::LoadInfo{std::to_string(i), "std::pair", info.typeId});
            inspected.insert(pair);
        }

        context.pop();
    }
}
template <typename KEY, typename VALUE>
void load(XmlLoader& context, std::unordered_map<KEY, VALUE>& inspected, const rfk::Field& info)
{
    GPE::load(context, inspected, XmlLoader::LoadInfo{info.name, "unknown", 0});
}


} // namespace GPE