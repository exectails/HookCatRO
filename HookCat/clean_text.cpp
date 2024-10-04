// Name: CleanText
// Author: exec
// 
// Fixed pink text by disabling aliasing for the fonts the client creates.

#include "clean_text.h"
#include "hookcat.h"

namespace HookCat
{
	namespace Mods
	{
		CleanText::CleanText()
		{
			enabled = Kitten.Conf.GetBool("CleanText", "Enabled", false);
		}

		void CleanText::OnLoad()
		{
			if (!enabled)
				return;

			Kitten.Log.Info("Loading CleanText...");

			auto addresses = Kitten.FindAll("RagExe.exe", "50  6A 10  6A 00  6A 00  6A 00  6A 01  6A 00  6A 00  6A 00  68 90010000  6A 00  6A 00  6A 00  51  FF15 40204D00");

			if (addresses->size() == 0)
			{
				Kitten.Log.Info("  patch failed, no addresses found.");
				return;
			}

			Kitten.Log.Info("  found %d instances of CreateFont.", addresses->size());

			for (auto const& address : *addresses)
			{
				Kitten.Patch("RagExe.exe", address, "50  6A 10  6A 03  6A 00  6A 00  6A 01  6A 00  6A 00  6A 00  68 90010000  6A 00  6A 00  6A 00  51  FF15 40204D00");
				Kitten.Log.Info("  patch applied at 0x%08X.", address);
			}

			// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createfonta
			// https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/9518fece-d2f2-4799-9df6-ba3db1d73371

			// We want iQuality to be 3 (NONANTIALIASED_QUALITY) on modern
			// systems, so the text doesn't get anti-aliased.

			//0040BA94 | 50                         | push eax                                             |
			//0040BA95 | 6A 10                      | push 10                                              |
			//0040BA97 | 6A 00                      | push 0                                               | << iQuality
			//0040BA99 | 6A 00                      | push 0                                               |
			//0040BA9B | 6A 00                      | push 0                                               |
			//0040BA9D | 6A 01                      | push 1                                               |
			//0040BA9F | 6A 00                      | push 0                                               |
			//0040BAA1 | 6A 00                      | push 0                                               |
			//0040BAA3 | 6A 00                      | push 0                                               |
			//0040BAA5 | 68 90010000                | push 190                                             |
			//0040BAAA | 6A 00                      | push 0                                               |
			//0040BAAC | 6A 00                      | push 0                                               |
			//0040BAAE | 6A 00                      | push 0                                               |
			//0040BAB0 | 51                         | push ecx                                             |
			//0040BAB1 | FF15 40204D00              | call dword ptr ds:[<&CreateFontA>]                   |

			//0045452F | 50                         | push eax                                             |
			//00454530 | 6A 10                      | push 10                                              |
			//00454532 | 6A 00                      | push 0                                               |
			//00454534 | 6A 00                      | push 0                                               |
			//00454536 | 6A 00                      | push 0                                               |
			//00454538 | 6A 01                      | push 1                                               |
			//0045453A | 6A 00                      | push 0                                               |
			//0045453C | 6A 00                      | push 0                                               |
			//0045453E | 6A 00                      | push 0                                               |
			//00454540 | 68 90010000                | push 190                                             |
			//00454545 | 6A 00                      | push 0                                               |
			//00454547 | 6A 00                      | push 0                                               |
			//00454549 | 6A 00                      | push 0                                               |
			//0045454B | 51                         | push ecx                                             |
			//0045454C | FF15 40204D00              | call dword ptr ds:[<&CreateFontA>]                   |

			//00454646 | 50                         | push eax                                             |
			//00454647 | 6A 10                      | push 10                                              |
			//00454649 | 6A 00                      | push 0                                               |
			//0045464B | 6A 00                      | push 0                                               |
			//0045464D | 6A 00                      | push 0                                               |
			//0045464F | 6A 01                      | push 1                                               |
			//00454651 | 6A 00                      | push 0                                               |
			//00454653 | 6A 00                      | push 0                                               |
			//00454655 | 6A 00                      | push 0                                               |
			//00454657 | 68 90010000                | push 190                                             |
			//0045465C | 6A 00                      | push 0                                               |
			//0045465E | 6A 00                      | push 0                                               |
			//00454660 | 6A 00                      | push 0                                               |
			//00454662 | 51                         | push ecx                                             |
			//00454663 | FF15 40204D00              | call dword ptr ds:[<&CreateFontA>]                   |
		}
	}
}
