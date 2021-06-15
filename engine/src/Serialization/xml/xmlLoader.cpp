#include "Engine/Serialization/xml/xmlLoader.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Resources/Scene.hpp>

std::string XmlLoader::getValue(Node* node)
{
    return findAttribValue(node, "value");
}

rapidxml::xml_node<>* XmlLoader::findNodeWithMatchingAttribValue(rapidxml::xml_node<>* parentNode,
                                                                 const std::string&    attribName,
                                                                 const std::string&    askedValue)
{
    for (Node* child = parentNode->first_node(); child; child = child->next_sibling())
    {
        std::string value = findAttribValue(child, attribName);
        if (value == askedValue)
        {
            return child;
        }
    }
    return nullptr;
}

XmlLoader::Node* XmlLoader::findSubNode(Node* parentNode, const rfk::Field& info)
{
    return findNodeWithMatchingAttribValue(parentNode, "name", info.name);
}

XmlLoader::Node* XmlLoader::findSubNode(Node* parentNode, const LoadInfo& info)
{
    return findNodeWithMatchingAttribValue(parentNode, "name", info.name);
}

XmlLoader::Node* XmlLoader::findSubNode(const rfk::Field& info)
{
    return findSubNode(hierarchy.top(), info);
}

XmlLoader::Node* XmlLoader::findSubNode(const LoadInfo& info)
{
    return findSubNode(hierarchy.top(), info);
}

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

void XmlLoader::addLazy(void** data)
{
    lazyPtrs.push_back(data);
}

void XmlLoader::addPersistentPtr(void* ptr)
{
    alreadyLoadedPtrs.insert({ptr, LoadedPtr{LoadInfo{"persistentPtr", "void*", 0}, ptr}});
}

void XmlLoader::addConvertedPtr(void* oldPtr, void* newPtr)
{
    alreadyLoadedPtrs.insert({oldPtr, LoadedPtr{LoadInfo{"newPtr", "void*", 0}, newPtr}});
}

// Pass a weak ptr pointing to an old value
void XmlLoader::updateLazyPtr(void*& weak)
{
    auto it = alreadyLoadedPtrs.find({weak});
    if (it != alreadyLoadedPtrs.end())
    {

        // assert(it != alreadyLoadedPtrs.end()); //  must be valid, instance must be here
        weak = it->second.data;
    }
    else
    {
        weak = nullptr;
    }
}

void XmlLoader::updateLazyPtrs()
{
    for (void** weak : lazyPtrs)
    {
        updateLazyPtr(*weak);
    }

    lazyPtrs.clear();
}

void XmlLoader::updateLinker(GPE::GameObject& base)
{
    for (auto&& [key, value] : linkers)
    {
        if (GPE::GameObject* pGo = base.getGameObject(key))
            value->setData(*pGo);
    }
}

bool XmlLoader::goToSubChild(const LoadInfo& info)
{
    Node* child = findSubNode(info);
    if (child)
    {
        hierarchy.push(child);
        return true;
    }
    else
    {
        std::cout << "Node not found : " << info.name << " / " << info.typeId << " / " << info.typeName << std::endl;
        return false;
    }
}

bool XmlLoader::goToSubChild(const rfk::Field& info)
{
    if (info.type.archetype == nullptr)
    {
        return goToSubChild(LoadInfo{info.name, "empty", 0});
    }
    else
    {
        return goToSubChild(LoadInfo{info.name, info.type.archetype->name, info.type.archetype->id});
    }
}

XmlLoader::LoadInfo fieldToLoadInfo(rfk::Field const& field)
{
    if (field.type.archetype == nullptr)
        return XmlLoader::LoadInfo{field.name, "unknown", 0};
    else
        return XmlLoader::LoadInfo{field.name, field.type.archetype->name, field.type.archetype->id};
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
void load(XmlLoader& context, int& data, const XmlLoader::LoadInfo& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoi(value) : 0;
}

template <>
void load(XmlLoader& context, unsigned int& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoi(value) : 0;
}

template <>
void load(XmlLoader& context, unsigned int& data, const XmlLoader::LoadInfo& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoi(value) : 0;
}

template <>
void load(XmlLoader& context, size_t& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoull(value) : 0;
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
void load(XmlLoader& context, float& data, const XmlLoader::LoadInfo& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stof(value) : 0.f;
}

template <>
void load(XmlLoader& context, double& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stod(value) : 0.0;
}

template <>
void load(XmlLoader& context, double& data, const XmlLoader::LoadInfo& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stod(value) : 0.0;
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