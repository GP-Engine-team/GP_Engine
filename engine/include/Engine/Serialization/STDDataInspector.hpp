#pragma once

#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/InspectContext.hpp>

#include <memory>
#include <vector>

// in inl
#include <imgui/imgui.h>

namespace GPE::DataInspector
{

template <typename T>
void inspect(GPE::InspectContext& context, std::vector<T>& inspected, const rfk::Field& info);

template <typename T>
void inspect(GPE::InspectContext& context, std::vector<T>& inspected, const char* name);

template <typename T>
void inspect(GPE::InspectContext& context, std::unique_ptr<T>& inspected, const rfk::Field& info);

template <typename T>
void inspect(GPE::InspectContext& context, std::unique_ptr<T>& inspected, const char* name);

template <typename T>
void inspect(GPE::InspectContext& context, std::unique_ptr<T>& inspected);

template <typename T>
void inspect(GPE::InspectContext& context, std::shared_ptr<T>& inspected, const rfk::Field& info);

template <typename T>
void inspect(GPE::InspectContext& context, std::shared_ptr<T>& inspected, const char* name);

template <typename T>
void inspect(GPE::InspectContext& context, std::shared_ptr<T>& inspected);

// template <typename T, typename SIZE>
// void  inspect(std::array<T, SIZE>& inspected, const rfk::Field& info);

template <>
void inspect(GPE::InspectContext& context, size_t& inspected, const rfk::Field& info);
template <>
void inspect(GPE::InspectContext& context, size_t& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, unsigned int& inspected, const rfk::Field& info);
template <>
void inspect(GPE::InspectContext& context, unsigned int& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, int& inspected, const rfk::Field& info);
template <>
void inspect(GPE::InspectContext& context, int& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, float& inspected, const rfk::Field& info);
template <>
void inspect(GPE::InspectContext& context, float& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, std::string& inspected, const rfk::Field& info);
template <>
void inspect(GPE::InspectContext& context, std::string& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, bool& inspected, const rfk::Field& info);
template <>
void inspect(GPE::InspectContext& context, bool& inspected, const char* name);

template <>
void inspect(GPE::InspectContext& context, std::string& inspected);

} // namespace GPE::DataInspector

#include "Engine/Serialization/STDDataInspector.inl"