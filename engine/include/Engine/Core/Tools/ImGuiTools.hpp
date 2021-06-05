/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
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
    ImGui::SetCursorPosX(ImGui::GetStyle().FramePadding.x + ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x +
                         (ImGui::GetWindowSize().x - ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x) *
                             xWindowSizeRatio -
                         size.x * (int)hItemAlign * 0.01f);

    ImGui::SetCursorPosY(ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y +
                         (ImGui::GetWindowSize().y - ImGui::GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y) *
                             yWindowSizeRatio -
                         size.y * (int)vItemAlign * 0.01f);
}

static inline float ImAcos01(float x)
{
    if (x <= 0.0f)
        return IM_PI * 0.5f;
    if (x >= 1.0f)
        return 0.0f;
    return ImAcos(x);
}

inline void RenderRectFilledRangeV(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float y_start_norm,
                                   float y_end_norm, float rounding)
{
    if (y_end_norm == y_start_norm)
        return;
    if (y_start_norm > y_end_norm)
        ImSwap(y_start_norm, y_end_norm);

    ImVec2 p0 = ImVec2(rect.Min.x, ImLerp(rect.Min.y, rect.Max.y, y_start_norm));
    ImVec2 p1 = ImVec2(rect.Max.x, ImLerp(rect.Min.y, rect.Max.y, y_end_norm));

    draw_list->AddRectFilled(p0, p1, col, rounding);
}

inline void displayBar(float current, float max, const ImVec2& size_arg, float rounding = 5.0f, float borderSize = 5.f,
                       ImColor mainColor = ImColor{255, 0, 0, 150}, ImColor bgColor = ImColor{0, 0, 0, 100},
                       ImColor borderColor = ImColor{0, 0, 0, 255}, const char* strFormat = "%.0f% / %.0f%")
{
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    // static float r = style.FrameBorderSize;
    // DragFloat("Test", &r, 0.1);

    ImVec2 pos  = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, {pos.x + size.x, pos.y + size.y});
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    float fraction = current / max;
    fraction       = ImSaturate(fraction);

    // Background

    const float halfBorderSize = borderSize * 0.5f;
    bb.Max.y += borderSize;
    bb.Min.y += borderSize;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, bgColor, rounding);
    window->DrawList->AddRect(bb.Min - ImVec2(halfBorderSize, halfBorderSize),
                              bb.Max + ImVec2(halfBorderSize, halfBorderSize), borderColor, rounding,
                              ImDrawCornerFlags_All, borderSize);

    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
    RenderRectFilledRangeH(window->DrawList, bb, mainColor, 0.0f, fraction, rounding);

    // Default displaying the fraction as percentage string, but user can override it
    char        overlay_buf[32];
    const char* overlay = nullptr;

    ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), strFormat, current, max);
    overlay = overlay_buf;

    ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f)
        RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x,
                                         bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x),
                                 bb.Min.y),
                          bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
}

} // namespace ImGui