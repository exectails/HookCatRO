#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class CleanText : public Mod
		{
		private:
			bool enabled;

		public:
			CleanText();
			void OnLoad();
		};
	}
}
