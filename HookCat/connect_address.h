#pragma once

#include <string>
#include "mod.h"

namespace HookCat
{
	namespace Mods
	{
		class ConnectAddress : public Mod
		{
		private:
			bool enabled;
			bool enableArgs;
			std::string ip;
			char ip2[15];
			int port;

		public:
			ConnectAddress();
			void OnLoad();
		};
	}
}
