#include <Engine/Serialization/InspectContext.hpp>
#include "imgui.h"

using namespace GPE;

InspectContext::InspectContext()
{
    startProperty("", 0.f);
}

InspectContext::~InspectContext()
{
    endProperty();
}

void InspectContext::startProperty(const char* name, float indentLength)
{
    Node node;
    node.indentLength = indentLength;
    hierarchy.emplace_back(std::move(node));
    if (hierarchy.size() > indentStartIndex) // don't indent if first node
        ImGui::Indent(indentLength);
    ImGui::PushID(name);

    ImGui::Text(name);

    ImGui::SameLine(startingWidth * sepRatio);
    ImGui::PushItemWidth(startingWidth * (1.f - sepRatio));
}

void InspectContext::endProperty()
{
    if (hierarchy.size() > indentStartIndex) // don't unindent if first node
        ImGui::Unindent(hierarchy.back().indentLength);
    ImGui::PopItemWidth();
    ImGui::PopID();

    hasLastChanged = hierarchy.back().hasChanged;
    hierarchy.pop_back();
}

void InspectContext::setDirty()
{
    for (auto it = hierarchy.rbegin(); it != hierarchy.rend() && !it->hasChanged; it++)
    {
        it->hasChanged = true;
    }
}

void InspectContext::setDirty(bool shouldBecomeDirty)
{
    if (shouldBecomeDirty)
    {
        setDirty();
    }
}

bool InspectContext::isDirty() const 
{
    return hierarchy.back().hasChanged;
}

bool InspectContext::wasLastDirty() const
{
    return hasLastChanged;
}