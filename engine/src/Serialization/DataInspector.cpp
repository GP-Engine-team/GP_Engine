#include "Engine/Serialization/DataInspector.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Serialization/Slider.hpp"

template <>
static void GPE::DataInspector::inspect(class GameObject& t)
{
    std::list<Component*>& comps = t.getComponents();
    unsigned int           i     = 0;
    for (Component* comp : comps)
    {
        //ImGui::Text("Component : ");
        ImGui::PushID(i);
        comp->inspect();
        ImGui::PopID();
        i++;
    }

    t.inspect();
}

template <>
static void GPE::DataInspector::inspect(int& inspected, const rfk::Field& info)
{
    ImGui::InputInt(info.name.c_str(), &inspected);
}

template <>
static void GPE::DataInspector::inspect(float& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        ImGui::SliderFloat(info.name.c_str(), &inspected, property->min, property->max);
    }
    else
    {
        ImGui::InputFloat(info.name.c_str(), &inspected, 0.1);
    }
}

template <>
static void GPE::DataInspector::inspect(std::string& t, const rfk::Field& info)
{
    // TODO : to optimize / remove fixed size
    constexpr size_t bufferSize = 256;
    char             buffer[bufferSize];
    strcpy_s(buffer, t.c_str());
    ImGui::InputText(info.name.c_str(), buffer, bufferSize);
    t = buffer;
}

template <>
static void GPE::DataInspector::DataInspector::inspect(bool& t, const rfk::Field& info)
{
    ImGui::Checkbox(info.name.c_str(), &t);
}

template <>
static void GPE::DataInspector::inspect(std::string& t)
{
    ImGui::Text(t.c_str());
}