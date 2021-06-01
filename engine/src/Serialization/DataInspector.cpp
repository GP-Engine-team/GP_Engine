#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

void GPE::DataInspector::startProperty(const char* name)
{
    ImGui::Indent(16.0f);
    ImGui::PushID(name);

    ImGui::Text(name);

    ImVec2 v = ImGui::GetWindowSize();
    v.x *= .5f;
    ImGui::SameLine(v.x);
    ImGui::PushItemWidth(v.x);
}

void GPE::DataInspector::endProperty()
{
    ImGui::Unindent(16.0f);
    ImGui::PopItemWidth();
    ImGui::PopID();
}
