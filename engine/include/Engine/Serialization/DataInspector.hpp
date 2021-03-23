#pragma once

#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
class DataInspector
{
public:

    template <typename T>
    static void inspect(T& t)
    {
        t.inspect();
    }

    template <typename T>
    static void inspect(T& t, const rfk::Field& info)
    {
        t.inspect();
    }
};

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