#pragma once

//#include "Engine/Serialization/FieldInfo.hpp"
//#include "Engine/Serialization/xml/xmlUtilities.hpp"
//#include "RapidXML/rapidxml.hpp"
//#include <string>
//
//class XmlSaver
//{
//public:
//    using Node = rapidxml::xml_node<>;
//
//protected:
//    rapidxml::xml_document<>& doc;
//
//    void appendAttribute(Node* node, const std::string& name, const std::string& value);
//
//public:
//    XmlSaver(rapidxml::xml_document<>& d) : doc(d)
//    {
//    }
//
//    /**
//     * @brief Save the data of any type. This function must be specialized for underlying primitive types.
//     * @tparam LOADED_TYPE The type of the saved data.
//     * @param parentNode The current context of the class hierarchy.
//     * @param data The data you want to save.
//     * @param fieldInfo The class information about the data you want to save.
//     */
//    template <typename T>
//    void save(Node* parentNode, const T& saved, const FieldInfo& fieldInfo);
//
//    /**
//     * @brief Save the string corresponding to the saved value.
//     * @param parentNode The current context of the class hierarchy.
//     * @param saved The string equivalent to the value, which will be saved.
//     * @param fieldInfo The class information about the data you want to save.
//     */
//    void saveAsString(Node* parentNode, const std::string& saved, const FieldInfo& fieldInfo);
//
//    /**
//     * @brief Serializing with the Saver is equivalent to saving.
//     * @tparam SERIALIZED_TYPE The type of the serialized data.
//     * @param parentNode The current context of the class hierarchy.
//     * @param data The data you want to serialize.
//     * @param fieldInfo The class information about the data you want to serialize.
//     */
//    template <typename LOADED_TYPE>
//    inline void serialize(Node* parentNode, LOADED_TYPE& data, const FieldInfo& fieldInfo);
//};
//
///**
// * @brief Specialization for std::string data. See the original function for more comments.
// */
//template <>
//void XmlSaver::save(Node* parentNode, const std::string& saved, const FieldInfo& fieldInfo);
//
///**
// * @brief Specialization for int data. See the original function for more comments.
// */
//template <>
//void XmlSaver::save(Node* parentNode, const int& saved, const FieldInfo& fieldInfo);
//
///**
// * @brief Specialization for char data. See the original function for more comments.
// */
//template <>
//void XmlSaver::save(Node* parentNode, const char& saved, const FieldInfo& fieldInfo);
//
//template <>
//void XmlSaver::save(Node* parentNode, const float& saved, const FieldInfo& fieldInfo);
//
//#include "Engine/Serialization/xml/xmlSaver.inl"

#include "Refureku/TypeInfo/Variables/Field.h"

class XmlSaver
{
};

namespace GPE
{

template <typename T>
void save(XmlSaver& context, T& inspected, const rfk::Field& info)
{
}

} // namespace GPE