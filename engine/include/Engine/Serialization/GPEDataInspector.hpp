#pragma once

#include "DataInspector.hpp"
#include <Engine/Resources/Color.hpp>
#include <Engine/Resources/Type.hpp>

namespace GPE
{
class Skin;
class Skeleton;

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
bool DataInspector::inspect(InspectContext& context, class GameObject*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class GameObject*& inspected, const char* name);

template <>
bool DataInspector::inspect(InspectContext& context, class Prefab*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Prefab*& inspected, const char* name);

template <>
bool DataInspector::inspect<Skin*>(InspectContext& context, Skin*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Skin*& inspected, const char* name);

template <>
bool DataInspector::inspect<Skeleton*>(InspectContext& context, Skeleton*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, class Skeleton*& inspected, const char* name);

//template <>
//bool DataInspector::inspect<Skin>(InspectContext& context, Skin*& inspected, const char* name)
//{
//
//}

} // namespace GPE