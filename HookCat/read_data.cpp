// Name: ReadData
// Author: exec
//
// Changes data loading to favor files from the data folder over files
// from the GRFs.

#include <string>
#include "read_data.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		static void* (__thiscall* readDataOriginal)(void* pThis, const char* lpFileName, int arg_4);
		static HookInfo readGrfHook;

		static int(__thiscall* featureCheckOriginal)(void* pThis, const char* featureName);
		static HookInfo featureCheckHook;

		static int __fastcall FeatureCheckHooked(void* pThis, void* edx, const char* featureName)
		{
			if (featureName && strcmp(featureName, "readfolder") == 0)
			{
				Kitten.Log.Info("ReadData: Forced 'readfolder' feature to 1.");
				return 1;
			}
			return ((decltype(&FeatureCheckHooked))featureCheckHook.originalFunc)(pThis, edx, featureName);
		}

		static void* __fastcall ReadGrfHooked(void* pThis, void* edx, const char* lpFileName, int arg_4)
		{
			auto result = readDataOriginal(pThis, lpFileName, arg_4);
			if (result)
			{
				Kitten.Log.Info("ReadData: Loaded %s from folder.", lpFileName);
				return result;
			}

			return ((decltype(&ReadGrfHooked))readGrfHook.originalFunc)(pThis, edx, lpFileName, arg_4);
		}

		ReadData::ReadData()
		{
			enabled = Kitten.Conf.GetBool("ReadData", "Enabled", false);
		}

		void ReadData::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading ReadData...");

			// jRO Beta2 (2002-08-09)
			auto address1 = Kitten.Find("RagExe.exe", "E8 ????????  8945 FC  837D FC 00  75 13  8B4D 0C  51  8B55 08  52  8B4D F8  E8 ????????");
			if (address1) {
				uint32_t call1OffsetAddr = address1.value() + 1;
				uint32_t call1EndAddr = address1.value() + 5;

				uint32_t call2OffsetAddr = address1.value() + 26;
				uint32_t call2EndAddr = address1.value() + 30;

				int32_t origOffset1 = *(int32_t*)(call1OffsetAddr);
				int32_t origOffset2 = *(int32_t*)(call2OffsetAddr);

				uint32_t target1 = call1EndAddr + origOffset1;
				uint32_t target2 = call2EndAddr + origOffset2;

				Kitten.Log.Info("  found ReadGrf at 0x%08X", target1);
				Kitten.Log.Info("  found ReadData at 0x%08X", target2);

				readDataOriginal = (decltype(readDataOriginal))target2;
				readGrfHook = Kitten.HookAddr(target1, &ReadGrfHooked);

				if (readGrfHook) {
					Kitten.Log.Info("  applied ReadGrf hook.");
					return;
				}
			}

			// jRO EP3 (2003-05-27)
			auto address2 = Kitten.Find("RagExe.exe", "E8 ????????  8BF8  85FF  75 0E  8B4D 0C  51  53  8BCE  E8 ????????");
			if (address2) {
				uint32_t call1OffsetAddr = address2.value() + 1;
				uint32_t call1EndAddr = address2.value() + 5;

				uint32_t call2OffsetAddr = address2.value() + 19;
				uint32_t call2EndAddr = address2.value() + 23;

				int32_t origOffset1 = *(int32_t*)(call1OffsetAddr);
				int32_t origOffset2 = *(int32_t*)(call2OffsetAddr);

				uint32_t target1 = call1EndAddr + origOffset1;
				uint32_t target2 = call2EndAddr + origOffset2;

				Kitten.Log.Info("  found ReadGrf at 0x%08X", target1);
				Kitten.Log.Info("  found ReadData at 0x%08X", target2);

				readDataOriginal = (decltype(readDataOriginal))target2;
				readGrfHook = Kitten.HookAddr(target1, &ReadGrfHooked);

				if (readGrfHook) {
					Kitten.Log.Info("  applied ReadGrf hook.");
					return;
				}
			}

			// Clients supporting "readfolder"
			auto address3 = Kitten.FindString("RagExe.exe", "readfolder");
			if (address3)
			{
				// Started with hooking the feature check function, but it
				// appears as if the client loads some files before it
				// checks the feature, such as, critically, clientinfo.
				// This makes it necessary to actually overwrite the
				// global feature variable further down. I'll leave the
				// hook though, in case it might become useful in the
				// future.

				Kitten.Log.Info("  found 'readfolder' string at 0x%08X.", address3.value());

				std::string usagePattern = "68";
				usagePattern += HexTool::GetString((int)address3.value());
				usagePattern += "8BCB  E8 ????????";

				usagePattern += "85C0  74 07  C605 ???????? 01";

				auto usageAddr = Kitten.Find("RagExe.exe", usagePattern.c_str());
				if (usageAddr)
				{
					uint32_t callOffsetAddr = usageAddr.value() + 8;
					uint32_t callEndAddr = usageAddr.value() + 12;

					int32_t origOffset = *(int32_t*)(callOffsetAddr);

					uint32_t target = callEndAddr + origOffset;

					Kitten.Log.Info("  found feature function at 0x%08X.", target);

					featureCheckOriginal = (decltype(featureCheckOriginal))target;
					featureCheckHook = Kitten.HookAddr(target, &FeatureCheckHooked);

					if (featureCheckHook)
					{
						Kitten.Log.Info("  applied feature check hook.");
					}

					auto globalVarAddr = *(int*)(usageAddr.value() + 18);
					std::string movPattern = "A0";
					movPattern += HexTool::GetString(globalVarAddr);

					auto movAddresses = Kitten.FindAll("RagExe.exe", movPattern.c_str());
					if (movAddresses->size() > 0)
					{
						for (auto& value : *movAddresses)
						{
							Kitten.Log.Info("  mov found at 0x%08X.", value);
							Kitten.Patch("RagExe.exe", value, "B0 01 90 90 90");
						}

						Kitten.Log.Info("  applied readfolder overwrites.");
					}
				}
			}

			Kitten.Log.Info("  patch failed, no potential entry point found.");
		}

		// jRO Beta2 (2002-08-09)
		//   0041E34D | E8 7E050000                | call ragexe.41E8D0                                   | Read from GRF
		//   0041E352 | 8945 FC                    | mov dword ptr ss:[ebp-4],eax                         |
		//   0041E355 | 837D FC 00                 | cmp dword ptr ss:[ebp-4],0                           |
		//   0041E359 | 75 13                      | jne ragexe.41E36E                                    | If not found...
		//   0041E35B | 8B4D 0C                    | mov ecx,dword ptr ss:[ebp+C]                         |
		//   0041E35E | 51                         | push ecx                                             |
		//   0041E35F | 8B55 08                    | mov edx,dword ptr ss:[ebp+8]                         |
		//   0041E362 | 52                         | push edx                                             |
		//   0041E363 | 8B4D F8                    | mov ecx,dword ptr ss:[ebp-8]                         |
		//   0041E366 | E8 A50A0000                | call ragexe.41EE10                                   | Read from folder

		// jRO EP3 (2003-05-27)
		//   004D6B69 | E8 82060000                | call ragexe.4D71F0                                   | Read from GRF
		//   004D6B6E | 8BF8                       | mov edi,eax                                          |
		//   004D6B70 | 85FF                       | test edi,edi                                         |
		//   004D6B72 | 75 0E                      | jne ragexe.4D6B82                                    | If not found...
		//   004D6B74 | 8B4D 0C                    | mov ecx,dword ptr ss:[ebp+C]                         |
		//   004D6B77 | 51                         | push ecx                                             |
		//   004D6B78 | 53                         | push ebx                                             |
		//   004D6B79 | 8BCE                       | mov ecx,esi                                          |
		//   004D6B7B | E8 E0080000                | call ragexe.4D7460                                   | Read from folder

		// euRO EP10 (2007-03-05)
		//   004E3B72 | 68 18536A00                | push ragexe.6A5318                                   | Arg1: readfolder
		//   004E3B77 | 8BCB                       | mov ecx,ebx                                          |
		//   004E3B79 | E8 926E0200                | call ragexe.50AA10                                   | Call feature check
		//   004E3B7E | 85C0                       | test eax,eax                                         |
		//   004E3B80 | 74 07                      | je ragexe.4E3B89                                     | If feature is enabled...
		//   004E3B82 | C605 348A7300 01           | mov byte ptr ds:[738A34],1                           | ... activate it
	}
}
