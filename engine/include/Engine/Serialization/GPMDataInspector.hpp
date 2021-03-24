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
static void DataInspector::inspect(GPM::SplitTransform& inspected, const rfk::Field& info);

template <>
static void DataInspector::inspect(GPM::Vector3& inspected, const rfk::Field& info);

} // namespace GPE