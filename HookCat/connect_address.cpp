// Name: ChatSpam
// Author: exec
// 
// Changes maximum number of messages send repeatedly, or disables spam
// protection entirely.

#include "connect_address.h"
#include "hookcat.h"
#include "hextool.h"

namespace HookCat
{
	namespace Mods
	{
		ConnectAddress::ConnectAddress()
		{
			enabled = Kitten.Conf.GetBool("ConnectAddress", "Enabled", false);
			enableArgs = Kitten.Conf.GetBool("ConnectAddress", "EnableArguments", true);
			ip = Kitten.Conf.GetString("ConnectAddress", "IP", "127.0.0.1");
			port = Kitten.Conf.GetInt("ConnectAddress", "Port", 7000);

			memset(ip2, 0, 15);
			memcpy(ip2, ip.c_str(), ip.length());
		}

		void ConnectAddress::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading ConnectAddress...");

			std::optional<uintptr_t> ipAddress, portAddress;

			// Alpha
			auto blockAddresses = Kitten.FindAll("RagExe.exe", "C705 ???????? 581B0000  E8 ????????  BF ????????  83C9 FF");

			if (blockAddresses->size() > 1)
			{
				Kitten.Log.Info("  patch failed, expected 1 address, found %d.", blockAddresses->size());
				return;
			}

			if (blockAddresses && blockAddresses->size() == 1)
			{
				auto blockAddress = blockAddresses->at(0);
				ipAddress = blockAddress + 15;
				portAddress = blockAddress + 0;
			}
			else
			{
				// Beta1
				auto blockAddresses = Kitten.FindAll("RagExe.exe", "BF ????????  83C9 FF  33C0  C705 ???????? F41A0000");

				if (blockAddresses->size() > 1)
				{
					Kitten.Log.Info("  patch failed, expected 1 address, found %d.", blockAddresses->size());
					return;
				}

				if (blockAddresses && blockAddresses->size() == 1)
				{
					auto blockAddress = blockAddresses->at(0);
					ipAddress = blockAddress;
					portAddress = blockAddress + 10;
				}
			}

			if (!ipAddress || !portAddress)
			{
				Kitten.Log.Info("  patch failed, address not found.");
				return;
			}

			// The Beta1 2001-12-14 client appears to be an English
			// client, except that it uses the Japanese region setting,
			// which changes the IP used. By forcing the English region,
			// for the IP, we get past this.
			auto beta1RegionAddress = Kitten.Find("RagExe.exe", "E8 36CBF6FF  A1 50055300  83E8 00  74 65  48");
			if (beta1RegionAddress)
			{
				Kitten.Patch("RagExe.exe", beta1RegionAddress.value(), "E8 36CBF6FF  B8 01000000  83E8 00  74 65  48");
				Kitten.Log.Info("  region patch applied at 0x%08X.", beta1RegionAddress.value());
			}

			if (enableArgs)
			{
				std::string cmdLine(GetCommandLineA());

				auto index = cmdLine.find("ip:", 0);
				if (index != std::string::npos)
				{
					auto endIndex = cmdLine.find(" ", index);
					if (endIndex == -1)
						endIndex = cmdLine.length();

					if (endIndex != std::string::npos)
					{
						std::string newIp = cmdLine.substr(index + 3, endIndex - index - 3);

						memset(ip2, 0, 15);
						memcpy(ip2, newIp.c_str(), newIp.length());

						Kitten.Log.Info("  overwriting conf IP with argument %s.", newIp.c_str());
					}
				}

				index = cmdLine.find("port:", 0);
				if (index != std::string::npos)
				{
					auto endIndex = cmdLine.find(" ", index);
					if (endIndex == -1)
						endIndex = cmdLine.length();

					if (endIndex != std::string::npos)
					{
						std::string newPort = cmdLine.substr(index + 5, endIndex - index - 5);
						port = atoi(newPort.c_str());
						Kitten.Log.Info("  overwriting conf port with argument %d.", port);
					}
				}
			}

			// ==== IP ====

			const char* ipStrPtr = ip2;

			std::string ipPatchStr;
			ipPatchStr += "BF";
			ipPatchStr += HexTool::GetString((int)ipStrPtr);

			Kitten.Patch("RagExe.exe", ipAddress.value(), ipPatchStr.c_str());

			// ==== Port ====

			std::string portPatchStr;
			portPatchStr += "C705 ????????";
			portPatchStr += HexTool::GetString(port);

			Kitten.Patch("RagExe.exe", portAddress.value(), portPatchStr.c_str());

			// ==============

			Kitten.Log.Info("  patches applied at 0x%08X and 0x%08X.", ipAddress.value(), portAddress.value());
			Kitten.Log.Info("  new address: %s:%d", ip2, port);

			// Alpha 2001-08-30
			// 0048BD56 | B9 40645200                | mov ecx,ragexe.526440                                |
			// 0048BD5B | C705 70605200 581B0000     | mov dword ptr ds:[526070],1B58                       | Port
			// 0048BD65 | E8 762C0100                | call <ragexe.sub_49E9E0>                             |
			// 0048BD6A | BF F8BA4E00                | mov edi,ragexe.4EBAF8                                | IP
			// 0048BD6F | 83C9 FF                    | or ecx,FFFFFFFF                                      |

			// Beta1 2001-12-14
			// 004AEEE5 | E8 36CBF6FF                | call ragexe.41BA20                                   |
			// 004AEEEA | A1 50055300                | mov eax,dword ptr ds:[530550]                        | Get Region
			// 004AEEEF | 83E8 00                    | sub eax,0                                            |
			// 004AEEF2 | 74 65                      | je ragexe.4AEF59                                     |
			// 004AEEF4 | 48                         | dec eax                                              |
			// 004AEEF5 | 74 36                      | je ragexe.4AEF2D                                     |

			// Beta1 2002-02-20
			// 004BB429 | BF DCA75400                | mov edi,ragexe.54A7DC                                | IP
			// 004BB42E | 83C9 FF                    | or ecx,FFFFFFFF                                      |
			// 004BB431 | 33C0                       | xor eax,eax                                          |
			// 004BB433 | C705 48A35900 F41A0000     | mov dword ptr ds:[59A348],1AF4                       | Port
		}
	}
}
