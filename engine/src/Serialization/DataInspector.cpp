#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

void GPE::DataInspector::startProperty(const char* name)
{
    ImGui::Text(name);

    ImGuiStyle& style = ImGui::GetStyle();
    style.ItemSpacing.x;

    ImVec2 v = ImGui::GetWindowSize();
    v.x *= 0.5;
    ImGui::SameLine(v.x);
    ImGui::PushItemWidth(v.x);
}

void GPE::DataInspector::endProperty()
{
    ImGui::PopItemWidth();
}