#pragma once

#include "xml/xmlSaver.hpp"

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

template <typename T>
void save(XmlSaver& context, const Linker<T>& data, const rfk::Field& info);

// Linker of component
template <typename T>
void save(XmlSaver& context, const Linker<T>& data, const XmlSaver::SaveInfo& info);

// Linker of gameObject
template <>
void save(XmlSaver& context, const Linker<GameObject>& data, const XmlSaver::SaveInfo& info);

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

#include <Engine/Serialization/GPESave.inl>