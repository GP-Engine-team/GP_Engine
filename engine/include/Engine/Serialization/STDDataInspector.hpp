#pragma once

#include "DataInspector.hpp"

namespace GPE
{
namespace DataInspector
{
template <typename T>
void inspect(std::vector<T>& inspected, const rfk::Field& info);

//template <typename T, typename SIZE>
//void inspect(std::array<T, SIZE>& inspected, const rfk::Field& info);
}

template <>
void DataInspector::inspect(int& inspected, const rfk::Field& info);
template <>
void DataInspector::inspect(int& inspected, const char* name);

template <>
void DataInspector::inspect(float& inspected, const rfk::Field& info);
template <>
void DataInspector::inspect(float& inspected, const char* name);

template <>
void DataInspector::inspect(std::string& inspected, const rfk::Field& info);
template <>
void DataInspector::inspect(std::string& inspected, const char* name);

template <>
void DataInspector::inspect(bool& inspected, const rfk::Field& info);
template <>
void DataInspector::inspect(bool& inspected, const char* name);

template <>
void DataInspector::inspect(std::string& inspected);

} // namespace GPE