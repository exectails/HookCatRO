#pragma once

#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		// Modifies Pleione's chat message handling, reducing the delay
		// between sending messages and removing the checks for sending
		// the same message again.
		class ChatSpam : public Mod
		{
		private:
			bool enabled;
			int maxRepeats;

		public:
			ChatSpam();
			void OnLoad();
		};
	}
}
