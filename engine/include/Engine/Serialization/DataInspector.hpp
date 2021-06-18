#pragma once

#include <Engine/Core/Tools/Format.hpp> //stringFormat
#include <Engine/Serialization/InspectContext.hpp>
#include <Refureku/TypeInfo/Variables/Field.h>

namespace GPE::DataInspector
{
// Usefull for the polymorphisme
template <class T>
void inspect(GPE::InspectContext& context, T*& inspected);

template <class T>
void inspect(GPE::InspectContext& context, T& inspected);

/**
 * @brief Shows the content of the inspected object.
 * @tparam T The type of the inspected element.
 * @param inspected The inspected element.
 * @param name The displayed name of the inspected element.
 */
// Usefull for the polymorphisme
template <typename T>
void inspect(GPE::InspectContext& context, T*& inspected, const char* name);

// Usefull for the polymorphisme
template <typename T>
void inspect(GPE::InspectContext& context, T*& inspected, const std::string& name);

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const char* name);

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const std::string& name);

/**
 * @brief Shows the content of the inspected object.
 * @tparam T The type of the inspected element.
 * @param inspected The inspected element.
 * @param info The information about the inspected element (its name, its properties, etc).
 */

// Usefull for the polymorphisme

template <typename T>
void inspect(GPE::InspectContext& context, T*& inspected, const rfk::Field& info);

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const rfk::Field& info);

} // namespace GPE::DataInspector

#include "DataInspector.inl"
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <Engine/Serialization/ReadOnly.hpp>
#include <Engine/Serialization/Separator.hpp>
//
#include "AudioDataInspector.hpp"
#include "GPEDataInspector.hpp"
#include "GPMDataInspector.hpp"
#include "PhysXDataInspector.hpp"
#include "STDDataInspector.hpp"
