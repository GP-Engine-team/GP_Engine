/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h> //ImGuiItemFlags_Disabled, PushItemFlag
#include <string>
// Thank's to : https://github.com/ocornut/imgui/issues/211#issuecomment-812293268
namespace ImGui
{
inline void PushEnabled(bool _enabled)
{
    PushItemFlag(ImGuiItemFlags_Disabled, !_enabled);
    PushStyleVar(ImGuiStyleVar_Alpha, GetStyle().Alpha * (_enabled ? 1.0f : 0.5f));
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
    SetCursorPosX(GetStyle().FramePadding.x + GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x +
                  (GetWindowSize().x - GetCurrentWindow()->Viewport->CurrWorkOffsetMin.x) * xWindowSizeRatio -
                  size.x * (int)hItemAlign * 0.01f);

    SetCursorPosY(GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y +
                  (GetWindowSize().y - GetCurrentWindow()->Viewport->CurrWorkOffsetMin.y) * yWindowSizeRatio -
                  size.y * (int)vItemAlign * 0.01f);
}

inline void SetNextTextLayout(const char* text, float xWindowSizeRatio, float yWindowSizeRatio,
                              EHAlign hItemAlign = EHAlign::Middle, EVAlign vItemAlign = EVAlign::Middle)
{
    const ImVec2 sizeText = CalcTextSize(text);
    SetCursorPosX(GetWindowSize().x * xWindowSizeRatio - sizeText.x * (int)hItemAlign * 0.01f);
    SetCursorPosY(GetWindowSize().y * yWindowSizeRatio - sizeText.y * (int)vItemAlign * 0.01f);
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

// https://github.com/ocornut/imgui/issues/1096#issuecomment-293544142
// Definition (.cpp file. Not sure if it needs "imgui_internal.h" or not)
inline IMGUI_API bool ImageButtonWithTextRight(ImTextureID texId, const char* label,
                                               const ImVec2& imageSize = ImVec2(0, 0), const ImVec2& uv0 = ImVec2(0, 0),
                                               const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1,
                                               const ImVec4& bg_col   = ImVec4(0, 0, 0, 0),
                                               const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 size = imageSize;
    if (size.x <= 0 && size.y <= 0)
    {
        size.x = size.y = GetTextLineHeightWithSpacing();
    }
    else
    {
        if (size.x <= 0)
            size.x = size.y;
        else if (size.y <= 0)
            size.y = size.x;
        size *= window->FontWindowScale * GetIO().FontGlobalScale;
    }

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id       = window->GetID(label);
    const ImVec2  textSize = CalcTextSize(label, NULL, true);
    const bool    hasText  = textSize.x > 0;

    const float innerSpacing =
        hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
    const ImVec2 padding =
        (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
    ImVec2       start(0, 0);
    start = window->DC.CursorPos + padding;
    if (size.y < textSize.y)
        start.y += (textSize.y - size.y) * .5f;
    const ImRect image_bb(start, start + size);
    start = window->DC.CursorPos + padding;
    start.x += size.x + innerSpacing;
    if (size.y > textSize.y)
        start.y += (size.y - textSize.y) * .5f;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered = false, held = false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive
                                  : hovered         ? ImGuiCol_ButtonHovered
                                                    : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (textSize.x > 0)
        RenderText(start, label);
    return pressed;
}

inline IMGUI_API void ImageWithTextRight(ImTextureID texId, const char* label, const ImVec2& imageSize = ImVec2(0, 0),
                                         const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1),
                                         int frame_padding = -1, const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImVec2 size = imageSize;
    if (size.x <= 0 && size.y <= 0)
    {
        size.x = size.y = GetTextLineHeightWithSpacing();
    }
    else
    {
        if (size.x <= 0)
            size.x = size.y;
        else if (size.y <= 0)
            size.y = size.x;
        size *= window->FontWindowScale * GetIO().FontGlobalScale;
    }

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id       = window->GetID(label);
    const ImVec2  textSize = CalcTextSize(label, NULL, true);
    const bool    hasText  = textSize.x > 0;

    const float innerSpacing =
        hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
    const ImVec2 padding =
        (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
    ImVec2       start(0, 0);
    start = window->DC.CursorPos + padding;
    if (size.y < textSize.y)
        start.y += (textSize.y - size.y) * .5f;
    const ImRect image_bb(start, start + size);
    start = window->DC.CursorPos + padding;
    start.x += size.x + innerSpacing;
    if (size.y > textSize.y)
        start.y += (size.y - textSize.y) * .5f;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return;

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (textSize.x > 0)
        RenderText(start, label);
}

inline IMGUI_API bool imageButtonWithTextCenter(ImTextureID texId, const char* label,
                                                const ImVec2& imageSize = ImVec2(0, 0),
                                                const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1),
                                                int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                                                const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 size = imageSize;
    if (size.x <= 0 && size.y <= 0)
    {
        size.x = size.y = GetTextLineHeightWithSpacing();
    }
    else
    {
        if (size.x <= 0)
            size.x = size.y;
        else if (size.y <= 0)
            size.y = size.x;
        // size *= window->FontWindowScale * GetIO().FontGlobalScale;
    }

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id       = window->GetID(label);
    const ImVec2  textSize = CalcTextSize(label, NULL, true);
    const bool    hasText  = textSize.x > 0;

    const float innerSpacing =
        hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
    const ImVec2 padding =
        (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing, size.y + innerSpacing);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding);
    ImVec2       start(0, 0);
    start = window->DC.CursorPos + padding;
    if (size.y < textSize.y)
        start.y += (textSize.y - size.y) * .5f;
    const ImRect image_bb(start, start + size);
    start = window->DC.CursorPos + padding;
    start.x += -textSize.x / 2.f + size.x / 2.f + innerSpacing;
    if (size.y > textSize.y)
        start.y += (size.y - textSize.y) * .5f;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered = false, held = false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive
                                  : hovered         ? ImGuiCol_ButtonHovered
                                                    : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (textSize.x > 0)
        RenderText(start, label);
    return pressed;
}

} // namespace ImGui