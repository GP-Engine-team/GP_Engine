#include "Engine/Serialization/GPMSave.hpp"
#include "GPM/Transform.hpp"
#include "Engine/Serialization/xml/xmlSaver.hpp"

namespace GPE
{
//std::string transformToString(const GPM::Transform& data)
//{
//    data.model;
//}
//
//template <>
//void save(XmlSaver& context, const GPM::Transform& data, const rfk::Field& info)
//{
//    context.push(info);
//
//    context.saveAsString(transformToString(data), info);
//    GPE::save(context, data.model, XmlSaver::SaveInfo{"model", "Mat4", 0});
//
//    context.pop();
//}

template <>
void save(XmlSaver& context, const GPM::SplitTransform& data, const rfk::Field& info)
{
    context.push(XmlSaver::SaveInfo{info.name, "SplitTransform", 0});

    GPE::save(context, data.position, XmlSaver::SaveInfo{"position", "Vec3", 0});
    GPE::save(context, data.scale, XmlSaver::SaveInfo{"scale", "Vec3", 0});
    GPE::save(context, data.rotation, XmlSaver::SaveInfo{"rotation", "Quat", 0});

    context.pop();
}

void saveFloatArray(XmlSaver& context, const float* arr, size_t size, const XmlSaver::SaveInfo& info)
{
    std::string saved = "";

    for (size_t i = 0; i < size; i++)
    {
        saved += std::to_string(arr[i]);
        saved += '*';
    }

    context.saveAsString(saved, info);
}

template <>
void save(XmlSaver& context, const GPM::Vector3& data, const XmlSaver::SaveInfo& info)
{
    saveFloatArray(context, data.e, 3, info);
}

template <>
void save(XmlSaver& context, const GPM::Quaternion& data, const XmlSaver::SaveInfo& info)
{
    saveFloatArray(context, data.e, 4, info);
}

}
