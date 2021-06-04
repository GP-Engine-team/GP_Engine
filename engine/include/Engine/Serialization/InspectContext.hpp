#pragma once

#include <vector>
#include <Refureku/Refureku.h>

namespace GPE
{

class InspectContext
{
private:
    struct Node
    {
        float indentLength = 0;
        bool hasChanged = false;
    };

    std::vector<Node> hierarchy;
    bool              hasLastChanged = false;
    float             sepRatio       = 0.5f;
    size_t            indentStartIndex = 2;

public:
    class GameObject* lastComponentOwner = nullptr;

    InspectContext();
    ~InspectContext();
    float startingWidth = 0.f;

    void startProperty(const char* name, float indentLength = 16.f);
    void endProperty();

    void startPropertyGroup(const char* name, float indentLength = 16.f);
    void endPropertyGroup();

    void setDirty();
    void setDirty(bool shouldBecomeDirty);
    bool isDirty() const;
    bool wasLastDirty() const;

    template <typename FUNCTOR>
    void applyProperties(const rfk::Field& info, FUNCTOR&& f);
};

} // namespace GPE

#include <Engine/Serialization/ReadOnly.hpp>
#include <Engine/Serialization/Separator.hpp>
#include <Engine/Core/Tools/ImGuiTools.hpp>
#include <imgui/imgui.h>

template <typename FUNCTOR>
void GPE::InspectContext::applyProperties(const rfk::Field& info, FUNCTOR&& functor)
{
    Separator const* separator = info.getProperty<Separator>();
    if (separator != nullptr && separator->startSeparation)
    {
        ImGui::Separator();
    }

    ReadOnly const* property = info.getProperty<ReadOnly>();
    if (property)
    {
        ImGui::PushEnabled(false);
        functor();
        ImGui::PopEnabled();
    }
    else
    {
        functor();
    }

    if (separator != nullptr && separator->endSeparation)
    {
        ImGui::Separator();
    }
}