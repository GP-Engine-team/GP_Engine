#pragma once

#include "Engine/Serialization/xml/xmlUtilities.hpp"
#include "RapidXML/rapidxml.hpp"
#include <Refureku/Refureku.h>
#include "Refureku/TypeInfo/Variables/Field.h"
#include <stack>
#include <set>

class XmlSaver
{
public:
    using Node = rapidxml::xml_node<>;

    struct SaveInfo
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
        SaveInfo info;
        void* data;

        bool operator<(const LoadedPtr& rhs) const
        {
            return data < rhs.data;
        }

        bool operator==(const LoadedPtr& rhs) const
        {
            return data == rhs.data;
        }
    };
    std::set<LoadedPtr> alreadySavedPtrs;

public:

    XmlSaver(rapidxml::xml_document<>& d) : doc(d)
    {
        hierarchy.push(&d);
    }

    /**
     * @brief Save the string corresponding to the saved value.
     * @param parentNode The current context of the class hierarchy.
     * @param saved The string equivalent to the value, which will be saved.
     * @param fieldInfo The class information about the data you want to save.
     */
    void saveAsString(const std::string& saved, const rfk::Field& info);
    void saveAsString(const std::string& saved, const SaveInfo& info);

    void appendAttribute(Node* node, const std::string& name, const std::string& value);

    void pop()
    {
        hierarchy.pop();
    }

    Node* push(const std::string& name, const std::string& typeName, const rfk::uint64& typeId)
    {
        Node* newNode = doc.allocate_node(rapidxml::node_element, "field");

        top()->append_node(newNode);
        hierarchy.push(newNode);

        appendAttribute(newNode, "name", name);
        appendAttribute(newNode, "type", typeName);
        appendAttribute(newNode, "typeID", std::to_string(typeId));

        return newNode;
    }

    Node* push(const SaveInfo& info)
    {
        return push(info.name, info.typeName, info.typeId);
    }

    Node* push(const rfk::Field& info)
    {
        if (info.type.archetype == nullptr)
            return push(info.name, "unknown", 0);
        else 
            return push(info.name, info.type.archetype->name, info.type.archetype->id);
    }

    Node* top()
    {
        return hierarchy.top();
    }

    void print();

    template<typename T>
    bool savePtrData(T* data, const SaveInfo& info);

    void addWeakPtr(void* ptr)
    {
        alreadySavedPtrs.insert(LoadedPtr{SaveInfo{"weakPtr", "unknown", 0}, ptr});
    }
};

inline XmlSaver::SaveInfo fieldToSaveInfo(rfk::Field const& field)
{
    if (field.type.archetype == nullptr)
        return XmlSaver::SaveInfo{field.name, "unknown", 0};
    else
        return XmlSaver::SaveInfo{field.name, field.type.archetype->name, field.type.archetype->id};
}

namespace GPE
{

template <typename T>
void save(XmlSaver& context, const T& inspected, const rfk::Field& info)
{
    context.push(info);

    inspected.save(context);

    context.pop();
}

template <typename T>
void save(XmlSaver& context, const T& inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    inspected.save(context);

    context.pop();
}

template <typename T>
void save(XmlSaver& context, T* const& inspected, const rfk::Field& info)
{
    if (inspected == nullptr)
        return;

    context.push(info);

    context.saveAsString(std::to_string(size_t(inspected)), info);
    context.savePtrData(inspected, fieldToSaveInfo(info));

    context.pop();
}

template <typename T>
void save(XmlSaver& context, T* const & inspected, const XmlSaver::SaveInfo& info)
{
    context.push(info);

    context.saveAsString(std::to_string(size_t(inspected)), info);
    context.savePtrData(inspected, info);

    context.pop();
}

/**
 * @brief Specialization for std::string data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const std::string& inspected, const rfk::Field& info);

/**
 * @brief Specialization for int data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const int& inspected, const rfk::Field& info);

/**
 * @brief Specialization for size_t data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const size_t& inspected, const XmlSaver::SaveInfo& info);

/**
 * @brief Specialization for char data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const char& inspected, const rfk::Field& info);

/**
 * @brief Specialization for float data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const float& inspected, const rfk::Field& info);

/**
 * @brief Specialization for bool data. See the original function for more comments.
 */
template <>
void save(XmlSaver& context, const bool& inspected, const rfk::Field& info);

template <>
void save(XmlSaver& context, rfk::Object* const & inspected, const rfk::Field& info);
template <>
void save(XmlSaver& context, rfk::Object* const& inspected, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, rfk::Method const* const& data, const XmlSaver::SaveInfo& info);

} // namespace GPE

template <typename T>
bool XmlSaver::savePtrData(T* data, const SaveInfo& info)
{
    bool hasBeenInserted = alreadySavedPtrs.insert(LoadedPtr{info, data}).second;
    if (hasBeenInserted)
    {
        std::stack<Node*> otherContextHierarchy;
        otherContextHierarchy.push(&doc);
        std::swap(otherContextHierarchy, hierarchy);

        if constexpr (std::is_base_of<rfk::Object, T>::value)
        {
            assert(info.typeId != 0);
        }

        SaveInfo newInfo{std::to_string(std::size_t(data)), info.typeName, info.typeId};
        GPE::save(*this, *data, newInfo);

        hierarchy = std::move(otherContextHierarchy);

        return true;
    }
    return false;
}

#include "Engine/Serialization/STDSave.hpp"
#include "Engine/Serialization/GPMSave.hpp"