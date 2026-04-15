// Name: MultiClient
// Author: exec
// 
// Disables a jump in RagExe.exe where it checks whether the game window
// already exists.

#include "better_mouse_capture.h"
#include "hookcat.h"
#include "dinput8.h"

namespace HookCat
{
	namespace Mods
	{
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		BetterMouseCapture::BetterMouseCapture()
		{
			enabled = Kitten.Conf.GetBool("BetterMouseCapture", "Enabled", false);
		}

		void BetterMouseCapture::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading BetterMouseCapture...");

			wndProcHooks.push_back(WndProc);

			Kitten.Log.Info("  added hook to window procedure.");
		}

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

			return 0;
		}
	}
}
