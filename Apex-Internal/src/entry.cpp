#include"../includes/includes.hpp"

__declspec() attach* attach::init_thread(HMODULE hModule, void* hBuffer)
{
	pThread->pNtCreateThread->NtCreateThreadEx((LPTHREAD_START_ROUTINE)present::hook_present, 0, 0);
	pThread->pNtCreateThread->NtCreateThreadEx((LPTHREAD_START_ROUTINE)FeaturesThread, 0, 0);
	return nullptr;
}

__declspec() attach* attach::attach_process(HMODULE hModule, DWORD hReasons, LPVOID hBuffer)
{
	if (hReasons != DLL_PROCESS_ATTACH) return nullptr;
	pAttach->init_thread(hModule, hBuffer);
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD hReasons, LPVOID hBuffer)
{
	pAttach->attach_process(hModule, hReasons, hBuffer);
	return TRUE;
}

