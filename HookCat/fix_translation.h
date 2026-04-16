#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class FixTranslation : public Mod
		{
		private:
			bool enabled;

		public:
			FixTranslation();
			void OnLoad();
		};
	}
}
