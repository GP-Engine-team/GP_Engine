#pragma once

#include "DataInspector.hpp"
#include <Engine/Resources/Color.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Resources/Type.hpp>

namespace GPE
{
template <>
bool DataInspector::inspect(InspectContext& context, RGB& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, RGB& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, RGB& inspected);

template <>
bool DataInspector::inspect(InspectContext& context, RGBA& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, RGBA& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, RGBA& inspected);

template <>
bool DataInspector::inspect(InspectContext& context, AmbiantComponent& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, AmbiantComponent& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, Linker<GameObject>& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, Linker<GameObject>& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, Linker<GameObject>& inspected);

// template <typename T>
// bool DataInspector::inspect(InspectContext& context, Linker<T>& inspected, const rfk::Field& info);
//
// template <typename T>
// bool DataInspector::inspect(InspectContext& context, Linker<T>& inspected, const char* name);
//
// template <typename T>
// void DataInspector::inspect(InspectContext& context, Linker<T>& inspected);

template <>
bool DataInspector::inspect(InspectContext& context, class Prefab*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Prefab*& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, class Material*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Material*& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, class Texture*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Texture*& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, class Shader*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Shader*& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, class Scene*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Scene*& inspected, const char* name);

} // namespace GPE

#include <Engine/Serialization/GPEDataInspector.inl>