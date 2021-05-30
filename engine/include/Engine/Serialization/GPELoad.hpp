#pragma once

#include "xml/xmlLoader.hpp"
#include <Engine/Resources/Linker.hpp>

namespace GPE
{
union AmbiantComponent;
class Prefab;
class Mesh;
class Shader;
class Material;
class Texture;
struct ColorRGBA;
struct ColorRGB;

template <typename T>
struct Linker;

class GameObject;

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, ColorRGBA& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, ColorRGB& data, const XmlLoader::LoadInfo& info);

template <typename T>
void load(XmlLoader& context, Linker<T>& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Prefab*& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Shader*& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Mesh*& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Material*& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Texture*& data, const XmlLoader::LoadInfo& info);

} // namespace GPE

#include <Engine/Serialization/GPELoad.inl>