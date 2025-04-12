// https://github.com/nertigel/StealthOverlay
#include "overlay.hpp"

#include <components/ui_access/ui_access.hpp>
#include <components/imgui/imgui_internal.h>
#include <components/render/render.hpp>
#include <components/security/xorstr.hpp>
#include <dwmapi.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef HWND(WINAPI* CreateWindowInBand)(_In_ DWORD dwExStyle, _In_opt_ ATOM atom, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam, DWORD band);

void Overlay::register_console_window() {
    AllocConsole();
    FILE* dummy; // c++ autism
    freopen_s(&dummy, xorstr_("CONIN$"), xorstr_("r"), stdin);
    freopen_s(&dummy, xorstr_("CONOUT$"), xorstr_("w"), stderr);
    freopen_s(&dummy, xorstr_("CONOUT$"), xorstr_("w"), stdout);
}

// Initialize Direct3D
bool Overlay::CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };

    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 1,
        D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext
    );

    if (FAILED(res)) return false;

    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();

    return true;
}

// Cleanup
void Overlay::CleanupDeviceD3D() {
    if (g_mainRenderTargetView) g_mainRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();
}

// WinProc
LRESULT CALLBACK Overlay::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (overlay_class.g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            if (overlay_class.g_mainRenderTargetView) { overlay_class.g_mainRenderTargetView->Release(); overlay_class.g_mainRenderTargetView = NULL; }
            overlay_class.g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            ID3D11Texture2D* pBackBuffer;
            overlay_class.g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            overlay_class.g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &overlay_class.g_mainRenderTargetView);
            pBackBuffer->Release();
        }
        return 0;
    //case WM_SYSCOMMAND:
    //    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
    //        return 0;
    //    break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Overlay::setup_window()
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    HMODULE user32_lla = LoadLibraryA(xorstr_("user32.dll"));
    CreateWindowInBand pCreateWindowInBand = reinterpret_cast<CreateWindowInBand>(GetProcAddress(user32_lla, xorstr_("CreateWindowInBand")));
    PrepareForUIAccess();

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    window_class = { sizeof(window_class), CS_CLASSDC, Overlay::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, xorstr_(L" "), NULL };
    ATOM window_class_ex = RegisterClassExW(&window_class);

    g_Window.hWnd = pCreateWindowInBand(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, window_class_ex, xorstr_(L" "), WS_POPUP, 0, 0, screen_width, screen_height, NULL, NULL, window_class.hInstance, LPVOID(window_class_ex), ZBID_UIACCESS);

    // Allow click-through and transparency
    SetLayeredWindowAttributes(g_Window.hWnd, 0, 0, LWA_COLORKEY);

    MARGINS Margin = { 0, 0, screen_width, screen_height };
    DwmExtendFrameIntoClientArea(g_Window.hWnd, &Margin);

    if (!CreateDeviceD3D(g_Window.hWnd)) {
        CleanupDeviceD3D();
        UnregisterClassW(window_class.lpszClassName, window_class.hInstance);
        return;
    }

    //hide from screen capture
    SetWindowDisplayAffinity(g_Window.hWnd, WDA_EXCLUDEFROMCAPTURE);

    ShowWindow(g_Window.hWnd, SW_SHOWDEFAULT);
    UpdateWindow(g_Window.hWnd);

    //SetWindowLong(g_Window.hWnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);

}

void Overlay::begin_render() {
    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();

    ImGui_ImplWin32_Init(g_Window.hWnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        if (GetAsyncKeyState(VK_F4) & 1) {
            if (g_Window.isOpen) {
                SetWindowLong(g_Window.hWnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
            }
            else {
                SetWindowLong(g_Window.hWnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
            }
            ShowWindow(g_Window.hWnd, g_Window.isOpen ? SW_HIDE : SW_SHOWDEFAULT);
            UpdateWindow(g_Window.hWnd);
            g_Window.isOpen = !g_Window.isOpen;
        }

        static float background_alpha = 0.f;
        background_alpha = ImLerp(background_alpha, g_Window.isOpen ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 5);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, background_alpha);

        if (g_Window.isOpen) {
            render_module::start();
        }

        ImGui::PopStyleVar();

        ImGui::Render();

        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    DestroyWindow(g_Window.hWnd);
    UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}