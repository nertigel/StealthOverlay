// https://github.com/nertigel/StealthOverlay
// Licensed under CC BY-NC-ND 4.0: https://creativecommons.org/licenses/by-nc-nd/4.0/
// © 2025 Nertigel. All rights reserved.

#include <iostream>

#include "components/overlay/overlay.hpp"
#include "components/hooks/keyboard.hpp"
#include <components/security/xorstr.hpp>

// application entry
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    // console window
    overlay_class.register_console_window();

    // hook
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, cKeyboardHook, NULL, 0);
    std::cout << xorstr_("hKeyboardHook hooked") << std::endl;

    // render
    overlay_class.setup_window();
    overlay_class.begin_render();

    // unhook
    UnhookWindowsHookEx(hKeyboardHook);

    return 0;
}