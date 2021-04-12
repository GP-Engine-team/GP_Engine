#include <Engine/Serialization/PhysXDataInspector.hpp>
#include <characterkinematic/PxCapsuleController.h>

/*template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxController*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxController*& inspected, const char* name)
{
    bool  changed = false;
    float test;
    changed |= DataInspector::inspect(context, test, "m_controller");
    return changed;
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxMaterial*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxMaterial*& inspected, const char* name)
{
    bool changed = false;
    // b |= DataInspector::inspect(context, inspected.
    return changed;
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxShape*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxShape*& inspected, const char* name)
{
    bool changed = false;
    // b |= DataInspector::inspect(context, inspected.
    return changed;
}*/