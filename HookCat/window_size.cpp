// Name: ChatSpam
// Author: exec
// 
// Changes window size.

#include "window_size.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		WindowSize::WindowSize()
		{
			enabled = Kitten.Conf.GetBool("WindowSize", "Enabled", false);
			width = Kitten.Conf.GetInt("WindowSize", "Width", 1024);
			height = Kitten.Conf.GetInt("WindowSize", "Height", 768);
		}

		void WindowSize::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading WindowSize...");

			auto address1 = Kitten.Find("RagExe.exe", "53 68 E0010000 68 80020000 6A 01 FFD6 2D E0010000 99 2BC2 D1F8 50 53 FFD6 2D 80020000");
			auto address2 = Kitten.Find("RagExe.exe", "8D843A E0010000 8B7C24 28 2BCF 50 8B4424 18 8D9431 80020000");

			if (!address1 || !address2)
			{
				Kitten.Log.Info("  patch failed address not found.");
				return;
			}

			auto widthHex = HexTool::GetString(width);
			auto heightHex = HexTool::GetString(height);

			// ==== Creation ====

			std::string patchStr1;
			patchStr1 += "53 68";
			patchStr1 += heightHex;
			patchStr1 += "68";
			patchStr1 += widthHex;
			patchStr1 += "6A 01 FFD6 2D";
			patchStr1 += heightHex;
			patchStr1 += "99 2BC2 D1F8 50 53 FFD6 2D";
			patchStr1 += widthHex;

			Kitten.Patch("RagExe.exe", address1.value(), patchStr1.c_str());

			// ==== Movement ====

			std::string patchStr2;
			patchStr2 += "8D843A";
			patchStr2 += heightHex;
			patchStr2 += "8B7C24 28 2BCF 50 8B4424 18 8D9431";
			patchStr2 += widthHex;

			Kitten.Patch("RagExe.exe", address2.value(), patchStr2.c_str());

			// ==================

			Kitten.Log.Info("  patches applied at 0x%08X and 0x%08X.", address1.value(), address2.value());

			//004A8DB8 | 53                         | push ebx                                             |
			//004A8DB9 | 68 E0010000                | push 1E0                                             | Height
			//004A8DBE | 68 80020000                | push 280                                             | Width
			//004A8DC3 | 6A 01                      | push 1                                               |
			//004A8DC5 | FFD6                       | call esi                                             |
			//004A8DC7 | 2D E0010000                | sub eax,1E0                                          | Height
			//004A8DCC | 99                         | cdq                                                  |
			//004A8DCD | 2BC2                       | sub eax,edx                                          |
			//004A8DCF | D1F8                       | sar eax,1                                            |
			//004A8DD1 | 50                         | push eax                                             |
			//004A8DD2 | 53                         | push ebx                                             |
			//004A8DD3 | FFD6                       | call esi                                             |
			//004A8DD5 | 2D 80020000                | sub eax,280                                          | Width

			//004A8E9E | 8D843A E0010000            | lea eax,dword ptr ds:[edx+edi+1E0]                   | Height
			//004A8EA5 | 8B7C24 28                  | mov edi,dword ptr ss:[esp+28]                        |
			//004A8EA9 | 2BCF                       | sub ecx,edi                                          |
			//004A8EAB | 50                         | push eax                                             |
			//004A8EAC | 8B4424 18                  | mov eax,dword ptr ss:[esp+18]                        |
			//004A8EB0 | 8D9431 80020000            | lea edx,dword ptr ds:[ecx+esi+280]                   | Width
		}
	}
}
