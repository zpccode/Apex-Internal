#pragma once
#include"../variables/variables.hpp"
#define STATUS_SUCCESS 1
#define STATUS_ERROR 0

class CMisc
{
private:
	NTSTATUS WINAPI UnlockAll() 
	{ 
		DWORD64 Offset = (DWORD64)pScanner.PatternScan((LPVOID)offsets_modules::module_base, 
			"48 8D 05 ? ? ? ? 48 89 05 ? ? ? ? 48 8D 05 ? ? ? ? ") + 17;
		DWORD Convar = (DWORD64)pScanner.PatternScan((LPVOID)offsets_modules::module_base,
			"48 8D 05 ? ? ? ? 48 89 35 ? ? ? ? ") + 10;
		*(DWORD64*)(Offset) = 1;
		*(DWORD64*)(Convar) = 1;
		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI FreeCam(CLocalEntity LocalEntity, UserCmd_t* pCmd)
	{
		static Vector3 ViewAngle = pCmd->m_vecViewAngles;
		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI TapStrafe(CLocalEntity LocalEntity, UserCmd_t* pCmd)
	{
		if (pCmd->m_flSidemove && pKeyInput->GetKeyState(VK_SPACE))
		{
			pCmd->m_flForwardmove += 1;
		}
		else if (pCmd->m_flForwardmove && !pKeyInput->GetKeyState(VK_SPACE))
		{
			pCmd->m_flForwardmove -= 1;
		}
		else if (pCmd->m_flForwardmove && pKeyInput->GetKeyState(VK_SPACE))
		{
			pCmd->m_flSidemove += 1;
			pCmd->m_flForwardmove += 1;
		}
		else
		{
			pCmd->m_flForwardmove -= 1;
			pCmd->m_flSidemove -= 1;
		}
		return STATUS_SUCCESS;
	}
public:
	NTSTATUS WINAPI Initialize(CLocalEntity LocalEntity, UserCmd_t* pCmd)
	{
		if (misc::unlock_all) { this->UnlockAll(); }
		if (misc::free_cam) { this->FreeCam(LocalEntity, pCmd); }
		if (misc::tap_strafe) { this->TapStrafe(LocalEntity, pCmd); }
		return STATUS_SUCCESS;
	}
}; CMisc* pMisc = new CMisc();