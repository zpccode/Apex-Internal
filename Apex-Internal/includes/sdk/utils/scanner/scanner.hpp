#pragma once
#include"../../../modules/modules.hpp"
#include <Psapi.h>

#define INRANGE(x,a,b)	(x >= a && x <= b)
#define GetBits(x)		(INRANGE((x & (~0x20)),'A','F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetBytes(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

class find_pattern
{
public:
	unsigned int LocatePattern(const DWORD dwAddress, const DWORD dwLen, const char* const szPattern)
	{
		auto szPatt = szPattern;
		DWORD dwFirstMatch = 0x0;

        DWORD dwCur;
        for (dwCur = dwAddress; dwCur < dwLen; dwCur++)
        {
            if (!szPatt)
                return dwFirstMatch;

            const auto& pCurByte = *(BYTE*)dwCur;
            const auto& pBytePatt = *(BYTE*)szPatt;

            if (pBytePatt == '\?' || pCurByte == GetBytes(szPatt))
            {
                if (!dwFirstMatch)
                    dwFirstMatch = dwCur;

                if (!szPatt[2])
                    return dwFirstMatch;

                szPatt += (pBytePatt == '\?\?' || pBytePatt != '\?') ? 3 : 2;
            }
            else
            {
                szPatt = szPattern;
                dwFirstMatch = 0x0;
            }
        }

        return 0x0;
	}
public:
	unsigned int FindPattern(const char* module, const char* pattern) 
	{
		if (const auto dwModule = reinterpret_cast<DWORD>(spoof_call::GetModuleBase(module))) {
			if (const auto& nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(dwModule + reinterpret_cast<PIMAGE_DOS_HEADER>(dwModule)->e_lfanew))
				return LocatePattern(dwModule + nt_headers->OptionalHeader.BaseOfCode, dwModule + nt_headers->OptionalHeader.SizeOfCode, pattern);
		}
	}
}; find_pattern pPattern;

class scanner
{
public:
	inline std::uint8_t* PatternScan(void* rawImage, const char* signature)
	{
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector < int >{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + spoof_call::StrLen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = (PIMAGE_DOS_HEADER)rawImage;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)rawImage + dosHeader->e_lfanew);

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);
        auto scanBytes = reinterpret_cast<uint8_t*>(rawImage);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i) {
            bool found = true;
            for (auto j = 0ul; j < s; ++j) {
                if (scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return &scanBytes[i];
            }
        }
        return nullptr;
	}

    inline uintptr_t occurence(const char* module, const char* pattern) 
    {
        #define in_range(x, a, b) (x >= a && x <= b)
        #define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
        #define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

        MODULEINFO Module;
       spoof_call::NtGetModuleInformation(spoof_call::NTGetCurrentProcess(), spoof_call::GetModuleBase(module), &Module, sizeof(MODULEINFO));
        
        uintptr_t start = (uintptr_t)Module.lpBaseOfDll;
        uintptr_t end = (uintptr_t)Module.lpBaseOfDll + (uintptr_t)Module.SizeOfImage;
        uintptr_t match = 0;

        const char* current = pattern;
        for (uintptr_t pCur = start; pCur < end; pCur++) {

            if (!*current)
                return match;

            if (*(PBYTE)current == ('\?') || *(BYTE*)pCur == get_byte(current)) {
                if (!match)
                    match = pCur;

                if (!current[2])
                    return match;

                if (*(PWORD)current == ('\?\?') || *(PBYTE)current != ('\?'))
                    current += 3;
                else
                    current += 2;
            }
            else {
                current = pattern;
                match = 0;
            }
        }
        return 0;
    }
    inline uintptr_t dereference(uintptr_t addr, unsigned int offset) 
    {
        if (addr == 0) return 0;
        if (sizeof(uintptr_t) == 8)
            return addr + (int)((*(int*)(addr + offset) + offset) + sizeof(int));
        return (uintptr_t) * (unsigned long*)(addr + offset);
    }
}; scanner pScanner;

