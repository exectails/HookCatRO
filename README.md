HookCat (RO Edition)
=============================================================================

HookCat is a memory patcher library I hacked together to simplify the
process of applying patches to applications, by injecting it as a DLL
into the process and modifying the memory on launch. This removes the
need to modify executables with hex editors or patchers and opens up
new possibilities.

The RO edition of this library targets early clients, such as iRO Alpha
(2001-08-30) and Beta1 (2002-02-20) and enables the following mods out
of the box.

- Enable multi-client
- Configure chat spam limitations
- Disable draw error message
- Hide disclaimer
- Fix pink borders around text
- Modify the connection information
- Change the window size in windowed mode
- Improved mouse capture handling

How it works
-----------------------------------------------------------------------------

The library consists of three files: `dinput.dll`, `dinput8.dll` and
`dinput8.ini`. The DLLs are DirectX wrappers that any DirectX application
will load by default, and these special versions of them will forward any
DirectX requests to the actual DLL, while also applying the custom mods
behind the scenes.

The ini allows you to enable or disable any of the mods and configure
them as you see fit.

How to use
-----------------------------------------------------------------------------

- Download the DLLs and INI and place them in your RO alpha client folder.
- That's it.

Disclaimer
-----------------------------------------------------------------------------

The code isn't great, but it does what it's supposed to do. If you'd like
to improve it or add additional mods, pull requests are always welcome.

Links
-----------------------------------------------------------------------------

- GitHub: https://github.com/exectails/HookCatRO
