#pragma once
#include"memory/memory.hpp"
#include"spoofer/spoofer.hpp"

#pragma section(".text")
__declspec(allocate(".text")) const unsigned char jmp_rdx[] = { 0xFF, 0x27 };

extern "C" {namespace spoof_call 
{
	HINSTANCE WINAPI GetModuleBase(_In_ LPCSTR szModuleName) { 
		return SpoofReturn(__safecall(GetModuleHandleA).get(), szModuleName); }
	FARPROC WINAPI GetModuleProcAddr(_In_ HMODULE hModule, _In_ LPCSTR szModuleProc) { 
		return SpoofReturn(__safecall(GetProcAddress).get(), hModule, szModuleProc); }
	LPVOID WINAPI SafeCopy(_In_ void* Source, _In_ const void* Detour, _In_ size_t Size) { 
		return SpoofReturn(__safecall(memcpy).get(), Source, Detour, Size); }
	BOOL WINAPI GetKey(int Key) { 
		return SpoofReturn(__safecall(GetAsyncKeyState).get(), Key); }
	BOOL WINAPI VirtualProcMem(_In_ LPVOID BaseAddr, _In_ SIZE_T BaseSize, _In_ DWORD OldProtect, _In_ PDWORD NewProtect) {
		return SpoofReturn(__safecall(VirtualProtect).get(), BaseAddr, BaseSize, OldProtect, NewProtect); }
	LPVOID WINAPI VirtualAllocMem(_In_ LPVOID BaseAddr, _In_ SIZE_T BaseSize, _In_ DWORD AllocType, _In_ DWORD NewProtect) {
		return SpoofReturn(__safecall(VirtualAlloc).get(), BaseAddr, BaseSize, AllocType, NewProtect); }
	HANDLE NTCreateRemoteThread(_In_ HANDLE hProcess, _In_ LPSECURITY_ATTRIBUTES Thread, _In_ SIZE_T Size, _In_ LPTHREAD_START_ROUTINE Entry, _In_ LPVOID Param, _In_ DWORD Flags, _In_ LPDWORD ThreadId) {
		return SpoofReturn(__safecall(CreateRemoteThread).get(), hProcess, Thread, Size, Entry, Param, Flags, ThreadId); }
	HANDLE NTGetCurrentProcess() { 
		return SpoofReturn(__safecall(GetCurrentProcess).get()); }
	BOOL _DeleteWindow(_In_ HWND hWindow) {
		return SpoofReturn(__safecall(DestroyWindow).get(), hWindow); }
	ATOM WINAPI RegisterWndClas(_In_ const WNDCLASSEX* WndClass) {
		return SpoofReturn(RegisterClassEx, WndClass); }
	ATOM WINAPI UnregisterWndClass(_In_ LPCWSTR Class, _In_ HINSTANCE hInst) {
		return SpoofReturn(UnregisterClass, Class, hInst); }
	BOOL WINAPI CreateConsole() {
		return SpoofReturn(__safecall(AllocConsole).get()); }
	errno_t WINAPI open_s(_In_ FILE** _Stream, _In_ const char* _FILENAME, _In_ const char* _MODE, _In_ FILE* _OldStream) {
		return SpoofReturn(__safecall(freopen_s).get(), _Stream, _FILENAME, _MODE, _OldStream); }
	VOID WINAPI SleepThread(_In_ DWORD Time) {
		return SpoofReturn(__safecall(Sleep).get(), Time); }
	LPVOID WINAPI SafeMemSet(_In_ void* ptr, _In_ int Value, _In_ size_t size) {
		return SpoofReturn(__safecall(memset).get(), ptr, Value, size); }
	LRESULT WINAPI CallWndProc(_In_ WNDPROC WndProc, _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		return SpoofReturn(CallWindowProcW, WndProc, hWnd, uMsg, wParam, lParam); }
	LRESULT WINAPI DefWndProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_  WPARAM wParam, _In_ LPARAM lParam) {
		return SpoofReturn(DefWindowProc, hWnd, uMsg, wParam, lParam); }
	HWND WINAPI NtCreateWindow(_In_ DWORD Exit, _In_ LPCWSTR Class, _In_ LPCWSTR Name, _In_ DWORD Style, _In_ int x, _In_ int y, _In_ int w, _In_ int h,
		_In_ HWND hWindow, _In_ HMENU hMenu, _In_ HINSTANCE hInst, _In_ LPVOID Param) {
		return SpoofReturn(CreateWindowExW, Exit, Class, Name, Style, x, y, w, h, hWindow, hMenu, hInst, Param); }
	int __cdecl NtStrCmp(_In_ const char* offset, _In_ const char* pointer) {
		return SpoofReturn(__safecall(strcmp).get(), offset, pointer); }
	BOOL WINAPI SetCursorPosNew(_In_ int x, _In_ int y) {
		return SpoofReturn(__safecall(SetCursorPos).get(), x, y); }
	size_t __cdecl StrLen(_In_ const char* offset) {
		return SpoofReturn(__safecall(strlen).get(), offset); }
	BOOL WINAPI NtGetModuleInformation(_In_ HANDLE hProcess, _In_ HMODULE hModule, _In_ LPMODULEINFO Info, _In_ DWORD cb) {
		return SpoofReturn(__safecall(K32GetModuleInformation).get(), hProcess, hModule, Info, cb); }
	LONG_PTR WINAPI NtSetWindowLongPtr(_In_ HWND hWindow, _In_ INT hIndex, _In_ LONG_PTR dwLong) {
		return SpoofReturn(__safecall(SetWindowLongPtrW,).get(), hWindow, hIndex, dwLong); }
	LRESULT WINAPI NtDefWndProc(_In_ HWND hWindow, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		return SpoofReturn(DefWindowProc, hWindow, uMsg, wParam, lParam);  }
	LPVOID WINAPI NtCalloc(_In_ size_t base, _In_ size_t address) {
		return SpoofReturn(__safecall(calloc).get(), base, address); }
}}

