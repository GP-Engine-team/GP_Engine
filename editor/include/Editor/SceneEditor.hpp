﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <Engine/Intermediate/Viewers/SceneViewer.hpp>
#include <imgui/imgui.h>
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
    GPE::SceneViewer    view;

private:
    string_enum         m_operations[3];
    string_enum         m_modes[2];

public:
    ImGuizmo::OPERATION activeOperation;
    ImGuizmo::MODE      activeMode;

private:
    void         captureInputs        (bool toggle);
    void         renderGizmoControlBar();
    void         renderGizmo          (float* inspectedTransfo);
    void         checkKeys            ();
    void         checkCursor          (GPE::GameObject*& inspectedObject);

private:
    class Editor* m_editorContext = nullptr;

public:
    SceneEditor(Editor& editorContext, GPE::Scene& scene);

    void render(GPE::GameObject*& inspectedObject);
};

} // namespace Editor
