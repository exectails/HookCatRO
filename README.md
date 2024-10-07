HookCat (RO Alpha Edition)
=============================================================================

HookCat is a memory patcher library I hacked together to simplify the
process of applying patches to applications, by injecting it as a DLL
into the process and modifying the memory on launch. This removes the
need to modify executables with hex editors or patchers and opens up
new possibilities.

The RO edition of this library targets the 2001-08-30 iRO alpha client
and enables the following mods out of the box.

- Enable multi-client
- Configure chat spam limitations
- Disable draw error message
- Hide disclaimer
- Fix pink borders around text
- Modify the connection information
- Change the window size in windowed mode

How it works
-----------------------------------------------------------------------------

The library consists of two files: `dinput8.dll` and `dinput8.ini`.
The DLL is a DirectX library that any DirectX application will load
by default, and this special version of it will forward any DirectX
requests to the actual DLL, while also applying the custom mods
behind the scenes.

The ini allows you to enable or disable any of the mods and configure
them as you see fit.

How to use
-----------------------------------------------------------------------------

- Download the DLL and INI and place them in your RO alpha client folder.
- That's it.

Disclaimer
-----------------------------------------------------------------------------

The code isn't great, but it does what it's supposed to do. If you'd like
to improve it or add additional mods, pull requests are always welcome.

Links
-----------------------------------------------------------------------------

- GitHub: https://github.com/exectails/HookCatRO
