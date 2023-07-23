#pragma once
#include"Client/client.hpp"
#include"../../eac/easyanticheat.hpp"

bool unload = false;
__declspec() NTSTATUS WINAPI FeaturesThread(LPVOID hBuffer)
{
	while (!unload)
	{
		UserCmd_t* pCmd = nullptr;
		CLocalEntity LocalEntity = CLocalEntity(offsets_modules::module_base,
			offsets::local_player);

		static Vector3 ViewAngles = pCmd->m_vecViewAngles;
		static float forward_move = pCmd->m_flForwardmove;
		static float side_move = pCmd->m_flSidemove;

		if (LocalEntity.Entity || LocalEntity.IsAlive(Classes::CPlayer::m_iHealth)) 
		{
			pAimbot->Initialize(LocalEntity, pCmd);
			pVisuals->Initialize(LocalEntity);
			pMisc->Initialize(LocalEntity, pCmd);
		}

	}
	return STATUS_SUCCESS;
}
