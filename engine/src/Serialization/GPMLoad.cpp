#include "Engine/Serialization/GPMLoad.hpp"
#include "GPM/Transform.hpp"

namespace GPE
{
template <>
void load(XmlLoader& context, GPM::SplitTransform& data, const rfk::Field& info)
{
    if (context.goToSubChild(info))
    {
        GPE::load(context, data.position, XmlLoader::LoadInfo{"position", "Vec3", 0});
        GPE::load(context, data.scale, XmlLoader::LoadInfo{"scale", "Vec3", 0});
        GPE::load(context, data.rotation, XmlLoader::LoadInfo{"rotation", "Quat", 0});
        context.pop();
    }
}

void loadFloatArray(XmlLoader& context, float* arr, size_t size, const XmlLoader::LoadInfo& info)
{
    std::string loaded = "";

    bool b = context.loadFromStr(loaded, info);
    assert(b);

    size_t off = 0;
    for (size_t i = 0; i < size; i++)
    {
        size_t n = loaded.find('*', off);
        std::string sub = loaded.substr(off, n - off);
        arr[i] = std::stof(sub);
        off = n + 1;
    }
}

template <>
void load(XmlLoader& context, GPM::Vector3& data, const XmlLoader::LoadInfo& info)
{
    loadFloatArray(context, data.e, 3, info);
}

template <>
void load(XmlLoader& context, GPM::Quaternion& data, const XmlLoader::LoadInfo& info)
{
    loadFloatArray(context, data.e, 4, info);
}

template <>
void load(XmlLoader& context, GPM::Matrix4& data, const XmlLoader::LoadInfo& info)
{
    loadFloatArray(context, data.e, 16, info);
}

template <>
void load(XmlLoader& context, GPM::Matrix4& data, const rfk::Field& info)
{
    GPE::load(context, data, fieldToLoadInfo(info));
}


} // namespace GPE
