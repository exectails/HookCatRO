#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class NoDisclaimer : public Mod
		{
		private:
			bool enabled;

		public:
			NoDisclaimer();
			void OnLoad();
		};
	}
}
