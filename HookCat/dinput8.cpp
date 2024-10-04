#include "dinput8.h"

// The dinput8 export enables us to easily "inject" our DLL into a game,
// as it will load the dinput8.dll in its local directory if it's present.
// Our DLL must then provide all functions that would normally be called
// in the original though, like DirectInput8Create.

typedef HRESULT(WINAPI* DirectInput8CreateFunc)(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk);

DirectInput8CreateFunc originalDirectInput8Create = nullptr;

HRESULT WINAPI DirectInput8Create(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk)
{
	if (!originalDirectInput8Create)
	{
		// Get reference to the original DirectInput8Create function in
		// the dinput8.dll in the system directory.
		CHAR syspath[MAX_PATH];
		GetSystemDirectory(syspath, MAX_PATH);
		strcat_s(syspath, "\\dinput8.dll");
		HMODULE hMod = LoadLibrary(syspath);

		originalDirectInput8Create = (DirectInput8CreateFunc)GetProcAddress(hMod, "DirectInput8Create");
	}

	// Call original DirectInput8Create
	return originalDirectInput8Create(inst_handle, version, r_iid, out_wrapper, p_unk);
}
