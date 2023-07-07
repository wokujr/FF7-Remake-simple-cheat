#include "mem.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
	//create Console
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);


	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		// NUMPAD key to activate cheat here
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{

		}

		//Sleep(5);

		fclose(file);
		FreeConsole();
		FreeLibraryAndExitThread(hModule, 0);
		return 0;

	}

}


BOOL APIENTRY DLLMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;

}
