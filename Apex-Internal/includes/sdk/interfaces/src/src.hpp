#pragma once
#include"Client/client.hpp"
#include"../../eac/easyanticheat.hpp"

bool unload = false;
extern "C"  {
	__declspec() NTSTATUS WINAPI FeaturesThread(LPVOID hBuffer) 
	{
		while (!unload)
		{
			pEasyAntiCheat->Initialize();
			pAimbot->Initialize();
			pVisuals->Initialize();
			pMisc->Initialize();
		}
		return STATUS_SUCCESS;
	}
}