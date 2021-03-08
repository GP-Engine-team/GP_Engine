#include "Engine/Serialization/xml/xmlSaver.hpp"
#include "Engine/Serialization/FieldInfo.hpp"
#include "Engine/Serialization/xml/xmlUtilities.hpp"
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_print.hpp"
#include <iostream>
#include <string>

using namespace rapidxml;

void XmlSaver::appendAttribute(Node* node, const std::string& name, const std::string& value)
{
    node->append_attribute(doc.allocate_attribute(doc.allocate_string(name.c_str(), name.size()),
                                                  doc.allocate_string(value.c_str(), value.size()), name.size(),
                                                  value.size()));
}

void XmlSaver::saveAsString(Node* parentNode, const std::string& saved, const FieldInfo& fieldInfo)
{
    Node* newNode = doc.allocate_node(rapidxml::node_element, "field");
    parentNode->append_node(newNode);

    appendAttribute(newNode, "name", fieldInfo.name);
    appendAttribute(newNode, "type", fieldInfo.type);
    appendAttribute(newNode, "value", saved);
}

template <>
void XmlSaver::save(Node* parentNode, const std::string& data, const FieldInfo& fieldInfo)
{
    saveAsString(parentNode, data, fieldInfo);
}

template <>
void XmlSaver::save(Node* parentNode, const int& data, const FieldInfo& fieldInfo)
{
    saveAsString(parentNode, std::to_string(data), fieldInfo);
}

template <>
void XmlSaver::save(Node* parentNode, const char& data, const FieldInfo& fieldInfo)
{
    saveAsString(parentNode, std::string(1, data), fieldInfo);
}

template <>
void XmlSaver::save(Node* parentNode, const float& data, const FieldInfo& fieldInfo)
{
    saveAsString(parentNode, std::to_string(data), fieldInfo);
}
