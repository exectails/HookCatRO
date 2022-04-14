#pragma once

namespace HookCat
{
	// A mod that modifies loaded modules.
	class Mod
	{
	public:
		// Called during initialization.
		virtual void OnLoad()
		{
		}

		// Called during uninitialization.
		virtual void OnUnload()
		{
		}
	};
}
