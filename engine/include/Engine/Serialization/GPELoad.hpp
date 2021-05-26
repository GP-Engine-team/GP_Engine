#pragma once

#include "xml/xmlLoader.hpp"

namespace GPE
{
union AmbiantComponent;
class Prefab;
class Mesh;
class Shader;
class Material;
class Texture;
struct GameObjectLinker;
struct ColorRGBA;
struct ColorRGB;

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, ColorRGBA& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, ColorRGB& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, GameObjectLinker& data, const XmlLoader::LoadInfo& info);

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