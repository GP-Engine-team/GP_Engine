#include <imgui.h>

#include <Engine/ECS/Component/AudioComponent.hpp>

#include <Engine/Serialization/AudioDataInspector.hpp>

using namespace GPE;

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, std::unordered_map<std::string, SourceData>& inspected,
                                 const char* name)
{
    context.startProperty(name, 0.f);

    for (auto& [key, value] : inspected)
    {
        // ImGui::Text(value.name.c_str());
        if (ImGui::TreeNode(value.sourceName.c_str()))
        {
            value.settings.inspect(context);
            value.isDirty = context.isDirty();
            ImGui::TreePop();
        }
    }

    context.endProperty();
}
