#include <imgui.h>
#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
template <typename T>
void save(class XmlSaver& context, const std::list<T*>& saved, const rfk::Field& info)
{
    GPE::save(context, saved, XmlSaver::SaveInfo{info.name, "std::list<T*>", 0});
}

template <typename T>
void load(XmlLoader& context, std::list<T*>& loaded, const rfk::Field& info)
{
    GPE::load(context, loaded, XmlLoader::LoadInfo{info.name, "std::list<T*>", 0});
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


} // namespace GPE