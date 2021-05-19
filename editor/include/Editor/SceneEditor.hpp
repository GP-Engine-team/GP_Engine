#pragma once

#include <Engine/Intermediate/Viewers/SceneViewer.hpp>
#include <imgui/imguizmo.h>

namespace GPE
{
class IInspectable;
}

namespace Editor
{
class Editor;

class SceneEditor
{
    // Internal helper
    struct string_enum
    {
        const char*    string;
        unsigned short id;
    };

public:
    GPE::SceneViewer view;

private:
    string_enum         m_operations[3];
    string_enum         m_modes[2];

public:
    ImGuizmo::OPERATION activeOperation;
    ImGuizmo::MODE      activeMode;

private:
    void         captureInputs      (bool toggle);
    void         renderGizmoControls();
    void         checkCursor        (GPE::IInspectable*& inspectedObject);

public:
    SceneEditor(GPE::Scene& scene);

    void render(GPE::IInspectable*& inspectedObject);
};

} // namespace Editor
