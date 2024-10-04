#pragma once

#include "hookcat.h"

#include "chatspam.h"
#include "clean_text.h"
#include "connect_address.h"
#include "ddraw_error.h"
#include "multiclient.h"
#include "no_disclaimer.h"
#include "window_size.h"

using namespace std;
using namespace HookCat::Mods;

void AddMods()
{
	// List of mods to load.

	Kitten.AddMod(make_shared<ChatSpam>());
	Kitten.AddMod(make_shared<CleanText>());
	Kitten.AddMod(make_shared<ConnectAddress>());
	Kitten.AddMod(make_shared<DDrawError>());
	Kitten.AddMod(make_shared<MultiClient>());
	Kitten.AddMod(make_shared<NoDisclaimer>());
	Kitten.AddMod(make_shared<WindowSize>());
}
