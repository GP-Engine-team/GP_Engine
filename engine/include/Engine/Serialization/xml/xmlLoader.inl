#include "Engine/Serialization/xml/xmlLoader.hpp"

namespace GPE
{

// template<typename LOADED_TYPE>
// void XmlLoader::load(Node* parentNode, LOADED_TYPE& data, const FieldInfo& fieldInfo)
//{
//	Node* child = findSubNodeWithName(parentNode, fieldInfo.name);
//
//	if (checkType(child, fieldInfo.name, fieldInfo.type))
//	{
//		data.serialize(data, *this, child);
//	}
//}
//
// template<typename SERIALIZED_TYPE>
// inline void XmlLoader::serialize(Node* parentNode, SERIALIZED_TYPE& data, const FieldInfo& fieldInfo)
//{
//	load(parentNode, data, fieldInfo);
//}

template <typename T>
void load(XmlLoader& context, T& inspected, const rfk::Field& info)
{
    if (context.goToSubChild(info))
    {
        inspected.load(context);
        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, T& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        inspected.load(context);
        context.pop();
    }
}

template <typename T>
void load(XmlLoader& context, T*& inspected, const XmlLoader::LoadInfo& info)
{
    if (context.goToSubChild(info))
    {
        //context.saveAsString(std::to_string(size_t(inspected)), info);
        std::string str;
        if (context.loadFromStr(str, info))
        {
            std::size_t p = std::stoll(str);
            

            context.loadPtrData(inspected, info, p);
        }

        context.pop();
    }
}


} // namespace GPE