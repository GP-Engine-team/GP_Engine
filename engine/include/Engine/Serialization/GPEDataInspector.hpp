#pragma once

#include "DataInspector.hpp"
#include <Engine/Resources/Color.hpp>
#include <Engine/Resources/Type.hpp>
//#include <Engine/Resources/Linker.hpp>

namespace GPE
{
class Skin;
class Skeleton;
class Animation;
template <typename T>
struct Linker;

class GameObject;

template <>
void DataInspector::inspect(InspectContext& context, RGB& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, RGB& inspected);


template <>
void DataInspector::inspect(InspectContext& context, RGBA& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, RGBA& inspected);

template <>
void DataInspector::inspect(InspectContext& context, AmbiantComponent& inspected, const char* name);

// namespace DataInspector
//{
// template <typename T>
// bool inspect(InspectContext& context, Linker<T>& inspected, const rfk::Field& info);
//
// template <typename T>
// bool inspect(InspectContext& context, Linker<T>& inspected, const char* name);
//
// template <typename T>
// void inspect(InspectContext& context, Linker<T>& inspected);
//} // namespace DataInspector

template <>
void DataInspector::inspect(InspectContext& context, class Prefab*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Material*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Texture*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Shader*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Scene*& inspected, const rfk::Field& info);

template <>
void DataInspector::inspect(InspectContext& context, class Scene*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Skin*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Skeleton*& inspected, const char* name);

template <>
void DataInspector::inspect(InspectContext& context, class Animation*& inspected, const char* name);

} // namespace GPE

#include <Engine/Serialization/GPEDataInspector.inl>

