#pragma once

#include "Refureku/TypeInfo/Variables/Field.h"
#include "Engine/Serialization/InspectContext.hpp"

namespace GPE
{
namespace DataInspector
{
    void startProperty(const char* name);
    void endProperty();

    template <typename T>
    void inspect(GPE::InspectContext& context, T& inspected)
    {
        inspected.inspect();
    }

    /**
     * @brief Shows the content of the inspected object.
     * @tparam T The type of the inspected element.
     * @param inspected The inspected element.
     * @param name The displayed name of the inspected element.
     * @return True if the inspected element has been modified, false otherwise.
    */
    template <typename T>
    bool inspect(GPE::InspectContext& context, T& inspected, const char* name)
    {
        startProperty(name);
        const bool hasBeenModified = GPE::DataInspector::inspect(inspected);
        endProperty();
        return hasBeenModified;
    }

    template <typename T>
    void inspect(GPE::InspectContext& context, T& inspected, const std::string& name)
    {
        DataInspector::inspect(inspected, name.c_str());
    }

    /**
     * @brief Shows the content of the inspected object.
     * @tparam T T The type of the inspected element.
     * @param inspected The inspected element.
     * @param info The information about the inspected element (its name, its properties, etc).
     * @return True if the inspected element has been modified, false otherwise.
    */
    template <typename T>
    bool inspect(GPE::InspectContext& context, T& inspected, const rfk::Field& info)
    {
        inspected.inspect(context);
        return true;
    }
};

} // namespace GPE

#include "STDDataInspector.hpp"
#include "GPMDataInspector.hpp"