#pragma once

//#include "Engine/Serialization/FieldInfo.hpp"
#include "Engine/Serialization/xml/xmlUtilities.hpp"
#include "RapidXML/rapidxml.hpp"
#include <set>
#include <stack>

#include "Engine/Serialization/STDReflect.hpp"

//#include <string>
//
// class XmlLoader
//{
// public:
//    using Node = rapidxml::xml_node<>;
//
// protected:
//    rapidxml::xml_document<>& doc;
//
//    /**
//     * @brief Returns a node having the same attrib value as the one asked.
//     * @param parentNode The parent node of the searched node.
//     * @param attribName The attribute you want to verify.
//     * @param askedValue The value of the attribute you want to be equal.
//     * @return The node with same same askedValue for the given attribute. Returns nullptr if not found.
//     */
//    static XmlLoader::Node* XmlLoader::findNodeWithMatchingAttribValue(Node* parentNode, const std::string&
//    attribName,
//                                                                       const std::string& askedValue)
//    {
//        for (Node* child = parentNode->first_node(); child; child = child->next_sibling())
//        {
//            std::string value = findAttribValue(child, attribName);
//            if (value == askedValue)
//            {
//                return child;
//            }
//        }
//        return nullptr;
//    }
//
//    /**
//     * @brief Verify if the type of the node is the same as the given type.
//     * @param node The current context in which you verify the type of the loaded data.
//     * @param loadedName The name of the loaded instance.
//     * @param typeName The type you want to get.
//     * @return True if the type is the same, false otherwise.
//     */
//    static bool checkType(rapidxml::xml_node<>* node, const std::string& loadedName, const std::string& typeName);
//
//    /**
//     * @brief Finds a sub node with the given name.
//     * @param parentNode The current context in which you search the children.
//     * @param name The name you want the node to have.
//     * @return The node with the given name.
//     */
//    static Node* findSubNodeWithName(Node* parentNode, const std::string& name);
//    /**
//     * @brief Get the value as a string of an instance serialized into an xml node.
//     * @param node An instance saved as a node, containing the class information data.
//     * @return The type as a string.
//     */
//    static std::string getValue(Node* node);
//    /**
//     * @brief Get the type as a string of an instance serialized into an xml node.
//     * @param node An instance saved as a node, containing the class information data.
//     * @return The type as a string.
//     */
//    static std::string getType(Node* node);
//
// public:
//    XmlLoader(rapidxml::xml_document<>& d) : doc(d)
//    {
//    }
//
//    /**
//     * @brief Loads the data of any type. This function must be specialized for underlying primitive types.
//     * @tparam LOADED_TYPE The type of the loaded data.
//     * @param parentNode The current context of the class hierarchy.
//     * @param data The data you want to load.
//     * @param fieldInfo The class information about the data you want to load.
//     */
//    template <typename LOADED_TYPE>
//    void load(Node* parentNode, LOADED_TYPE& data, const FieldInfo& fieldInfo);
//
//    /**
//     * @brief Serializing with the Loader is equivalent to loading.
//     * @tparam SERIALIZED_TYPE The type of the serialized data.
//     * @param parentNode The current context of the class hierarchy.
//     * @param data The data you want to serialize.
//     * @param fieldInfo The class information about the data you want to serialize.
//     */
//    template <typename SERIALIZED_TYPE>
//    inline void serialize(Node* parentNode, SERIALIZED_TYPE& data, const FieldInfo& fieldInfo);
//
//    /**
//     * @brief Load the string corresponding to the data value.
//     * @param parentNode The current context of the class hierarchy.
//     * @param str The loaded string will be put here.
//     * @param fieldInfo The class information about the data you want to load.
//     * @return True if the data was loaded successfully, false otherwise.
//     */
//    bool loadFromStr(Node* parentNode, std::string& str, const FieldInfo& fieldInfo);
//};
//
///**
// * @brief Specialization for int data. See the original function for more comments.
// */
// template <>
// void XmlLoader::load(Node* parentNode, int& data, const FieldInfo& fieldInfo);
//
///**
// * @brief Specialization for char data. See the original function for more comments.
// */
// template <>
// void XmlLoader::load(Node* parentNode, char& data, const FieldInfo& fieldInfo);
//
///**
// * @brief Specialization for float data. See the original function for more comments.
// */
// template <>
// void XmlLoader::load(Node* parentNode, float& data, const FieldInfo& fieldInfo);
//
//#include "Engine/Serialization/xml/xmlLoader.inl"

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
    std::map<size_t, LoadedPtr> alreadyLoadedPtrs;

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

    void pop()
    {
        hierarchy.pop();
    }

    Node* top()
    {
        return hierarchy.top();
    }

    template <typename T>
    void loadPtrData(T*& data, const LoadInfo& info, std::size_t key);
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
 * @brief Specialization for char data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, char& data, const rfk::Field& info);

/**
 * @brief Specialization for std::string data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, std::string& data, const rfk::Field& info);

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

} // namespace GPE

#include <Refureku/Refureku.h>
#include <type_traits>

template <typename T>
void XmlLoader::loadPtrData(T*& data, const LoadInfo& info, std::size_t key)
{
    auto pair = alreadyLoadedPtrs.insert({key, LoadedPtr{info}});
    if (pair.second) // Has been inserted ?
    {
        if constexpr (std::is_base_of<rfk::Object, T>::value)
        {
            std::string       idStr     = findAttribValue(top(), "typeID");
            rfk::Class const* archetype = static_cast<rfk::Class const*>(rfk::Database::getEntity(std::stoull(idStr)));
            assert(archetype != 0);              // Type is not complete.
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

        LoadInfo newInfo{std::to_string(key), info.typeName, info.typeId};
        GPE::load(*this, *data, newInfo);

        hierarchy = std::move(otherContextHierarchy);
    }
    else
    {
        data = static_cast<T*>(pair.first->second.data); // CAST
    }
}

#include "Engine/Serialization/STDSave.hpp"
#include "Engine/Serialization/xml/xmlLoader.inl"