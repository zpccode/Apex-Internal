#pragma once
#include"../../sdk.hpp"

class Proc
{
public:
	BOOL WINAPI NtReadProcMem(HANDLE hProcess, LPCVOID SourceAddress, LPVOID Buffer, SIZE_T Size, SIZE_T* Bytes) {
		return SpoofReturn(__safecall(ReadProcessMemory).get(), hProcess, SourceAddress, Buffer, Size, Bytes);
	}
	size_t __cdecl NtStrlen(const char* offset) {
		return SpoofReturn(__safecall(strlen).get(), offset);
	}
	long __cdecl NtStrtol(const char* offset, char** pointer, int value) {
		return SpoofReturn(__safecall(strtol).get(), offset, pointer, value);
	}
}; Proc* pProc;

class DumperPatterns
{
public:
	int found, missed = 0;
public:
	template<typename Read>
	Read RPM(SIZE_T Address)
	{
		Read Buffer; 
		pProc->NtReadProcMem(m_pMemory->pNTModules->NTGetCurrentProcess(), 
			(LPVOID)Address, &Buffer, sizeof(Read), nullptr);
		return Buffer;
	}
public:
	__forceinline DWORD64 find_pattern(LPCSTR pattern, LPCSTR mask, DWORD64 begin, DWORD64 end, HANDLE hProc)
	{
		auto scan_pattern = [](LPCSTR pattern, LPCSTR mask, CHAR* begin, UINT size) -> CHAR*
		{
			DWORD64 pattern_length = __safecall(strlen)(mask);
			for (UINT index = 0; index < size - pattern_length; index++) {
				bool found = true;
				for (UINT nindex = 0; nindex < pattern_length; nindex++) {
					if (mask[nindex] != '?' && pattern[nindex] != *(begin + index + nindex)){
						found = false;
						break;
					}
				}
				if (found)
					return (begin + index);
			}
			return nullptr;
		};

		DWORD64 match = 0;
		SIZE_T bytes_read;
		DWORD old_protect;
		CHAR* buffer = nullptr;
		MEMORY_BASIC_INFORMATION mbi = { 0 };

		DWORD64 current_byte = begin;
		for (DWORD64 current_byte = begin; current_byte < end; current_byte += mbi.RegionSize)
		{
			if (!__safecall(VirtualQueryEx).get()(hProc, (LPVOID)current_byte, &mbi, sizeof(mbi)))
				continue;
			if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
				continue;
			buffer = new CHAR[mbi.RegionSize];
			__safecall(ReadProcessMemory).get()(hProc, mbi.BaseAddress, buffer, mbi.RegionSize, &bytes_read);
			CHAR* internal_address = scan_pattern(pattern, mask, buffer, (UINT)bytes_read);
			if (internal_address != nullptr)
			{
				match = current_byte + (DWORD64)(internal_address - buffer);
				break;
			}
		}

		delete[] buffer;
		return match;
	}
	__forceinline DWORD64 FindPattern(LPCSTR Sig) 
	{
		CHAR pattern[100];
		CHAR mask[100];

		CHAR lastChar = ' ';
		UINT nindex = 0;

		DWORD64 dw_base = (DWORD64)__safecall(GetModuleHandleA).get()(NULL);
		DWORD64 module_size = m_pMemory->pNtImages->NtGetModuleSize(dw_base);
		HANDLE hProc = m_pMemory->pNTModules->NTGetCurrentProcess();

		for (UINT index = 0; index < pProc->NtStrlen(Sig); index++)
		{
			if ((Sig[index] == '?' || Sig[index] == '*') && (lastChar != '?' && lastChar != '*')) {
				pattern[nindex] = mask[nindex] = '?';
				nindex++;
			}

			else if (isspace(lastChar)) {
				pattern[nindex] = lastChar = (CHAR)pProc->NtStrtol(&Sig[index], 0, 16);
				mask[nindex] = 'x';
				nindex++;
			}
		}

		pattern[nindex] = mask[nindex] = '\0';
		auto current_address = this->find_pattern(pattern, mask, dw_base, dw_base + module_size, hProc) + 0x3;

		if (current_address <= 0x5)
		{
			missed++;
			return 0x0;
		}

		found++;
		return current_address + this->RPM<uint32_t>(current_address) + 4 - dw_base;
	}

