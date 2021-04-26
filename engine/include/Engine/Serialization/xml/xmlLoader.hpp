#pragma once

#include <Refureku/Refureku.h>
#include "Engine/Serialization/xml/xmlUtilities.hpp"
#include "RapidXML/rapidxml.hpp"
#include <set>
#include <stack>
#include "Engine/Serialization/STDReflect.hpp"
#include "Refureku/TypeInfo/Variables/Field.h"
#include <map>
#include <stack>

class XmlLoader
{
public:
    using Node = rapidxml::xml_node<>;

    struct LoadInfo
    {
        std::string name;
        std::string typeName;
        size_t      typeId;
    };

protected:
    rapidxml::xml_document<>& doc;

    std::stack<Node*> hierarchy;

    struct LoadedPtr
    {
        LoadInfo info;
        void*    data = nullptr;
    };
    // key is the saved ptr
    std::map<void*, LoadedPtr> alreadyLoadedPtrs;

protected:
    /**
     * @brief Get the value as a string of an instance serialized into an xml node.
     * @param node An instance saved as a node, containing the class information data.
     * @return The type as a string.
     */
    static std::string getValue(Node* node)
    {
        return findAttribValue(node, "value");
    }

    /**
     * @brief Returns a node having the same attrib value as the one asked.
     * @param parentNode The parent node of the searched node.
     * @param attribName The attribute you want to verify.
     * @param askedValue The value of the attribute you want to be equal.
     * @return The node with same same askedValue for the given attribute. Returns nullptr if not found.
     */
    static rapidxml::xml_node<>* findNodeWithMatchingAttribValue(rapidxml::xml_node<>* parentNode,
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

    static Node* findSubNode(Node* parentNode, const rfk::Field& info)
    {
        return findNodeWithMatchingAttribValue(parentNode, "name", info.name);
    }

    static Node* findSubNode(Node* parentNode, const LoadInfo& info)
    {
        return findNodeWithMatchingAttribValue(parentNode, "name", info.name);
    }

    Node* findSubNode(const rfk::Field& info)
    {
        return findSubNode(hierarchy.top(), info);
    }

    Node* findSubNode(const LoadInfo& info)
    {
        return findSubNode(hierarchy.top(), info);
    }

public:
    XmlLoader(rapidxml::xml_document<>& d) : doc(d)
    {
        hierarchy.push(&d);
    }

    /**
     * @brief Load the string corresponding to the data value.
     * @param str The loaded string will be put here.
     * @param fieldInfo The class information about the data you want to load.
     * @return True if the data was loaded successfully, false otherwise.
     */
    bool loadFromStr(std::string& str, const rfk::Field& info);
    bool loadFromStr(std::string& str, const LoadInfo& info);

    bool goToSubChild(const LoadInfo& info)
    {
        Node* child = findSubNode(info);
        if (child)
        {
            hierarchy.push(child);
            return true;
        }
        else
        {
            std::cout << "Node not found" << std::endl;
            return false;
        }
    }

    bool goToSubChild(const rfk::Field& info)
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

    void pop()
    {
        hierarchy.pop();
    }

    Node* top()
    {
        return hierarchy.top();
    }

    template <typename T>
    void loadPtrData(T*& data, const LoadInfo& info, void* key);

private:
    std::vector<void**> lazyPtrs;

public:
    void addLazy(void*& data);

    void addPersistentPtr(void* ptr)
    {
        alreadyLoadedPtrs.insert({ptr, LoadedPtr{LoadInfo{"persistentPtr", "void*", 0}, ptr}});
    }

    // Pass a weak ptr pointing to an old value
    void updateLazyPtr(void*& weak);

    void updateLazyPtrs();
};

namespace GPE
{

template <typename T>
void load(XmlLoader& context, T& inspected, const rfk::Field& info);

template <typename T>
void load(XmlLoader& context, T& inspected, const XmlLoader::LoadInfo& info);

template <typename T>
void load(XmlLoader& context, T*& inspected, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for int data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, int& data, const rfk::Field& info);

/**
 * @brief Specialization for size_t data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, size_t& data, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for char data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, char& data, const rfk::Field& info);

/**
 * @brief Specialization for std::string data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, std::string& data, const rfk::Field& info);

template <>
void load(XmlLoader& context, std::string& data, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for float data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, float& data, const rfk::Field& info);

/**
 * @brief Specialization for bool data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, bool& data, const rfk::Field& info);
template <>
void load(XmlLoader& context, bool& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, rfk::Method const*& data, const XmlLoader::LoadInfo& info);

} // namespace GPE

#include <Refureku/Refureku.h>
#include <type_traits>

inline XmlLoader::LoadInfo fieldToLoadInfo(rfk::Field const& field)
{
    if (field.type.archetype == nullptr)
        return XmlLoader::LoadInfo{field.name, "unknown", 0};
    else
        return XmlLoader::LoadInfo{field.name, field.type.archetype->name, field.type.archetype->id};
}

template <typename T>
void XmlLoader::loadPtrData(T*& data, const LoadInfo& info, void* key)
{
    auto pair = alreadyLoadedPtrs.insert({key, LoadedPtr{info}});
    if (pair.second) // Has been inserted ?
    {
        if constexpr (std::is_base_of<rfk::Object, T>::value)
        {
            std::string       idStr     = findAttribValue(top(), "typeID");
            size_t            s         = (std::stoull(idStr));
            rfk::Class const* archetype = static_cast<rfk::Class const*>(rfk::Database::getEntity(s));
            assert(archetype != 0);              // Type is not complete. Try adding corresponding include in game.cpp
            data = archetype->makeInstance<T>(); // TODO : Call custom instantiator
        }
        else 
        {
            data = new T();
        }

        pair.first->second.data     = data;

        assert(data != nullptr); // Type is not default constructible.

        std::stack<Node*> otherContextHierarchy;
        otherContextHierarchy.push(&doc);
        std::swap(otherContextHierarchy, hierarchy);

        LoadInfo newInfo{std::to_string(size_t(key)), info.typeName, info.typeId};
        GPE::load(*this, *data, newInfo);

        hierarchy = std::move(otherContextHierarchy);
    }
    else
    {
        data = static_cast<T*>(pair.first->second.data); // CAST
    }
}

#include "Engine/Serialization/STDSave.hpp"
#include "Engine/Serialization/GPMLoad.hpp"
#include "Engine/Serialization/xml/xmlLoader.inl"