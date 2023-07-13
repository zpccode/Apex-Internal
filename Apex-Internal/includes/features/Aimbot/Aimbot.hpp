#pragma once
#include"../variables/variables.hpp"
#include"Prediection/Prediction.hpp"
#include"../../sdk/apex_src/apex_src.hpp"
#include"../../sdk/utils/memory/memory.h"

class CAimbot
{
private:
	NTSTATUS WINAPI SlientAim(CLocalEntity LocalEntity, UserCmd_t* pCmd) { 
		uintptr_t local_entity = LocalEntity.Entity;
		CBaseEntity Entity = CBaseEntity();

		for (int i = 0; i < 200; i++)
		{
			uintptr_t entity = Entity.GetEntity(i);
			if (!entity || entity == LocalEntity.Entity || !LocalEntity.Entity)
				continue;

			uintptr_t is_entity_alive = Entity.isAlive(Classes::CPlayer::m_iHealth);
			if (!is_entity_alive)
				continue;

			uintptr_t local_team = LocalEntity.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
			uintptr_t enemy_team = Entity.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
			if (enemy_team == local_team)
				continue;

			CGlobalVars GlobalVars = *(CGlobalVars*)(offsets_modules::module_base + offsets::global_vars);

		}
		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI SmoothAim(CLocalEntity LocalEntity, UserCmd_t* pCmd) {
		uintptr_t local_entity = LocalEntity.Entity;
		CBaseEntity Entity = CBaseEntity();

		for (int i = 0; i < 200; i++)
		{
			uintptr_t entity = Entity.GetEntity(i);
			if (!entity || entity == LocalEntity.Entity || !LocalEntity.Entity)
				continue;

			uintptr_t is_entity_alive = Entity.isAlive(Classes::CPlayer::m_iHealth);
			if (!is_entity_alive)
				continue;

			uintptr_t local_team = LocalEntity.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
			uintptr_t enemy_team = Entity.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
			if (enemy_team == local_team)
				continue;

			CGlobalVars GlobalVars = *(CGlobalVars*)(offsets_modules::module_base + offsets::global_vars);

			
		}

		return STATUS_SUCCESS;
	}
private:
	NTSTATUS WINAPI InitAimbot(CLocalEntity LocalEntity, UserCmd_t* pCmd)
	{
		if (!aimbot::enable_aimbot)
			return STATUS_ERROR;

		if (aimbot::smooth_aim) { this->SmoothAim(LocalEntity, pCmd); }
		if (aimbot::slient_aim) { this->SlientAim(LocalEntity, pCmd); }
		
		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI NoRecoil(CLocalEntity LocalEnity, UserCmd_t* pCmd)
	{
		if (!aimbot::no_recoil)
			return STATUS_ERROR;
		
		Vector3 oPunch{ 0,0,0 };
		Vector3 AimPunch = LocalEnity.GetAimPunch(Classes::CPlayer::m_vecPunchWeapon_Angle) * (0.500f * 2);

		if (spoof_call::GetKey(VK_LBUTTON)) 
		{
			Vector3 ViewAngles = LocalEnity.GetViewAngles(Classes::CPlayer::m_view_angles);
			Vector3 NewAngle = ViewAngles + oPunch - AimPunch;
			LocalEnity.SetViewAngles(Classes::CPlayer::m_view_angles, NewAngle);
		}
		oPunch = AimPunch;
		return STATUS_SUCCESS;
	}
public:
	NTSTATUS WINAPI Initialize()
	{
		CLocalEntity LocalEntity = CLocalEntity(offsets_modules::module_base, offsets::local_player);
		this->InitAimbot(LocalEntity, nullptr);
		this->NoRecoil(LocalEntity, nullptr);
		return STATUS_SUCCESS;
	}
}; CAimbot* pAimbot = new CAimbot();