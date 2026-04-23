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

			auto address1 = Kitten.Find("RagExe.exe", "FF15 1C224D00  85C0  74 09");
			if (address1)
			{
				Kitten.Patch("RagExe.exe", address1.value(), "FF15 1C224D00  85C0  EB 09");
				Kitten.Log.Info("  window patch applied at 0x%08X.", address1.value());
				return;
			}

			auto address2 = Kitten.Find("RagExe.exe", "FF15 ????????  6A 00  50  FF15 ????????  85C0  0F85 ????????");
			auto address3 = Kitten.Find("RagExe.exe", "68 ????????  68 ????????  8935 ????????  FF15 ????????  85C0  74 09");

			if (address2 && address3)
			{
				Kitten.Patch("RagExe.exe", address3.value(), "68 ????????  68 ????????  8935 ????????  FF15 ????????  85C0  EB 09");
				Kitten.Patch("RagExe.exe", address2.value(), "FF15 ????????  6A 00  50  FF15 ????????  85C0  9090 90909090");

				Kitten.Log.Info("  window patch applied at 0x%08X.", address3.value());
				Kitten.Log.Info("  mutex patch applied at 0x%08X.", address2.value());
				return;
			}

			Kitten.Log.Info("  patch failed, address not found.");
			return;


			// Alpha 2001-08-30
			//   FindWindow
			//     004A8D4A | FF15 1C224D00              | call dword ptr ds:[<&FindWindowA>]                   | Find window
			//     004A8D50 | 85C0                       | test eax,eax                                         |
			//     004A8D52 | 74 09                      | je ragexe.4A8D5D                                     | Jump only if window wasn't found

			// Beta1 2002-02-20
			//   FindWindow
			//     004FD1EA | 68 EC4B5500                | push ragexe.554BEC                                   | 554BEC:"Ragnarok"
			//     004FD1EF | 68 EC4B5500                | push ragexe.554BEC                                   | 554BEC:"Ragnarok"
			//     004FD1F4 | 8935 10C45900              | mov dword ptr ds:[59C410],esi                        |
			//     004FD1FA | FF15 54A25200              | call dword ptr ds:[<&FindWindowA>]                   | Find window
			//     004FD200 | 85C0                       | test eax,eax                                         |
			//     004FD202 | 74 09                      | je ragexe.4FD20D                                     | Jump only if window wasn't found
			//   Mutex
			//     004FDAC0 | FF15 88A05200              | call dword ptr ds:[<&CreateMutexA>]                  | Create mutex
			//     004FDAC6 | 6A 00                      | push 0                                               |
			//     004FDAC8 | 50                         | push eax                                             |
			//     004FDAC9 | FF15 8CA05200              | call dword ptr ds:[<&WaitForSingleObject>]           | Wait for mutex
			//     004FDACF | 85C0                       | test eax,eax                                         |
			//     004FDAD1 | 0F85 1B040000              | jne ragexe.4FDEF2                                    | Jump if mutex exists, nop to always allow
		}
	}
}
