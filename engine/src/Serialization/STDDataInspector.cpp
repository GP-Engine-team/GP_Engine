#include "Engine/Serialization/STDDataInspector.hpp"

#include "imgui.h"
#include "Engine/Serialization/Slider.hpp"

template <typename T>
void GPE::DataInspector::inspect(std::vector<T>& inspected, const rfk::Field& info)
{
    
}

template <>
void GPE::DataInspector::inspect(int& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(int& inspected, const char* name)
{
    startProperty(name);
    ImGui::InputInt("", &inspected);
    endProperty();
}

template <>
void GPE::DataInspector::inspect(float& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        startProperty(info.name.c_str());
        ImGui::SliderFloat(info.name.c_str(), &inspected, property->min, property->max);
        endProperty();
    }
    else
    {
        GPE::DataInspector::inspect(inspected, info.name.c_str());
    }
}

template <>
void GPE::DataInspector::inspect(float& inspected, const char* name)
{
    startProperty(name);
    ImGui::InputFloat("", &inspected);
    endProperty();
}

template <>
void GPE::DataInspector::inspect(std::string& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(std::string& inspected, const char* name)
{
    startProperty(name);

    // TODO : to optimize / remove fixed size
    constexpr size_t bufferSize = 256;
    char             buffer[bufferSize];
    strcpy_s(buffer, inspected.c_str());
    ImGui::InputText("", buffer, bufferSize);
    inspected = buffer;

    endProperty();
}

template <>
void GPE::DataInspector::inspect(bool& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(bool& inspected, const char* name)
{
    startProperty(name);
    ImGui::Checkbox("", &inspected);
    endProperty();
}

template <>
void GPE::DataInspector::inspect(std::string& t)
{
    ImGui::Text(t.c_str());
}