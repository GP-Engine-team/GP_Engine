#pragma once

#include "DataInspector.hpp"

namespace GPE
{

template <>
static void DataInspector::inspect(int& t, const rfk::Field& info);

template <>
static void DataInspector::inspect(float& t, const rfk::Field& info);

template <>
static void DataInspector::inspect(std::string& t, const rfk::Field& info);

template <>
static void DataInspector::inspect(bool& t, const rfk::Field& info);

template <>
static void DataInspector::inspect(std::string& t);

} // namespace GPE