// Name: ChatSpam
// Author: exec
// 
// Changes maximum number of messages send repeatedly, or disables spam
// protection entirely.

#include "chatspam.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		ChatSpam::ChatSpam()
		{
			enabled = Kitten.Conf.GetBool("ChatSpam", "Enabled", false);
			maxRepeats = Kitten.Conf.GetInt("ChatSpam", "MaxRepeats", 2);
		}

		void ChatSpam::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading ChatSpam...");

			std::optional<uintptr_t> address;

			auto patterns = {
				"C785 80020000 00000000  83BD 80020000 02  0F8D 23020000", // Alpha
				"C785 D4020000 00000000  83BD D4020000 02  0F8D 6B010000", // Beta1
			};

			for (const auto& pattern : patterns)
			{
				address = Kitten.Find("RagExe.exe", pattern);
				if (address)
					break;
			}

			if (!address)
			{
				Kitten.Log.Info("  patch failed, address not found.");
				return;
			}

			if (maxRepeats > 0)
			{
				std::string patchStr;
				patchStr += "C785 ???????? 00000000  83BD ????????";
				patchStr += HexTool::GetString((byte)min(127, maxRepeats));
				patchStr += "0F8D ????????";

				Kitten.Patch("RagExe.exe", address.value(), patchStr.c_str());
			}
			else
			{
				Kitten.Patch("RagExe.exe", address.value(), "C785 ???????? 00000000  83BD ???????? 02  9090 90909090");
			}

			Kitten.Log.Info("  patch applied at 0x%08X.", address.value());

			// Alpha 2001-08-30
			// 00480D99 | C785 80020000 00000000     | mov dword ptr ss:[ebp+280],0                         |
			// 00480DA3 | 83BD 80020000 02           | cmp dword ptr ss:[ebp+280],2                         | Compares number of repeats to 2
			// 00480DAA | 0F8D 23020000              | jge ragexe.480FD3                                    | Jumps to "nah-ah" if number of repeats is >= 2

			// Beta 2002-02-20
			// 004ABF7F | C785 D4020000 00000000     | mov dword ptr ss:[ebp+2D4],0                         |
			// 004ABF89 | 83BD D4020000 02           | cmp dword ptr ss:[ebp+2D4],2                         |
			// 004ABF90 | 0F8D 6B010000              | jge ragexe.4AC101                                    |
		}
	}
}
