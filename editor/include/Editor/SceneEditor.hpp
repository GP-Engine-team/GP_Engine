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

// TODO: Add a thin top-bar with translation/rotation/scale options
// TODO: Add a thin top-bar with camera orthographic/perspectives options
class SceneEditor
{
public:
    GPE::SceneViewer view;

private:
    ImGuizmo::OPERATION m_active;
    ImGuizmo::MODE      m_mode;

private:
    void captureInputs(bool toggle);
    void checkCursor(GPE::IInspectable*& inspectedObject);

public:
    SceneEditor(GPE::Scene& scene);

    void render(GPE::IInspectable*& inspectedObject);
};

} // namespace Editor
