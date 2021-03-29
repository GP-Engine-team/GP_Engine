#pragma once

#include "DataInspector.hpp"

namespace GPE
{
namespace DataInspector
{
template <typename T>
bool inspect(GPE::InspectContext& context, std::vector<T>& inspected, const rfk::Field& info);

//template <typename T, typename SIZE>
//void inspect(std::array<T, SIZE>& inspected, const rfk::Field& info);
}

template <>
bool DataInspector::inspect(GPE::InspectContext& context, int& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, int& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, float& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, float& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, std::string& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, std::string& inspected, const char* name);

template <>
bool DataInspector::inspect(GPE::InspectContext& context, bool& inspected, const rfk::Field& info);
template <>
bool DataInspector::inspect(GPE::InspectContext& context, bool& inspected, const char* name);

template <>
void DataInspector::inspect(GPE::InspectContext& context, std::string& inspected);

} // namespace GPE