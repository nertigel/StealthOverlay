// https://github.com/nertigel/StealthOverlay
#pragma once
#include <Windows.h>
#include "..\globals.hpp"

inline HHOOK hKeyboardHook;
inline LRESULT CALLBACK cKeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == 0 && g_Window.isOpen)
	{
		KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
		switch (wParam)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			int lp = 1 | (pKeyboard->scanCode << 16) | (1 << 24) | (0 << 29) | (0 << 30) | (0 << 31);
			PostMessage(g_Window.hWnd, (UINT)wParam, pKeyboard->vkCode, lp);
		}
		break;
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			int lp = 1 | (pKeyboard->scanCode << 16) | (1 << 24) | (0 << 29) | (1 << 30) | (1 << 31);
			PostMessage(g_Window.hWnd, (UINT)wParam, pKeyboard->vkCode, lp);
		}
		break;
		}
	}
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}