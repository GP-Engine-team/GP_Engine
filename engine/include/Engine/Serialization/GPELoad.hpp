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
class Animation;
class Skeleton;
class Skin;

template <>
void load(XmlLoader& context, AmbiantComponent& data, const XmlLoader::LoadInfo& info);

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

template <>
void load(XmlLoader& context, Animation*& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Skeleton*& data, const XmlLoader::LoadInfo& info);

template <>
void load(XmlLoader& context, Skin*& data, const XmlLoader::LoadInfo& info);

} // namespace GPE
