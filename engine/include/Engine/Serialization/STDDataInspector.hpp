#pragma once

#include "Engine/Serialization/DataInspector.hpp"

// in inl
#include "imgui.h"

namespace GPE::DataInspector
{
template <typename T>
void inspect(std::vector<T>& inspected, const rfk::Field& info);

// template <typename T, typename SIZE>
// void  inspect(std::array<T, SIZE>& inspected, const rfk::Field& info);

template <>
bool inspect(int& inspected, const rfk::Field& info);
template <>
bool inspect(int& inspected, const char* name);

template <>
bool inspect(float& inspected, const rfk::Field& info);
template <>
bool inspect(float& inspected, const char* name);

template <>
bool inspect(std::string& inspected, const rfk::Field& info);
template <>
bool inspect(std::string& inspected, const char* name);

template <>
bool inspect(bool& inspected, const rfk::Field& info);
template <>
bool inspect(bool& inspected, const char* name);

template <>
void inspect(std::string& inspected);

} // namespace GPE::DataInspector

#include "Engine/Serialization/STDDataInspector.inl"