	__forceinline LPVOID PatternScan(CHAR* Base, DWORD64 Size, CHAR* Pattern, CHAR* Mask)
	{
		DWORD64 PatternLength = pProc->NtStrlen(Mask);
		for (UINT index = 0; index < Size - PatternLength; index++)
		{
			bool found = true;
			for (UINT nindex = 0; nindex < PatternLength; nindex++)
			{
				if (Mask[nindex] != '?' && Pattern[nindex] != *(Base + index + nindex)) {
					found = false;
					break;
				}
			}
			if (found) {
				return (LPVOID)(Base + index);
			}
		}
		return nullptr;
	}
	__forceinline LPVOID PatternScanEx(char* Pattern, char* Mask)
	{
		DWORD64 CurrentChunk = global::g_hGameImage;
		DWORD64 End = global::g_hGameImage + global::g_dwModuleSize;
		SIZE_T BytesRead;
		while (CurrentChunk < End)
		{
			CHAR Buffer[4096];
			DWORD OldProtect;
			pProc->NtReadProcMem(m_pMemory->pNTModules->NTGetCurrentProcess(), (LPVOID)CurrentChunk, &Buffer, sizeof(Buffer), &BytesRead);
			if (BytesRead == 0)
				return nullptr;
			LPVOID InternalAddress = this->PatternScan((CHAR*)&Buffer, BytesRead, Pattern, Mask);
			if (InternalAddress != nullptr)
			{
				DWORD64 OffsetFromBuffer = (DWORD64)InternalAddress - (DWORD64)&Buffer;
				return (LPVOID)(CurrentChunk + OffsetFromBuffer);
			}
			else
				CurrentChunk = CurrentChunk + BytesRead;
		}
		return nullptr;
	}

}; DumperPatterns* pDumperPatterns = new DumperPatterns();

class Dumper
{
public:
	DWORD64 cl_entitylist = 0x0;
	DWORD64 dw_local_entity = 0x0;
	DWORD64 dw_view_render = 0x0;
	DWORD64 dw_view_matrix = 0x0;
	DWORD64 dw_global_vars = 0x0;

	DWORD64 glow_enable = 0x0;
	DWORD64 glow_color = 0x0;
	DWORD64 glow_type = 0x0;
public:
	DWORD64 DumpOffsets(DWORD64 Address, int del = 6, int offset = 0)
	{
		auto buffer = 0;
		pProc->NtReadProcMem(m_pMemory->pNTModules->NTGetCurrentProcess(), (LPVOID)Address, &buffer, sizeof(int), NULL);
		if (!buffer)
			return 0;
		return buffer + del + offset;
	}
public:
	NTSTATUS Initialize()
	{
		auto IClientEntityList = pDumperPatterns->FindPattern(__("4C 8D 05 ? ? ? ? 4C 8B 25 ? ? ? ?"));
		this->cl_entitylist = (DWORD64)IClientEntityList;

		auto IClientLocalPlayer = pDumperPatterns->FindPattern(__("48 8B 05 ? ? ? ? 48 0F 44 C7"));
		this->dw_local_entity = (DWORD64)IClientLocalPlayer;

		auto IClientViewRender = pDumperPatterns->FindPattern(__("48 8B 0D ? ? ? ? 48 8B 01 FF 50 40 48 8B 0D ? ? ? ?"));
		this->dw_view_render = (DWORD64)IClientViewRender;

		auto IClientViewMatrix = pDumperPatterns->FindPattern(__("48 8D 1D ? ? ? ? 66 0F 1F 84 00 00 00 00 00 48 8B 01 48 0F BE F7 49 3B C6 0F 85"));
		this->dw_view_matrix = (DWORD64)IClientViewMatrix;

		auto IClientGlobalVars = pDumperPatterns->FindPattern(__("4C 8D 05 ? ? ? ? 48 8D 15 ? ? ? ? 41 FF D1"));
		this->dw_global_vars = (DWORD64)IClientGlobalVars;

		return true;
	}
}; Dumper pDumper;