#pragma once

#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/InspectContext.hpp"

// in inl
#include <imgui/imgui.h>

namespace GPE::DataInspector
{

template <typename T>
bool inspect(GPE::InspectContext& context, std::vector<T>& inspected, const rfk::Field& info);

template <typename T>
bool inspect(GPE::InspectContext& context, std::vector<T>& inspected, const char* name);

// template <typename T, typename SIZE>
// void  inspect(std::array<T, SIZE>& inspected, const rfk::Field& info);

template <>
bool inspect(GPE::InspectContext& context, unsigned int& inspected, const rfk::Field& info);
template <>
bool inspect(GPE::InspectContext& context, unsigned int& inspected, const char* name);

template <>
bool inspect(GPE::InspectContext& context, int& inspected, const rfk::Field& info);
template <>
bool inspect(GPE::InspectContext& context, int& inspected, const char* name);

template <>
bool inspect(GPE::InspectContext& context, float& inspected, const rfk::Field& info);
template <>
bool inspect(GPE::InspectContext& context, float& inspected, const char* name);

template <>
bool inspect(GPE::InspectContext& context, std::string& inspected, const rfk::Field& info);
template <>
bool inspect(GPE::InspectContext& context, std::string& inspected, const char* name);

template <>
bool inspect(GPE::InspectContext& context, bool& inspected, const rfk::Field& info);
template <>
bool inspect(GPE::InspectContext& context, bool& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, std::string& inspected);

} // namespace GPE::DataInspector

#include "Engine/Serialization/STDDataInspector.inl"