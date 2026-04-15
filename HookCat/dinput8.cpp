#include "dinput8.h"
#include "MinHook.h"
#include "hookcat.h"

// The dinput8 export enables us to easily "inject" our DLL into a game,
// as it will load the dinput8.dll in its local directory if it's present.
// Our DLL must then provide all functions that would normally be called
// in the original though, like DirectInput8Create.

HWND appWindowHandle = NULL;

typedef HRESULT(WINAPI* DirectInput8CreateFunc)(HINSTANCE hinst, DWORD dwVersion, const IID& riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
typedef HRESULT(WINAPI* DirectInputCreateAFunc)(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppDI, LPUNKNOWN punkOuter);

typedef HRESULT(WINAPI* CreateDeviceFunc)(void* self, REFGUID rguid, LPVOID* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
typedef HRESULT(WINAPI* SetCooperativeLevelFunc)(void* self, HWND hwnd, DWORD dwFlags);

DirectInput8CreateFunc originalDirectInput8Create = nullptr;
DirectInputCreateAFunc originalDirectInputCreateA = nullptr;

CreateDeviceFunc originalCreateDevice = nullptr;
SetCooperativeLevelFunc originalSetCooperativeLevel = nullptr;

WNDPROC originalWndProc = nullptr;

static HRESULT WINAPI HookedCreateDevice(void* self, REFGUID rguid, LPVOID* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
static HRESULT WINAPI HookedSetCooperativeLevel(void* self, HWND hwnd, DWORD dwFlags);
static LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, const IID& riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	if (!originalDirectInput8Create)
	{
		// Get reference to the original DirectInput8Create function in
		// dinput8.dll in the system directory
		CHAR syspath[MAX_PATH];
		GetSystemDirectory(syspath, MAX_PATH);
		strcat_s(syspath, "\\dinput8.dll");
		HMODULE hMod = LoadLibrary(syspath);

		if (!hMod)
			return E_FAIL;

		originalDirectInput8Create = (DirectInput8CreateFunc)GetProcAddress(hMod, "DirectInput8Create");
	}

	// Call original DirectInput8Create and hook CreateDevice
	HRESULT hr = originalDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);

	if (SUCCEEDED(hr) && ppvOut && *ppvOut)
	{
		if (!originalCreateDevice)
		{
			void** vtbl = *(void***)(*ppvOut);
			void* pCreateDevice = vtbl[3]; // IDirectInput8::CreateDevice index

			if (MH_CreateHook(pCreateDevice, &HookedCreateDevice, (LPVOID*)&originalCreateDevice) == MH_OK)
				MH_EnableHook(pCreateDevice);
		}
	}

	return hr;
}

HRESULT WINAPI DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppDI, LPUNKNOWN punkOuter)
{
	if (!originalDirectInputCreateA)
	{
		// Get reference to the original DirectInputCreateA function in
		// dinput.dll in the system directory
		CHAR syspath[MAX_PATH];
		GetSystemDirectory(syspath, MAX_PATH);
		strcat_s(syspath, "\\dinput.dll");
		HMODULE hMod = LoadLibrary(syspath);

		if (!hMod)
			return E_FAIL;

		originalDirectInputCreateA = (DirectInputCreateAFunc)GetProcAddress(hMod, "DirectInputCreateA");
	}

	// Call original DirectInputCreateA and hook CreateDevice
	HRESULT hr = originalDirectInputCreateA(hinst, dwVersion, ppDI, punkOuter);

	if (SUCCEEDED(hr) && ppDI && *ppDI)
	{
		if (!originalCreateDevice)
		{
			void** vtbl = *(void***)(*ppDI);
			void* pCreateDevice = vtbl[3]; // IDirectInputA::CreateDevice index

			if (MH_CreateHook(pCreateDevice, &HookedCreateDevice, (LPVOID*)&originalCreateDevice) == MH_OK)
				MH_EnableHook(pCreateDevice);
		}
	}

	return hr;
}

static HRESULT WINAPI HookedCreateDevice(void* self, REFGUID rguid, LPVOID* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	HRESULT hr = originalCreateDevice(self, rguid, lplpDirectInputDevice, pUnkOuter);

	if (SUCCEEDED(hr) && lplpDirectInputDevice && *lplpDirectInputDevice)
	{
		if (!originalSetCooperativeLevel)
		{
			// IDirectInputDevice8::SetCooperativeLevel
			void** vtbl = *(void***)(*lplpDirectInputDevice);
			void* pSetCooperativeLevel = vtbl[13];

			if (MH_CreateHook(pSetCooperativeLevel, &HookedSetCooperativeLevel, (LPVOID*)&originalSetCooperativeLevel) == MH_OK)
				MH_EnableHook(pSetCooperativeLevel);
		}
	}

	return hr;
}

static HRESULT WINAPI HookedSetCooperativeLevel(void* self, HWND hwnd, DWORD dwFlags)
{
	appWindowHandle = hwnd;

	if (Kitten.Conf.GetBool("BetterMouseCapture", "Enabled", false))
	{
		if (appWindowHandle && !originalWndProc)
			originalWndProc = (WNDPROC)SetWindowLongPtr(appWindowHandle, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
	}

	return originalSetCooperativeLevel(self, hwnd, dwFlags);
}

static LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				ClipCursor(NULL);
				ShowCursor(TRUE);
			}
			else
			{
				RECT rect;
				GetClientRect(hWnd, &rect);

				POINT tl = { rect.left, rect.top };
				POINT br = { rect.right, rect.bottom };

				ClientToScreen(hWnd, &tl);
				ClientToScreen(hWnd, &br);

				rect.left = tl.x;
				rect.top = tl.y;
				rect.right = br.x;
				rect.bottom = br.y;

				ClipCursor(&rect);
				ShowCursor(FALSE);
			}
			break;
		}
	}

	return CallWindowProc(originalWndProc, hWnd, msg, wParam, lParam);
}
