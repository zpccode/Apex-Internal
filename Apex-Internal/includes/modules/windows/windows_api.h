#pragma once
#include"../spoofer/spoofer.hpp"
#include"../memory/memory.hpp"
#include"../security/global.h"

class Memory
{
public:
	class NTVirtualAllocEx
	{
	public:
		LPVOID WINAPI NtVirtualAllocExW(LPVOID BaseAddr, SIZE_T Size, DWORD AllocType, DWORD Protect) {
			return SpoofReturn(__safecall(VirtualAlloc).get(), BaseAddr, Size, AllocType, Protect);
		}
	}; NTVirtualAllocEx* pNTVirtualAllocEx = new NTVirtualAllocEx();
	class NTVirtualProtect
	{
	public:
		BOOL WINAPI NtVirtualProtectExW(LPVOID BaseAddr, SIZE_T Size, DWORD OldProtect, PDWORD NewProtect) {
			return SpoofReturn(__safecall(VirtualProtect).get(), BaseAddr, Size, OldProtect, NewProtect);
		}
	}; NTVirtualProtect* pNTVirtualProtect = new NTVirtualProtect();
public:
	class NTModules
	{
	public:
		HMODULE WINAPI NtGetModuleHandleExW(LPCSTR ModuleName) {
			return SpoofReturn(__safecall(GetModuleHandleA).get(), ModuleName);
		}
		FARPROC WINAPI NtGetProcAddress(HMODULE hModule, LPCSTR ProcAddr) {
			return SpoofReturn(__safecall(GetProcAddress).get(), hModule, ProcAddr);
		}
		HANDLE NTGetCurrentProcess() {
			return SpoofReturn(__safecall(GetCurrentProcess).get());
		}
	}; NTModules* pNTModules = new NTModules();
public:
	class NtImages
	{
	public:
		DWORD64 WINAPI NtGetModuleSize(DWORD64 ModuleImage)
		{
			IMAGE_DOS_HEADER dos_header = *(IMAGE_DOS_HEADER*)ModuleImage;
			IMAGE_NT_HEADERS nt_headers = *(IMAGE_NT_HEADERS*)(ModuleImage + dos_header.e_lfanew);
			return (DWORD64)nt_headers.OptionalHeader.SizeOfImage;
		}
		PBYTE WINAPI NtGetModuleBytes(HMODULE hModule, DWORD64 hSize, DWORD64 HexCode) {
			return (PBYTE)hModule + hSize - HexCode;
		}
	}; NtImages* pNtImages = new NtImages();
public:
	class NtMemCopy
	{
	public:
		LPVOID WINAPI NtMemCopyEx(void* src, const void* det, size_t size) {
			return SpoofReturn(__safecall(memcpy).get(), src, det, size);
		}
	}; NtMemCopy* pNtMemCopy = new NtMemCopy();
}; Memory* m_pMemory = new Memory();


class Thread
{
public:
	class NtCreateThread
	{
	public:
		HANDLE WINAPI NtRemoteThread(_In_ HANDLE hProcess, _In_ LPSECURITY_ATTRIBUTES Thread, 
			_In_ SIZE_T Size, _In_ LPTHREAD_START_ROUTINE Entry, _In_ LPVOID Param, _In_ DWORD Flags, _In_ LPDWORD ThreadId) {
			return SpoofReturn(__safecall(CreateRemoteThread).get(), hProcess, Thread, Size, Entry, Param, Flags, ThreadId);
		}
	public:
		NTSTATUS WINAPI NtCreateThreadEx(LPTHREAD_START_ROUTINE EntryPoint, LPVOID Buffer, LPDWORD ThreadId)
		{
			LPVOID Address = m_pMemory->pNTVirtualAllocEx->NtVirtualAllocExW(NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			PSYSTEM_MEMORY pMemory = (PSYSTEM_MEMORY)Address;

			pMemory->dwModule = m_pMemory->pNTModules->NtGetModuleHandleExW(skCrypt("ntdll.dll").decrypt());
			pMemory->dwModuleSize = m_pMemory->pNtImages->NtGetModuleSize((DWORD64)pMemory->dwModule);
			pMemory->dwModuleMemory = m_pMemory->pNtImages->NtGetModuleBytes(pMemory->dwModule, pMemory->dwModuleSize, 0x400);

			m_pMemory->pNTVirtualProtect->NtVirtualProtectExW(pMemory->dwModuleMemory, 0x100, PAGE_EXECUTE_READWRITE, &pMemory->dwProtect);
			pMemory->dwEP = (DWORD64)(EntryPoint);
			pMemory->dwParam = Buffer;

			m_pMemory->pNtMemCopy->NtMemCopyEx((LPVOID)pMemory->dwModuleMemory, (LPVOID)thread::InitThread, 0x100);
			this->NtRemoteThread(m_pMemory->pNTModules->NTGetCurrentProcess(), 0, 0x100, (LPTHREAD_START_ROUTINE)pMemory->dwModuleMemory, pMemory, 0, ThreadId);

			return STATUS_SUCCESS;
		}
	}; NtCreateThread* pNtCreateThread = new NtCreateThread();
}; Thread* pThread = new Thread();