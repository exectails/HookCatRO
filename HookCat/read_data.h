#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class ReadData : public Mod
		{
		private:
			bool enabled;

		public:
			ReadData();
			void OnLoad();
		};
	}
}
