#pragma once

#include "InspectContext.hpp"

class IInspect : public ::rfk::Object
{
public:
    virtual void inspect(GPE::InspectContext&)
    {
    }
};