#pragma once

#include "Refureku/TypeInfo/Variables/Field.h"

namespace GPE
{
namespace DataInspector
{
    void startProperty(const char* name);
    void endProperty();

    template <typename T>
    void inspect(T& inspected)
    {
        inspected.inspect();
    }

    template <typename T>
    void inspect(T& inspected, const char* name)
    {
        startProperty(name);
        GPE::DataInspector::inspect(inspected);
        endProperty();
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