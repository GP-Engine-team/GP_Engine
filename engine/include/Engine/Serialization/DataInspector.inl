
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
bool inspect(GPE::InspectContext& context, T& inspected, const char* name)
{
    startProperty(name);
    const bool hasBeenModified = GPE::DataInspector::inspect(inspected);
    endProperty();
    return hasBeenModified;

    // inspected.inspect(context);
    // return false;
}

template <typename T>
void inspect(GPE::InspectContext& context, T& inspected, const std::string& name)
{
    DataInspector::inspect(inspected, name.c_str());
}

template <typename T>
bool inspect(GPE::InspectContext& context, T& inspected, const rfk::Field& info)
{
    inspected.inspect(context);
    return true;
}

} // namespace DataInspector
} // namespace GPE
