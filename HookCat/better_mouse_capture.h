#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class BetterMouseCapture : public Mod
		{
		private:
			bool enabled;

		public:
			BetterMouseCapture();
			void OnLoad();
		};
	}
}
