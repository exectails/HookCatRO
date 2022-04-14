// Name: Pake
// Author: exec
// 
// Pake hooks the client's send and receive functions for network packets,
// which allows it to forward the packets to its own modules, and to take
// new packets from said modules to send or receive them. This essentially
// allows users to edit packets and send custom ones as they see fit.

#include "pake.h"

#include <cctype>
#include <filesystem>
#include "hextool.h"
#include "hookcat.h"
#include "util.h"

namespace HookCat
{
	namespace Mods
	{
		HookInfo gSendHook, gRecvHook;
		std::unordered_map<std::string, PakeMod> *gMods;
		void* gCClientHandlerEx;

		Pake::Pake()
		{
			enabled = Kitten.Conf.GetBool("Pake", "Enabled", false);
			listFilePath = Kitten.Conf.GetString("Pake", "List");
		}

		int __stdcall Send(const char* data, uint32_t size);
		int __stdcall Recv(const char* data, uint32_t size);

		// Sahara2.dll - sahara2::CClientHandlerEx::Send
		char __fastcall sahara2_CClientHandlerEx_Send(void* self, void* _EDX, const char* data, uint32_t size, bool a4, bool a5)
		{
			// Save the client handle reference so we can use it in our own
			// send calls, and overwrite every time in case it changes.
			gCClientHandlerEx = self;

			// Execute mod hooks, and if one of them returns 0, don't send
			// the packet, but act as if it was.
			auto stop = false;
			for (auto it = gMods->begin(); it != gMods->end(); ++it)
			{
				if (!it->second.SendHook(data, size))
					stop = true;
			}

			if (stop)
				return 1;

			return ((decltype(sahara2_CClientHandlerEx_Send)*)gSendHook.originalFunc)(self, _EDX, data, size, a4, a5);
		}

		// Pleione.dll - pleione::CPleioneNetworkHandler::UserProcedure
		char __stdcall pleione_CPleioneNetworkHandler_UserProcedure(const char* data, uint32_t size)
		{
			// Execute mod hooks, and if one of them returns 0, don't send
			// the packet, but act as if it was.
			auto stop = false;
			for (auto it = gMods->begin(); it != gMods->end(); ++it)
			{
				if (!it->second.RecvHook(data, size))
					stop = true;
			}

			if (stop)
				return 1;

			return ((decltype(pleione_CPleioneNetworkHandler_UserProcedure)*)gRecvHook.originalFunc)(data, size);
		}

		// Send function called by mods to send packets to the server.
		// Always returns 1.
		int __stdcall Send(const char* data, uint32_t size)
		{
			if (gSendHook)
				((decltype(sahara2_CClientHandlerEx_Send)*)gSendHook.originalFunc)(gCClientHandlerEx, NULL, data, size, 0, 0);

			return 1;
		}

		// Receive function called by mods to send packets to the client.
		// Always returns 1.
		int __stdcall Recv(const char* data, uint32_t size)
		{
			if (gRecvHook)
				((decltype(pleione_CPleioneNetworkHandler_UserProcedure)*)gRecvHook.originalFunc)(data, size);

			return 1;
		}

		void Pake::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading Pake...");

			// Check if list file exists
			std::ifstream stream(listFilePath);
			if (!stream.good())
			{
				Kitten.Log.Error("  mod list '%s' not found.", listFilePath.c_str());
				return;
			}

			// Try to hook required functions
			gSendHook = Kitten.Hook("Sahara2.dll", "?Send@CClientHandlerEx@sahara2@@QAE_NPBXK_N1@Z", &sahara2_CClientHandlerEx_Send);
			if (!gSendHook)
			{
				Kitten.Log.Error("  failed to hook send function.");
				return;
			}

			gRecvHook = Kitten.Hook("Pleione.dll", "?UserProcedure@CPleioneNetworkHandler@pleione@@UAE_NPBXK@Z", &pleione_CPleioneNetworkHandler_UserProcedure);
			if (!gRecvHook)
			{
				Kitten.Log.Error("  failed to hook receive function.");
				return;
			}

			Kitten.Log.Info("  hooked send and receive functions.");
			Kitten.Log.Info("  reading mod list...");

			// Loads modules from list and initialize them
			std::string line;
			while (std::getline(stream, line))
			{
				// Ignore commented and empty lines
				if (line.find(";") == 0 || IsEmptyOrWhitespace((line)))
					continue;

				// Check if mod file exists
				auto modFilePath = std::filesystem::path(listFilePath).replace_filename(line);
				auto modFilePathStr = modFilePath.string();
				auto modFilePathCStr = modFilePathStr.c_str();
				if (!std::filesystem::exists(modFilePath))
				{
					Kitten.Log.Error("  file '%s' not found.", modFilePathCStr);
					continue;
				}

				auto lineCStr = line.c_str();

				Kitten.Log.Info("  loading %s...", lineCStr);

				// Try to load DLL
				PakeMod mod;
				mod.hModule = LoadLibrary(modFilePathCStr);
				if (!mod.hModule)
				{
					Kitten.Log.Error("  failed to load library %s.", lineCStr);
					continue;
				}

				// Try to initialize mod
				mod.Initialize = (InitializeFunc)GetProcAddress(mod.hModule, "Initialize");
				mod.Terminate = (TerminateFunc)GetProcAddress(mod.hModule, "Terminate");
				mod.SendHook = (SendHookFunc)GetProcAddress(mod.hModule, "SendHook");
				mod.RecvHook = (RecvHookFunc)GetProcAddress(mod.hModule, "RecvHook");

				if (!mod.Initialize)
				{
					Kitten.Log.Warning("  mod %s does not expose an Initialize function.", lineCStr);
					continue;
				}

				if (!mod.Initialize(mod.hModule, &Send, &Recv))
				{
					Kitten.Log.Warning("  mod %s failed to initialize.", lineCStr);
					continue;
				}

				// Add mod to list of mods to receive packets if the it
				// exposes one of the respective functions.
				if (mod.SendHook || mod.RecvHook)
					mods[lineCStr] = mod;
			}

			gMods = &mods;
		}

		void Pake::OnUnload()
		{
			Kitten.Log.Info("Unloading Pake...");

			if (gSendHook && !Kitten.Unhook(gSendHook))
				Kitten.Log.Error("  failed to unhook send function.");

			if (gRecvHook && !Kitten.Unhook(gRecvHook))
				Kitten.Log.Error("  failed to unhook receive function.");
		}
	}
}