#pragma once

#include <Windows.h>

typedef int(__stdcall* SendHookFunc)(const char*, uint32_t length);
typedef int(__stdcall* RecvHookFunc)(const char*, uint32_t length);
typedef int(__stdcall* SendFunc)(const char*, uint32_t length);
typedef int(__stdcall* RecvFunc)(const char*, uint32_t length);
typedef int(__stdcall* InitializeFunc)(HINSTANCE, SendFunc, RecvFunc);
typedef void(__stdcall* TerminateFunc)();
typedef void(__cdecl* WriteLogFunc)(const char*, ...);

class PakeMod
{
public:
	HMODULE hModule;

	InitializeFunc Initialize = nullptr;
	TerminateFunc Terminate = nullptr;
	SendHookFunc SendHook = nullptr;
	RecvHookFunc RecvHook = nullptr;
};
