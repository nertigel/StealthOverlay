// https://github.com/nertigel/StealthOverlay
#pragma once
#include <components/imgui/imgui.h>

void set_imgui_theme(ImVec4 accent_color)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // base
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);  // Dark charcoal
    colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.14f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.23f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // frames
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);

    // accent
    colors[ImGuiCol_CheckMark] = accent_color;
    colors[ImGuiCol_SliderGrab] = accent_color;
    colors[ImGuiCol_SliderGrabActive] = accent_color;
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.35f);
    colors[ImGuiCol_ButtonActive] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.55f);

    // header / tabs
    colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.40f);
    colors[ImGuiCol_HeaderActive] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.65f);

    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.40f);
    colors[ImGuiCol_TabActive] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.60f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);

    // etc
    colors[ImGuiCol_Separator] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.95f);

    colors[ImGuiCol_TextSelectedBg] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.35f);
    colors[ImGuiCol_NavHighlight] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.80f);

    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f); // dark gray
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f); // slightly brighter when focused
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.11f, 0.75f); // when collapsed

    // settings
    style.WindowRounding = 1.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 2.0f;

    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(8, 6);
}