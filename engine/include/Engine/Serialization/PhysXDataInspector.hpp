/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU
 * license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/InspectContext.hpp>

namespace physx
{
class PxController;
class PxMaterial;
class PxShape;
} // namespace physx

namespace GPE
{
class Collider;
enum class EShapeType;

/*template <>
bool DataInspector::inspect(GPE::InspectContext& context, physx::PxController*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, physx::PxController*& inspected, const char* name);*/

/*template <>
bool DataInspector::inspect(GPE::InspectContext& context, physx::PxMaterial*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, physx::PxMaterial*& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, physx::PxShape*& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, physx::PxShape*& inspected, const char* name);*/

template <>
void DataInspector::inspect(GPE::InspectContext& context, GPE::Collider& inspected, const char* name);

template <>
void DataInspector::inspect(GPE::InspectContext& context, GPE::EShapeType& inspected, const char* name);

} // namespace GPE
