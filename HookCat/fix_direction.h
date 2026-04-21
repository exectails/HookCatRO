#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class FixDirection : public Mod
		{
		private:
			bool enabled;

		public:
			FixDirection();
			void OnLoad();
		};
	}
}
