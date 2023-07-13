#pragma once
#include"../security/global.h"

#define MAXBUFFER 1024
#define STATUS_SUCCESS 1
#define STATUS_ERROR 0

typedef struct _SYSTEM_MEMORY
{
	DWORD64 dwEP;
	LPVOID dwParam;
	SIZE_T dwSize;
	SIZE_T* dwBytes;
	LPVOID dwBaseAddress;
	HANDLE dwProcess;
	LPVOID dwAddress;
	LPCVOID dwBufferAddress;
	DWORD dwProtect;
	PDWORD dwNewProtect;
	DWORD dwAllocType;
	BOOL dwWrite;
	BOOL dwRead;
	HINSTANCE dwModule;
	DWORD64 dwModuleSize;
	PBYTE dwModuleMemory;
} SYSTEM_MEMORY, * PSYSTEM_MEMORY;
typedef DWORD(*INIT_MEM)(LPVOID P);

extern "C" {namespace thread {
	NTSTATUS WINAPI InitThread(PSYSTEM_MEMORY pMemory) {
		if (pMemory != NULL && pMemory->dwEP) {
			INIT_MEM CALL_MEM = (INIT_MEM)pMemory->dwEP;
			CALL_MEM(pMemory->dwParam);
		}
		return STATUS_SUCCESS;
	}
}}

