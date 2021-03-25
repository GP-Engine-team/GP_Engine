#pragma once

#include "DataInspector.hpp"

namespace GPM
{
struct SplitTransform;
union Vector3;
} // namespace GPM

namespace GPE
{

template <>
void DataInspector::inspect(GPM::SplitTransform& inspected, const rfk::Field& info);

template <>
void DataInspector::inspect(GPM::Vector3& inspected, const rfk::Field& info);

template <>
void DataInspector::inspect(GPM::Vector3& inspected, const char* name);

} // namespace GPE