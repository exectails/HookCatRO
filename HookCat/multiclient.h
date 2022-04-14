#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class MultiClient : public Mod
		{
		private:
			bool enabled;

		public:
			MultiClient();
			void OnLoad();
		};
	}
}
