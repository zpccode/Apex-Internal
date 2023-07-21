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
	HANDLE WINAPI NtCreateFileW(_In_ LPCWSTR FileName, _In_ DWORD Access, _In_ DWORD Handle, 
		_In_ LPSECURITY_ATTRIBUTES Attr, _In_ DWORD Flags, _In_ DWORD xFlags, _In_ HANDLE hFile) {
		return SpoofReturn(__safecall(CreateFileW).get(), FileName, Access, Handle, Attr, Flags, xFlags, hFile); }
	BOOL WINAPI NtDeviceIoToControl(_In_ HANDLE hHandle, _In_ DWORD dword, _In_ LPVOID lpvoid, _In_ DWORD Dword, _In_ 
		LPVOID Lpvoid, _In_ DWORD dWord, _In_ LPDWORD lpdword, _In_ LPOVERLAPPED lpOverLapped) {
		return SpoofReturn(__safecall(DeviceIoControl).get(), hHandle, dword, lpvoid, Dword, Lpvoid, dWord, lpdword, lpOverLapped);
	}
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

extern "C" { namespace anti_cheat_globals
{
	LPVOID BaseFile = nullptr;
	LPVOID BaseAddress = nullptr;
	DWORD64 BaseSize = NULL;
	PBYTE BaseMemory = nullptr;
	BOOL CallIoFunc = FALSE;
}}

extern "C" {namespace easyanticheat
{
	NTSTATUS WINAPI Manager(SC_HANDLE* pHandle)
	{
		*pHandle = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (pHandle) return STATUS_SUCCESS;
		return STATUS_ERROR;
	}

	NTSTATUS WINAPI OpenDriverEx(LPCWSTR Name, DWORD Access, SC_HANDLE* manager, SC_HANDLE* Driver, INT Level)
	{
		auto _manager = manager;
		if (!manager || !Manager(manager))
			return STATUS_ERROR;
		*Driver = OpenService(*manager, Name, Access);
		if (!*Driver) { 
			if (Level > 1) { return STATUS_ERROR; } 
			if (!_manager) CloseServiceHandle(*manager); 
			return STATUS_ERROR; }
		return STATUS_SUCCESS;
	}

	NTSTATUS WINAPI GetStatus(LPCWSTR Name, SERVICE_STATUS* status, SC_HANDLE* manager = 0)
	{
		bool level = 0;
		SC_HANDLE handle, service = 0;
		
		if (manager)
			handle = *manager;
		
		if (!OpenDriverEx(Name, SERVICE_QUERY_STATUS, &handle, &service, 0)) { return STATUS_ERROR; }
		
		if (!QueryServiceStatus(service, status))
			MessageBoxA(NULL, "Failed", "Status", NULL);
		else level = 1;

		if (service)
			CloseServiceHandle(service);
		if (!manager)
			CloseServiceHandle(handle);

		return level;
	}

	NTSTATUS WINAPI LoadDriver(LPCWSTR Path, LPCWSTR Name, LPCWSTR Display, SC_HANDLE* manager = 0, bool overwrite = 0)
	{
		bool level = 0;
		auto _manager = manager;
		SC_HANDLE handle;

		if (manager)
			handle = *manager;
		else if (!Manager(&handle))
			return STATUS_ERROR;
	CREATE_SERVICE:
		SC_HANDLE service = CreateService(handle, Name, Display, SERVICE_START | DELETE | SERVICE_STOP,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE,
			Path,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

		if (!service)
		{
			if (overwrite) { goto CREATE_SERVICE; }
			else
				MessageBoxA(NULL, "FAiled ", "Create Service", 0);
			goto CLOSE;
		}

		if (!StartServiceA(service, NULL, NULL))
			MessageBoxA(NULL, "Fauled", "Service can not be run", 0);
		else level = 1;
	CLOSE:
		if (service)
			CloseServiceHandle(service);
		if (!_manager)
			CloseServiceHandle(handle);

		return level;
	}

	NTSTATUS WINAPI Initialize()
	{

	}
}}
