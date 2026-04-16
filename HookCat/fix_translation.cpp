// Name: FixTranslation
// Author: exec
//
// Fixes some missing or incorrect translations, such as hardcoded,
// untranslated string.

#include "fix_translation.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		FixTranslation::FixTranslation()
		{
			enabled = Kitten.Conf.GetBool("FixTranslation", "Enabled", false);
		}

		static void ReplaceStringPush(const char* hexPattern, const char* oldString, const char* newString)
		{
			auto address = Kitten.Find("RagExe.exe", hexPattern);

			if (!address)
			{
				Kitten.Log.Info("  patch for '%s' failed, address not found.", oldString);
				return;
			}

			std::string patchStr;
			patchStr += "68";
			patchStr += HexTool::GetString((int)newString);

			Kitten.Patch("RagExe.exe", address.value(), patchStr.c_str());

			Kitten.Log.Info("  patch applied at 0x%08X, '%s' -> '%s'.", address.value(), oldString, newString);
		}

		void FixTranslation::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading FixTranslation...");

			ReplaceStringPush("68 C0944E00", "교환창", "Trade");
		}

		// Alpha
		// 004490A1 | 68 C0944E00                | push ragexe.4E94C0                                   | Push "교환창" (Exchange Window)
	}
}
