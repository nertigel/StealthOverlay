#include <stdlib.h>

#include "styles.hpp"
#include "render.hpp"
#include <components/security/xorstr.hpp>

void render_module::start() {
    set_theme();

    ImGui::Begin(xorstr_("Stealth overlay"));
    ImGui::Text(xorstr_("This is your overlay."));

    if (ImGui::Button(xorstr_("Unload"))) {
        exit(1);
    }

    ImGui::End();
}

void render_module::set_theme() {
    set_imgui_theme();
}