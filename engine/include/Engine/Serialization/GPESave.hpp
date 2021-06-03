#pragma once

#include "xml/xmlSaver.hpp"

namespace GPE
{
union AmbiantComponent;
class Prefab;
class Mesh;
class Shader;
class Material;
class Texture;
class Skeleton;
class Skin;
class Animation;
struct ColorRGBA;

template <typename T>
struct Linker;

class GameObject;

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, const ColorRGBA& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, const ColorRGBA& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, const ColorRGB& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, const ColorRGB& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Prefab* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Prefab* const& data, const XmlSaver::SaveInfo& info);

// template <typename T>
// void save(XmlSaver& context, const Linker<T>& data, const rfk::Field& info);

template <typename T>
void save(XmlSaver& context, const Linker<T>& data, const XmlSaver::SaveInfo& info);

template <>
void save<GPE::Linker<GPE::GameObject>>(XmlSaver& context, const GPE::Linker<GPE::GameObject>& data,
                                        const XmlSaver::SaveInfo& info);

template <>
void save<GPE::GameObject>(XmlSaver& context, const GPE::Linker<GPE::GameObject>& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Shader* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Material* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Mesh* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Texture* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Animation* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Skeleton* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Skin* const& data, const XmlSaver::SaveInfo& info);

} // namespace GPE

#include <string>
#include <Engine/Serialization/GPESave.inl>

