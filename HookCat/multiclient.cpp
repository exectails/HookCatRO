// Name: MultiClient
// Author: exec
// 
// Disables a jump in RagExe.exe where it checks whether the game window
// already exists.

#include "multiclient.h"
#include "hookcat.h"

namespace HookCat
{
	namespace Mods
	{
		MultiClient::MultiClient()
		{
			enabled = Kitten.Conf.GetBool("MultiClient", "Enabled", false);
		}

		void MultiClient::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading MultiClient...");

			auto address = Kitten.Find("RagExe.exe", "FF15 1C224D00  85C0  74 09");
			if (!address)
			{
				Kitten.Log.Info("  patch failed, address not found.");
				return;
			}

			Kitten.Patch("RagExe.exe", address.value(), "FF15 1C224D00  85C0  EB 09");

			Kitten.Log.Info("  patch applied at 0x%08X.", address.value());

			//004A8D4A | FF15 1C224D00              | call dword ptr ds:[<&FindWindowA>]                   | Find window
			//004A8D50 | 85C0                       | test eax,eax                                         |
			//004A8D52 | 74 09                      | je ragexe.4A8D5D                                     | Jump only if window wasn't found
		}
	}
}
