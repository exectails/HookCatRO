// Name: DDrawError
// Author: exec
// 
// Disables the error message after the client closed, telling us
// that DDraw wasn't cleaned up properly.

#include "ddraw_error.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		DDrawError::DDrawError()
		{
			enabled = Kitten.Conf.GetBool("DDrawError", "Enabled", false);
		}

		void DDrawError::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading DDrawError...");

			auto address1 = Kitten.Find("RagExe.exe", "8BD8  3BDF  7E 2B  53  8D5424");

			if (!address1)
			{
				Kitten.Log.Info("  patch failed address not found.");
				return;
			}

			Kitten.Patch("RagExe.exe", address1.value(), "8BD8  3BDF  EB 2B  53  8D5424");

			Kitten.Log.Info("  patch applied at 0x%08X.", address1.value());

			//00403725 | 8BD8                       | mov ebx,eax                                          |
			//00403727 | 3BDF                       | cmp ebx,edi                                          |
			//00403729 | 7E 2B                      | jle ragexe.403756                                    |
			//0040372B | 53                         | push ebx                                             |
			//0040372C | 8D5424 14                  | lea edx,dword ptr ss:[esp+14]                        |
			//00403730 | 68 5C624E00                | push ragexe.4E625C                                   | 4E625C:" : DDraw object is still referenced %d times"

			//00403729 | EB 2B                      | jmp ragexe.403756                                    |
		}
	}
}