extern "C" {namespace modules 
{
	DWORD64 WINAPI GetModuleSize(_In_ DWORD64 Image) {
		IMAGE_DOS_HEADER dos_header = *(IMAGE_DOS_HEADER*)Image;
		IMAGE_NT_HEADERS nt_headers = *(IMAGE_NT_HEADERS*)(Image + dos_header.e_lfanew);
		return (DWORD64)nt_headers.OptionalHeader.SizeOfImage;
	}
	PBYTE WINAPI GetModuleBytes(_In_ HINSTANCE hInstace, _In_ DWORD64 Image, _In_ DWORD64 Size) {
		PBYTE Memory = (PBYTE)hInstace + Image - Size;
		return Memory;
	}
}}

extern "C" {namespace thread 
{
	NTSTATUS WINAPI NTCreateThread(_In_ LPTHREAD_START_ROUTINE EntryPoint, _In_ LPVOID Buffer, _In_ LPDWORD ThreadId)
	{
		LPVOID Address = spoof_call::VirtualAllocMem(NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		PSYSTEM_MEMORY pMemory = (PSYSTEM_MEMORY)Address;
		
		pMemory->dwModule = spoof_call::GetModuleBase(skCrypt("ntdll.dll").decrypt());
		pMemory->dwModuleSize = modules::GetModuleSize((DWORD64)pMemory->dwModule);
		pMemory->dwModuleMemory = modules::GetModuleBytes(pMemory->dwModule, pMemory->dwModuleSize, 0x400);
		
		spoof_call::VirtualProcMem(pMemory->dwModuleMemory, 0x100, PAGE_EXECUTE_READWRITE, &pMemory->dwProtect);
		pMemory->dwEP = (DWORD64)(EntryPoint);
		pMemory->dwParam = Buffer;

		spoof_call::SafeCopy((LPVOID)pMemory->dwModuleMemory, (LPVOID)InitThread, 0x100);
		spoof_call::NTCreateRemoteThread(spoof_call::NTGetCurrentProcess(), 0, 0x100, (LPTHREAD_START_ROUTINE)pMemory->dwModuleMemory, pMemory, 0, ThreadId);
		
		return STATUS_SUCCESS;
	}
}}