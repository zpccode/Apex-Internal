#pragma once
#include"../variables/variables.hpp"
#include"../../sdk/utils/dumper/dumper.h"
#define STATUS_SUCCESS 1
#define STATUS_ERROR 0

class CMisc
{
private:
	NTSTATUS WINAPI UnlockAll() 
	{ 
		auto offset = pDumperPatterns->FindPattern("48 8D 05 ? ? ? ? 48 89 05 ? ? ? ? 48 8D 05 ? ? ? ? ");
		auto convar = pDumperPatterns->FindPattern("48 8D 05 ? ? ? ? 48 89 35 ? ? ? ? ");

		if (offset && convar)
		{
			DWORD64 offset_result = *(DWORD64*)offset;
			DWORD64 convar_result = *(DWORD64*)convar;

			offset_result = 1;
			convar_result = 1;
		}

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