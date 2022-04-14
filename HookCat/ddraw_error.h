#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class DDrawError : public Mod
		{
		private:
			bool enabled;

		public:
			DDrawError();
			void OnLoad();
		};
	}
}
