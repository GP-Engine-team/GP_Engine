#include "Engine/Serialization/STDDataInspector.hpp"

#include "imgui.h"
#include "Engine/Serialization/Slider.hpp"

template <typename T>
bool GPE::DataInspector::inspect(std::vector<T>& inspected, const rfk::Field& info)
{
    
    return false;
}

template <>
bool GPE::DataInspector::inspect(int& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(int& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = ImGui::InputInt("", &inspected);
    endProperty();
    return hasChanged;
}

template <>
bool GPE::DataInspector::inspect(float& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        startProperty(info.name.c_str());
        bool hasChanged = ImGui::SliderFloat(info.name.c_str(), &inspected, property->min, property->max);
        endProperty();
        return hasChanged;
    }
    else
    {
        return GPE::DataInspector::inspect(inspected, info.name.c_str());
    }
}

template <>
bool GPE::DataInspector::inspect(float& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = ImGui::InputFloat("", &inspected);
    endProperty();
    return hasChanged;
}

template <>
bool GPE::DataInspector::inspect(std::string& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(std::string& inspected, const char* name)
{
    startProperty(name);

    // TODO : to optimize / remove fixed size
    constexpr size_t bufferSize = 256;
    char             buffer[bufferSize];
    strcpy_s(buffer, inspected.c_str());
    bool hasChanged = ImGui::InputText("", buffer, bufferSize);
    inspected = buffer;

    endProperty();
    return hasChanged;
}

template <>
bool GPE::DataInspector::inspect(bool& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(bool& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = ImGui::Checkbox("", &inspected);
    endProperty();
    return hasChanged;
}

template <>
void GPE::DataInspector::inspect(std::string& t)
{
    return ImGui::Text(t.c_str());
}