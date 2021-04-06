#include "Engine/Serialization/xml/xmlLoader.hpp"
//#include "Engine/Serialization/xml/xmlUtilities.hpp"
//#include <iostream>
//
// XmlLoader::Node* XmlLoader::findNodeWithMatchingAttribValue(Node* parentNode, const std::string& attribName, const
// std::string& askedValue)
//{
//	for (Node* child = parentNode->first_node(); child; child = child->next_sibling())
//	{
//		std::string value = findAttribValue(child, attribName);
//		if (value == askedValue)
//		{
//			return child;
//		}
//	}
//	return nullptr;
//}
//
// bool XmlLoader::checkType(Node* node, const std::string& loadedName, const std::string& typeName)
//{
//	return getType(node) == typeName;
//}
//
// XmlLoader::Node* XmlLoader::findSubNodeWithName(XmlLoader::Node* parentNode, const std::string& name)
//{
//	return findNodeWithMatchingAttribValue(parentNode, "name", name);
//}
//
// std::string XmlLoader::getValue(Node* node)
//{
//	return findAttribValue(node, "value");
//}
//
// std::string XmlLoader::getType(Node* node)
//{
//	return findAttribValue(node, "type");
//}
//
// bool XmlLoader::loadFromStr(Node* parentNode, std::string& str, const FieldInfo& fieldInfo)
//{
//	Node* child = findSubNodeWithName(parentNode, fieldInfo.name);
//
//	if (checkType(child, fieldInfo.name, fieldInfo.type))
//	{
//		str = getValue(child);
//		return true;
//	}
//
//	return false;
//}
//
// template<>
// void XmlLoader::load(Node* parentNode, int& data, const FieldInfo& fieldInfo)
//{
//	std::string value;
//	data = loadFromStr(parentNode, value, fieldInfo) ? std::stoi(value) : 0;
//}
//
// template<>
// void XmlLoader::load(Node* parentNode, char& data, const FieldInfo& fieldInfo)
//{
//	std::string value;
//	data = loadFromStr(parentNode, value, fieldInfo) ? value[0] : 0;
//}
//
// template<>
// void XmlLoader::load(Node* parentNode, float& data, const FieldInfo& fieldInfo)
//{
//	std::string value;
//	data = loadFromStr(parentNode, value, fieldInfo) ? std::stof(value) : 0.f;
//}

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

namespace GPE
{

template <>
void load(XmlLoader& context, int& data, const rfk::Field& info)
{
    std::string value;
    data = context.loadFromStr(value, info) ? std::stoi(value) : 0;
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

} // namespace GPE