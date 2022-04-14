#pragma once

#include <string>
#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class WindowSize : public Mod
		{
		private:
			bool enabled;
			int width;
			int height;

		public:
			WindowSize();
			void OnLoad();
		};
	}
}
