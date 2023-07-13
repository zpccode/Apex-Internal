#pragma once
#include"Client/client.hpp"
#include"../../eac/easyanticheat.hpp"
bool unload = false;
extern "C" 
{
	__declspec() NTSTATUS WINAPI FeaturesThread(LPVOID hBuffer) 
	{
		while (!unload)
		{
			pVisuals->Initialize();
			pAimbot->Initialize();
		}
		return STATUS_SUCCESS;
	}
}