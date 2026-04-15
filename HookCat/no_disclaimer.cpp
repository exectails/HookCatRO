// Name: NoDisclaimer
// Author: exec
// 
// Disables disclaimer on game start.

#include "no_disclaimer.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		NoDisclaimer::NoDisclaimer()
		{
			enabled = Kitten.Conf.GetBool("NoDisclaimer", "Enabled", false);
		}

		void NoDisclaimer::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading NoDisclaimer...");

			auto addresses = Kitten.FindAll("RagExe.exe", "B9 ????????  C705 ???????? 0A000000");
			auto addressCount = addresses->size();

			if (addressCount == 0)
			{
				Kitten.Log.Info("  patch failed, address not found.");
				return;
			}

			if (addressCount != 1)
			{
				Kitten.Log.Info("  patch failed, expected 1 address, found %d.", addressCount);
				return;
			}

			auto address = addresses->at(0);

			Kitten.Patch("RagExe.exe", address, "B9 ????????  C705 ???????? 00000000");

			Kitten.Log.Info("  patch applied at 0x%08X.", address);

			// Alpha 2001-08-30
			//004A9A29 | B9 68295200                | mov ecx,ragexe.522968                                |
			//004A9A2E | C705 04615200 0A000000     | mov dword ptr ds:[526104],A                          | A:'\n'

			// Beta1 2002-02-20
			//004FDD85 | B9 A8CD5800                | mov ecx,ragexe.58CDA8                                |
			//004FDD8A | C705 ECA35900 0A000000     | mov dword ptr ds:[59A3EC],A                          | A:'\n'
		}
	}
}
