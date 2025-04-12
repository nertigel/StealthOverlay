// https://github.com/nertigel/StealthOverlay
#include <stdlib.h>
#include <Windows.h>

#include "styles.hpp"
#include "render.hpp"
#include <components/security/xorstr.hpp>
#include <components/globals.hpp>

bool streamproof = true;
static ImVec4 accent_color = ImVec4(1.00f, 0.85f, 0.10f, 1.00f);

bool test_checkbox = false;
bool test_checkbox2 = true;
int itest_slider = 1337;
float test_slider = 199.99;

void render_module::start() {
    set_theme(accent_color);

    ImGui::SetNextWindowSize(ImVec2(500, 300));

    ImGui::Begin(xorstr_("StealthOverlay"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::BeginTabBar(xorstr_("tab_bar"))) {
        if (ImGui::BeginTabItem(xorstr_("Tab 1"))) {
            ImGui::Text(xorstr_("Text label"));

            if (ImGui::Checkbox(xorstr_("Streamproof"), &streamproof)) {
                SetWindowDisplayAffinity(g_Window.hWnd, streamproof ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
            }

            ImGui::ColorEdit3(xorstr_("Accent color picker"), (float*)&accent_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

            ImGui::Checkbox(xorstr_("Checkbox"), &test_checkbox);
            ImGui::SameLine();
            ImGui::Checkbox(xorstr_("Checkbox 2"), &test_checkbox2);

            ImGui::SliderInt(xorstr_("iSlider"), &itest_slider, 1100, 10000);
            ImGui::SliderFloat(xorstr_("fSlider"), &test_slider, 0, 400);

            ImGui::Button(xorstr_("Button"));
            ImGui::SameLine();
            ImGui::Button(xorstr_("Button sized"), ImVec2(300, 0));

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(xorstr_("Tab 2"))) {
            ImGui::Text(xorstr_("This is the content of Tab 2."));
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(xorstr_("Unload"))) {
            exit(1);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void render_module::set_theme(ImVec4 accent_color) {
    set_imgui_theme(accent_color);
}