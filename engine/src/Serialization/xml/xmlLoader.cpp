#include "Engine/Serialization/xml/xmlLoader.hpp"

bool XmlLoader::loadFromStr(std::string& str, const rfk::Field& info)
{
    if (goToSubChild(info))
    {
        str = getValue(hierarchy.top());
        pop();
        return true;
    }
    return false;
}

bool XmlLoader::loadFromStr(std::string& str, const LoadInfo& info)
{
    if (goToSubChild(info))
    {
        str = getValue(hierarchy.top());
        pop();
        return true;
    }
    return false;
}

void XmlLoader::addLazy(void*& data)
{
    lazyPtrs.push_back(&data);
}

// Pass a weak ptr pointing to an old value
void XmlLoader::updateLazyPtr(void*& weak)
{
    auto it = alreadyLoadedPtrs.find({weak});
    assert(it != alreadyLoadedPtrs.end()); //  must be valid
    weak = it->second.data;
}

void XmlLoader::updateLazyPtrs()
{
    for (void** weak : lazyPtrs)
    {
        updateLazyPtr(*weak);
    }

    lazyPtrs.clear();
}

namespace GPE
{

template <>
void load(XmlLoader& context, int& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoi(value) : 0;
}

template <>
void load(XmlLoader& context, size_t& data, const XmlLoader::LoadInfo& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoull(value) : 0;
}

template <>
void load(XmlLoader& context, char& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? value[0] : 0;
}

template <>
void load(XmlLoader& context, std::string& data, const rfk::Field& info)
{
    context.loadFromStr(data, info);
}

template <>
void load(XmlLoader& context, std::string& data, const XmlLoader::LoadInfo& info)
{
    context.loadFromStr(data, info);
}

template <>
void load(XmlLoader& context, float& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stof(value) : 0.f;
}

template <>
void load(XmlLoader& context, bool& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? (value == "true") : false;
}

template <>
void load(XmlLoader& context, bool& data, const XmlLoader::LoadInfo& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? (value == "true") : false;
}

template <>
void load(XmlLoader& context, rfk::Method const*& data, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        size_t methodId;
        GPE::load(context, methodId, XmlLoader::LoadInfo{"id", "int", 0});
        data = rfk::entityCast<rfk::Method>(rfk::Database::getEntity(methodId));

        context.pop();
    }
}

} // namespace GPE