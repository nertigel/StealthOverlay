// https://github.com/nertigel/StealthOverlay
#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <tchar.h>

#include "components/imgui/imgui.h"
#include "components/imgui/imgui_impl_win32.h"
#include "components/imgui/imgui_impl_dx11.h"

// Link DirectX libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "components/globals.hpp"

class Overlay {
private:
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	void register_console_window();
	void setup_window();
	void begin_render();

	WNDCLASSEX window_class = {};

	ID3D11Device* g_pd3dDevice = nullptr;
	ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
};

inline Overlay overlay_class;