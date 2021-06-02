﻿
namespace GPE
{
namespace DataInspector
{

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected)
{
    inspected.inspect(context);
}

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const char* name)
{
    context.startProperty(name);
    inspected.inspect(context);
    context.endProperty();
}

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const std::string& name)
{
    DataInspector::inspect(inspected, name.c_str());
}

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

} // namespace DataInspector
} // namespace GPE
