#pragma once

#include <Windows.h>
#include <vector>

// Creates a DirectInput8 interface and returns a pointer to it.
HRESULT WINAPI DirectInput8Create(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk);

// Creates a DirectInput interface and returns a pointer to it.
HRESULT WINAPI DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppDI, LPUNKNOWN punkOuter);

extern HWND appWindowHandle;

// list of wndproc hook functions to call when a message is received
extern std::vector<WNDPROC> wndProcHooks;
