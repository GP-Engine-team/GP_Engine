#pragma once

#include "DataInspector.hpp"

namespace GPM
{
struct SplitTransform;
union Vector3;
union Quaternion;
} // namespace GPM

namespace GPE
{

template <>
bool DataInspector::inspect(GPM::SplitTransform& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPM::SplitTransform& inspected, const char* name);

template <>
bool DataInspector::inspect(GPM::Vector3& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPM::Vector3& inspected, const char* name);

template <>
bool DataInspector::inspect(GPM::Quaternion& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPM::Quaternion& inspected, const char* name);

} // namespace GPE