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

			auto address1 = Kitten.Find("RagExe.exe", "BF F8BA4E00"); // 211.239.123.168
			auto address2 = Kitten.Find("RagExe.exe", "C705 70605200 581B0000"); // 7000

			if (!address1 || !address2)
			{
				Kitten.Log.Info("  patch failed address not found.");
				return;
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
						std::string newPort = cmdLine.substr(index + 3, endIndex - index - 3);
						port = atoi(newPort.c_str());
						Kitten.Log.Info("  overwriting conf port with argument %d.", port);
					}
				}
			}

			// ==== IP ====

			const char* ipStrPtr = ip2;

			std::string patchStr1;
			patchStr1 += "BF";
			patchStr1 += HexTool::GetString((int)ipStrPtr);

			Kitten.Patch("RagExe.exe", address1.value(), patchStr1.c_str());

			// ==== Port ====

			std::string patchStr2;
			patchStr2 += "C705 70605200";
			patchStr2 += HexTool::GetString(port);

			Kitten.Patch("RagExe.exe", address2.value(), patchStr2.c_str());

			// ==============

			Kitten.Log.Info("  patches applied at 0x%08X and 0x%08X.", address1.value(), address2.value());
			Kitten.Log.Info("  new address: %s:%d", ip2, port);

			//0048BD56 | B9 40645200                | mov ecx,ragexe.526440                                |
			//0048BD5B | C705 70605200 581B0000     | mov dword ptr ds:[526070],1B58                       | Port
			//0048BD65 | E8 762C0100                | call <ragexe.sub_49E9E0>                             |
			//0048BD6A | BF F8BA4E00                | mov edi,ragexe.4EBAF8                                | IP
			//0048BD6F | 83C9 FF                    | or ecx,FFFFFFFF                                      |
		}
	}
}
