#include "Engine/Serialization/STDDataInspector.hpp"

#include "Engine/Serialization/Slider.hpp"

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, size_t& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::InputScalar("", ImGuiDataType_U64, &inspected));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, unsigned int& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::InputScalar("", ImGuiDataType_U32, &inspected));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, int& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::InputInt("", &inspected));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, float& inspected, const rfk::Field& info)
{
    bool isReadOnly = info.getProperty<ReadOnly>() != nullptr;
    if (isReadOnly)
    {
        ImGui::PushEnabled(false);
    }

    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        context.startProperty(info.name.c_str());
        context.setDirty(ImGui::SliderFloat(info.name.c_str(), &inspected, property->min, property->max));
        context.endProperty();
    }
    else
    {
        GPE::DataInspector::inspect(context, inspected, info.name.c_str());
    }

    if (isReadOnly)
    {
        ImGui::PopEnabled();
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, float& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::DragFloat("", &inspected));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, std::string& inspected, const char* name)
{
    context.startProperty(name);

    // TODO : to optimize / remove fixed size
    constexpr size_t bufferSize = 256;
    char             buffer[bufferSize];
    strcpy_s(buffer, inspected.c_str());
    context.setDirty(ImGui::InputText("", buffer, bufferSize));
    inspected       = buffer;

    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, bool& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::Checkbox("", &inspected));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, std::string& t)
{
    ImGui::Text(t.c_str());
}