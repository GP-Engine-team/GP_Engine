#include "Engine/Serialization/xml/xmlSaver.hpp"

template<typename T>
void XmlSaver::save(Node* parentNode, const T& saved, const FieldInfo& fieldInfo)
{
	Node* newNode = doc.allocate_node(rapidxml::node_element, "field");
	parentNode->append_node(newNode);

	appendAttribute(newNode, "name", fieldInfo.name);
	appendAttribute(newNode, "type", fieldInfo.type);

	saved.serialize(saved, *this, newNode);
}

template<typename LOADED_TYPE>
inline void XmlSaver::serialize(Node* parentNode, LOADED_TYPE& data, const FieldInfo& fieldInfo)
{
	save(parentNode, data, fieldInfo);
}