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

			auto address1 = Kitten.Find("RagExe.exe", "B9 68295200  C705 04615200 0A000000");

			if (!address1)
			{
				Kitten.Log.Info("  patch failed address not found.");
				return;
			}

			Kitten.Patch("RagExe.exe", address1.value(), "B9 68295200  C705 04615200 00000000");

			Kitten.Log.Info("  patch applied at 0x%08X.", address1.value());

			//004A9A29 | B9 68295200                | mov ecx,ragexe.522968                                |
			//004A9A2E | C705 04615200 0A000000     | mov dword ptr ds:[526104],A                          | A:'\n'
		}
	}
}
