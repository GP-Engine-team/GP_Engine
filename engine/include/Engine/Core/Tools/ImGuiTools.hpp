/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h> //ImGuiItemFlags_Disabled, PushItemFlag

// Thank's to : https://github.com/ocornut/imgui/issues/211#issuecomment-812293268
namespace ImGui
{
inline void PushEnabled(bool _enabled)
{
    PushItemFlag(ImGuiItemFlags_Disabled, !_enabled);
    PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (_enabled ? 1.0f : 0.5f));
}

inline void PopEnabled()
{
    PopItemFlag();
    PopStyleVar();
}

enum class EHAlign
{
    Left   = 0,  // multiply by 0.01 = 0.f
    Middle = 50, // multiply by 0.01 = 0.5
    Right  = 100 // multiply by 0.01 = 1.f
};

enum class EVAlign
{
    Top    = 0,  // multiply by 0.01 = 0.f
    Middle = 50, // multiply by 0.01 = 0.5
    Bottom = 100 // multiply by 0.01 = 1.f
};

inline void SetNextElementLayout(float xWindowSizeRatio, float yWindowSizeRatio, const ImVec2& size,
                                 EHAlign hItemAlign = EHAlign::Middle, EVAlign vItemAlign = EVAlign::Middle)
{
    ImGui::SetCursorPosX(ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x +
                         (ImGui::GetWindowSize().x - ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x) *
                             xWindowSizeRatio -
                         size.x * (int)hItemAlign * 0.01f);
    ImGui::SetCursorPosY(ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y +
                         (ImGui::GetWindowSize().y - ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y) *
                             yWindowSizeRatio -
                         size.y * (int)vItemAlign * 0.01f);
}

} // namespace ImGui