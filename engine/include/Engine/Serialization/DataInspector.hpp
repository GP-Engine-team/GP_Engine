#pragma once

#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
namespace DataInspector
{
    template <typename T>
    void inspect(T& inspected)
    {
        inspected.inspect();
    }

    template <typename T>
    void inspect(T& inspected, const char* info)
    {
        inspected.inspect();
    }

    template <typename T>
    void inspect(T& inspected, const std::string& name)
    {
        DataInspector::inspect(inspected, name.c_str());
    }

    template <typename T>
    void inspect(T& inspected, const rfk::Field& info)
    {
        inspected.inspect();
    }
};

} // namespace GPE

#include "STDDataInspector.hpp"
#include "GPMDataInspector.hpp"