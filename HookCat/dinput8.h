#pragma once

#include <Windows.h>

// Creates a DirectInput object and returns an IDirectInput8 Interface or
// later interface.
HRESULT WINAPI DirectInput8Create(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk);
