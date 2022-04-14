#pragma once

#include <unordered_map>
#include "mod.h"
#include "pakemod.h"

namespace HookCat
{
	namespace Mods
	{
		class Pake : public Mod
		{
			bool enabled;
			std::string listFilePath;
			std::unordered_map<std::string, PakeMod> mods;

		public:
			Pake();
			void OnLoad();
			void OnUnload();
		};
	}
}
