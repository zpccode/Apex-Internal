#pragma once
#include"../../modules/modules.hpp"
#include <tchar.h>
#define buffer_size 1024
#define packet_max_size 4096

typedef struct  _EAC_SOCKETS
{
	ULONG MaxSize[packet_max_size];
	SOCKET pSocket;
	LPCSTR SocketName;
} EAC_SOCKET, * PEAC_SOCKET;

extern "C" {namespace service_handles
{
	SC_HANDLE WINAPI NtOpenService(SC_HANDLE hHandle, LPCSTR hServiceName, DWORD Access) {
		return SpoofReturn(__safecall(OpenServiceA).get(), hHandle, hServiceName, Access); }
	HANDLE WINAPI NtOpenProcessEx(DWORD Access, BOOL Flags, DWORD pID) {
		return SpoofReturn(__safecall(OpenProcess).get(), Access, Flags, pID); }
	BOOL WINAPI NtStartServiceEx(SC_HANDLE hService, DWORD Flags, LPCSTR* Offset) {
		return SpoofReturn(__safecall(StartServiceA).get(), hService, Flags, Offset); }
	VOID WINAPI GetInfo(_In_ LPSTARTUPINFOW Info) {
		return SpoofReturn(__safecall(GetStartupInfoW).get(), Info); }
}}

extern "C" {namespace anticheatentry
{
	DWORD64 WINAPI GetModuleSizeEx(DWORD64 BaseAddress) {
		IMAGE_DOS_HEADER dos_header = *(IMAGE_DOS_HEADER*)BaseAddress;
		IMAGE_NT_HEADERS nt_headers = *(IMAGE_NT_HEADERS*)(BaseAddress + dos_header.e_lfanew);
		return (DWORD64)nt_headers.OptionalHeader.SizeOfImage; }

	DWORD64 WINAPI GetEntryPoint(DWORD64 BaseAddress) {
		IMAGE_DOS_HEADER dos_header = *(IMAGE_DOS_HEADER*)BaseAddress;
		IMAGE_NT_HEADERS nt_headers = *(IMAGE_NT_HEADERS*)(BaseAddress + dos_header.e_lfanew);
		return (DWORD64)nt_headers.OptionalHeader.AddressOfEntryPoint; }
}}

extern "C" {namespace easyanticheat
{
	NTSTATUS WINAPI Initialize()
	{
		LPVOID EasyAnitCheatAddress = (LPVOID)spoof_call::GetModuleBase(skCrypt("easyanticheat.dll").decrypt());
		DWORD64 EasyAnitCheatEntryPoint = anticheatentry::GetEntryPoint((DWORD64)EasyAnitCheatAddress);
		
		if (!EasyAnitCheatAddress || !EasyAnitCheatEntryPoint)
			return STATUS_ERROR;



		return STATUS_SUCCESS;
	}
}}
