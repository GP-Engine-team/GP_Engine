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

} // namespace GPE

#include "STDDataInspector.hpp"
#include "GPMDataInspector.hpp"