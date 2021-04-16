/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "DataInspector.hpp"

namespace GPM
{
struct SplitTransform;
union Vector2;
union Vector3;
union Vector4;
union Quaternion;
} // namespace GPM

namespace GPE
{

template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::SplitTransform& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::SplitTransform& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Vector2& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Vector2& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Vector3& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Vector3& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Vector4& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Vector4& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Quaternion& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, GPM::Quaternion& inspected, const char* name);

} // namespace GPE