/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Serialization/STDReflect.hpp"
#include "Engine/Serialization/xml/xmlUtilities.hpp"
#include "RapidXML/rapidxml.hpp"
#include "Refureku/TypeInfo/Variables/Field.h"
#include <Refureku/Refureku.h>
#include <map>
#include <set>
#include <stack>
#include <type_traits>

namespace GPE
{
class GameObject;
struct GameObjectLinker;
} // namespace GPE

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

public:
    std::map<std::string, GPE::GameObjectLinker*> gameObjectLinkers;

protected:
    /**
     * @brief Get the value as a string of an instance serialized into an xml node.
     * @param node An instance saved as a node, containing the class information data.
     * @return The type as a string.
     */
    static std::string getValue(Node* node);

    /**
     * @brief Returns a node having the same attrib value as the one asked.
     * @param parentNode The parent node of the searched node.
     * @param attribName The attribute you want to verify.
     * @param askedValue The value of the attribute you want to be equal.
     * @return The node with same same askedValue for the given attribute. Returns nullptr if not found.
     */
    static rapidxml::xml_node<>* findNodeWithMatchingAttribValue(rapidxml::xml_node<>* parentNode,
                                                                 const std::string&    attribName,
                                                                 const std::string&    askedValue);

    static Node* findSubNode(Node* parentNode, const rfk::Field& info);

    static Node* findSubNode(Node* parentNode, const LoadInfo& info);

    Node* findSubNode(const rfk::Field& info);

    Node* findSubNode(const LoadInfo& info);

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

    bool goToSubChild(const LoadInfo& info);

    bool goToSubChild(const rfk::Field& info);

    inline void pop()
    {
        hierarchy.pop();
    }

    inline Node* top()
    {
        return hierarchy.top();
    }

    template <typename T>
    void loadPtrData(T*& data, const LoadInfo& info, void* key);

private:
    std::vector<void**> lazyPtrs;

public:
    void addLazy(void*& data);
    void addLazy(void** data);

    void addPersistentPtr(void* ptr);
    void addConvertedPtr(void* oldPtr, void* newPtr);

    // Pass a weak ptr pointing to an old value
    void updateLazyPtr(void*& weak);

    void updateLazyPtrs();
    void updateGameObjectLinker(GPE::GameObject& base);
};

XmlLoader::LoadInfo fieldToLoadInfo(rfk::Field const& field);

namespace GPE
{

template <typename T>
void load(XmlLoader& context, T& inspected, const XmlLoader::LoadInfo& info);

template <typename T>
void load(XmlLoader& context, T& inspected, const rfk::Field& info);

template <typename T>
void load(XmlLoader& context, T*& inspected, const XmlLoader::LoadInfo& info);

/**
 * @brief Specialization for int data. See the original function for more comments.
 */
template <>
void load(XmlLoader& context, int& data, const rfk::Field& info);
template <>
void load(XmlLoader& context, int& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, unsigned int& data, const rfk::Field& info);

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

#include "Engine/Serialization/GPELoad.hpp"
#include "Engine/Serialization/GPMLoad.hpp"
#include "Engine/Serialization/STDLoad.hpp"
#include "Engine/Serialization/xml/xmlLoader.inl"