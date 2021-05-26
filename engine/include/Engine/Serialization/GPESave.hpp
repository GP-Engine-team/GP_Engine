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
struct GameObjectLinker;

template <>
void save(XmlSaver& context, const AmbiantComponent& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Prefab* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Prefab* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, const GameObjectLinker& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, const GameObjectLinker& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Shader* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Shader* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Material* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Material* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Mesh* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Mesh* const& data, const XmlSaver::SaveInfo& info);

template <>
void save(XmlSaver& context, Texture* const& data, const rfk::Field& info);

template <>
void save(XmlSaver& context, Texture* const& data, const XmlSaver::SaveInfo& info);

} // namespace GPE