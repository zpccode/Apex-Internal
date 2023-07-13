#pragma once
#include<Windows.h>
class attach
{
public:
	__declspec() attach* attach_process(HMODULE hModule, DWORD hReasons, LPVOID hBuffer);
	__declspec() attach* init_thread(HMODULE hModule, void* hBuffer);
}; attach* pAttach;