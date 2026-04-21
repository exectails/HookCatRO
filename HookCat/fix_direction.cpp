// Name: FixDirection
// Author: exec
//
// Fixes direction calculation issues where the direction sent to the
// server is different from the direction shown on the client.

#include "fix_direction.h"
#include "hookcat.h"
#include "hextool.h"
#include "patcher.h"

namespace HookCat
{
	namespace Mods
	{
		static uintptr_t returnAddr = 0;

		static int CalcDirection(float angle)
		{
			return (int)((angle + 180.0f + 22.5f) / 45.0f) & 7;
		}

		__declspec(naked) static void HookAddress()
		{
			__asm {
				mov eax, dword ptr[esi + 0x138]
				push eax

				call CalcDirection
				add esp, 4

				mov edx, eax

				jmp dword ptr[returnAddr]
			}
		}

		FixDirection::FixDirection()
		{
			enabled = Kitten.Conf.GetBool("FixDirection", "Enabled", false);
		}

		void FixDirection::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading FixDirection...");

			auto injectAddress = Kitten.Find("RagExe.exe", "B8 B7600BB6  81C1 B4000000  F7E9  03D1  C1FA 05  8BCA  C1E9 1F  03D1  81E2 07000080  79 05  4A  83CA F8  42");
			if (!injectAddress)
			{
				Kitten.Log.Info("  patch failed, address not found.");
				return;
			}

			auto info = Kitten.Detour(injectAddress.value(), (uintptr_t)HookAddress, 38);
			if (!info)
			{
				Kitten.Log.Info("  patch failed, detour failed.");
				return;
			}

			returnAddr = info.returnAddr;

			Kitten.Log.Info("  patched successfully.");
		}

		// Beta1
		// 004C8423 | E8 C8D6FCFF                | call <ragexe.sub_495AF0>                             |
		// 004C8428 | D986 38010000              | fld st(0),dword ptr ds:[esi+138]                     |
		// 004C842E | 66:C74424 30 3700          | mov word ptr ss:[esp+30],37                          | Write op 0x37 (CZ_CHANGE_DIRECTION) to buffer
		// 004C8435 | E8 1A5F0400                | call <ragexe.sub_50E354>                             |
		// 004C843A | 8BC8                       | mov ecx,eax                                          |
		// 004C843C | B8 B7600BB6                | mov eax,B60B60B7                                     |
		// 004C8441 | 81C1 B4000000              | add ecx,B4                                           |
		// 004C8447 | F7E9                       | imul ecx                                             |
		// 004C8449 | 03D1                       | add edx,ecx                                          |
		// 004C844B | C1FA 05                    | sar edx,5                                            |
		// 004C844E | 8BCA                       | mov ecx,edx                                          |
		// 004C8450 | C1E9 1F                    | shr ecx,1F                                           |
		// 004C8453 | 03D1                       | add edx,ecx                                          |
		// 004C8455 | 81E2 07000080              | and edx,80000007                                     |
		// 004C845B | 79 05                      | jns ragexe.4C8462                                    |
		// 004C845D | 4A                         | dec edx                                              |
		// 004C845E | 83CA F8                    | or edx,FFFFFFF8                                      |
		// 004C8461 | 42                         | inc edx                                              |
		// 004C8462 | 885424 32                  | mov byte ptr ss:[esp+32],dl                          | Write direction to buffer
		// 004C8466 | 8D5424 30                  | lea edx,dword ptr ss:[esp+30]                        |
		// 004C846A | 52                         | push edx                                             |
		// 004C846B | 6A 37                      | push 37                                              | Pass op 0x37 (CZ_CHANGE_DIRECTION)
		// 004C846D | B9 E8B95500                | mov ecx,ragexe.55B9E8                                |
		// 004C8472 | E8 F922F5FF                | call <ragexe.sub_41A770>                             | Send buffer
	}
}
