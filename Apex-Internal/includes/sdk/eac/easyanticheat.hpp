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
	LPVOID BaseProcAddr = nullptr;
}}

class EasyAntiCheat
{
public:
	HANDLE WINAPI NtCreateFileW(LPCWSTR File, DWORD Access, DWORD Read, LPSECURITY_ATTRIBUTES Attr, DWORD Flags, DWORD Open, HANDLE hFile) {
		return SpoofReturn(__safecall(CreateFileW).get(), File, Access, Read, Attr, Flags, Open, hFile);
	}

	SC_HANDLE WINAPI NtCreateService(_In_  SC_HANDLE    hSCManager,
		_In_        LPCSTR     lpServiceName,
		_In_opt_    LPCSTR     lpDisplayName,
		_In_        DWORD        dwDesiredAccess,
		_In_        DWORD        dwServiceType,
		_In_        DWORD        dwStartType,
		_In_        DWORD        dwErrorControl,
		_In_opt_    LPCSTR     lpBinaryPathName,
		_In_opt_    LPCSTR     lpLoadOrderGroup,
		_Out_opt_   LPDWORD      lpdwTagId,
		_In_opt_    LPCSTR     lpDependencies,
		_In_opt_    LPCSTR     lpServiceStartName,
		_In_opt_    LPCSTR     lpPassword) {
		return SpoofReturn(__safecall(CreateServiceA).get(), hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType, dwStartType, 
			dwErrorControl, lpBinaryPathName, lpLoadOrderGroup, lpdwTagId, lpDependencies, lpServiceStartName, lpPassword);
	}

	NTSTATUS WINAPI LoadService(LPCWSTR File, DWORD Access) {
		if (!File || !Access)
			return STATUS_ERROR;

		SC_HANDLE Service = OpenSCManagerA((LPCSTR)File, 0, Access);
		SC_HANDLE Manager = CreateServiceA(Service, (LPCSTR)File, "EasyAntiCheat", Access, SERVER_ALL_ACCESS, 0, 0, 0, 0, 0, 0, 0, 0);

		if (!Service || !Manager)
			return STATUS_ERROR;

		this->NtCreateService(Manager, (LPCSTR)File, "EasyAntiCheat", Access, SERVICE_ALL_ACCESS, 0, 0, 0, 0, 0, 0, 0, 0);
	}
public:
	NTSTATUS WINAPI Initialize() {

		LPVOID Service = this->NtCreateFileW(__(L"\\\\.\EasyAntiCheat.sys"), GENERIC_READ, FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, NULL);
		LPVOID EnableService = (LPVOID)m_pMemory->pNTModules->NtGetProcAddress((HMODULE)Service, __("EnableService"));

		if (!Service || !EnableService)
			return STATUS_ERROR;

		PSYSTEM_MEMORY pMemory = (PSYSTEM_MEMORY)m_pMemory->pNTVirtualAllocEx->NtVirtualAllocExW(0, 0x500, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		BOOL First = m_pMemory->pNTVirtualProtect->NtVirtualProtectExW(Service, 0x400, PAGE_EXECUTE_READWRITE, &pMemory->dwProtect);
		BOOL Second = m_pMemory->pNTVirtualProtect->NtVirtualProtectExW(EnableService, 0x100, PAGE_EXECUTE_READWRITE, &pMemory->dwProtect);

		if (!First || !Second)
			return STATUS_ERROR;

		this->LoadService(L"\\\\.\EasyAntiCheat.sys", GENERIC_EXECUTE);
	}
}; EasyAntiCheat* pEasyAntiCheat = new EasyAntiCheat();