#pragma once

#include "Engine/Serialization/InspectContext.hpp"
#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
namespace DataInspector
{
template <class T>
void inspect(GPE::InspectContext& context, T& inspected);
/**
 * @brief Shows the content of the inspected object.
 * @tparam T The type of the inspected element.
 * @param inspected The inspected element.
 * @param name The displayed name of the inspected element.
 * @return True if the inspected element has been modified, false otherwise.
 */
template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const char* name);

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const std::string& name);

/**
 * @brief Shows the content of the inspected object.
 * @tparam T The type of the inspected element.
 * @param inspected The inspected element.
 * @param info The information about the inspected element (its name, its properties, etc).
 * @return True if the inspected element has been modified, false otherwise.
 */
template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const rfk::Field& info);

}; // namespace DataInspector

} // namespace GPE

#include <Engine/Serialization/ReadOnly.hpp>
#include <Engine/Serialization/Separator.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include "DataInspector.inl"
//
#include "AudioDataInspector.hpp"
#include "GPEDataInspector.hpp"
#include "GPMDataInspector.hpp"
#include "PhysXDataInspector.hpp"
#include "STDDataInspector.hpp"
