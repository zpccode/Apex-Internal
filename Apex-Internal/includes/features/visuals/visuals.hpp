#pragma once
#include"../Aimbot/Aimbot.hpp"
#include"../../sdk/utils/utils.hpp"

class VisualFeatures
{
public:
	NTSTATUS WINAPI ThirdPerson(CLocalEntity LocalEntity) 
	{ 
		static bool enabled = false;
		
		if (visuals::enable_thirdperson)
		{
			enabled = true;
			*(int*)(offsets_modules::module_base + offsets::thirdperson_override) = 1;
			*(int*)(LocalEntity.Entity + offsets::m_thirdPersonShoulderView) = 1;
		}
		else
		{
			if (enabled) {
				enabled = false; *(int*)(offsets_modules::module_base + offsets::thirdperson_override) = 0;
				*(int*)(LocalEntity.Entity + offsets::m_thirdPersonShoulderView) = 0;
			}
		}
		return STATUS_SUCCESS;
	}
	
	NTSTATUS WINAPI PlayerBox(CLocalEntity LocalEntity) { return STATUS_SUCCESS; }
	
	NTSTATUS WINAPI PlayerName(CLocalEntity LocalEntity) { return STATUS_SUCCESS; }
	
	NTSTATUS WINAPI PlayerOutline(CLocalEntity LocalEntity) 
	{ 
		if (!visuals::enable_outline)
			return STATUS_ERROR;

		uintptr_t local_team = LocalEntity.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
		CBaseEntity BaseEntity = CBaseEntity();
		
		for (int i = 0; i < 100; i++)
		{
			uintptr_t entity_from_index = BaseEntity.GetEntity(i);
			
			if (!entity_from_index || entity_from_index == LocalEntity.Entity || !LocalEntity.Entity)
				continue;
			
			BaseEntity.EnableGlow(offsets::glow_type, offsets::glow_color, offsets::glow_enable,
				255, 255, 255, 255);
		}

		return STATUS_SUCCESS; 
	}
}; VisualFeatures* pVisualFeatures = new VisualFeatures();

class Visuals {
public:
	NTSTATUS WINAPI RenderRadar()
	{
		if (pImGuiWindow->BeginWindow(skCrypt("Radar").decrypt(), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
		{
			ImGui::SetWindowSize(ImVec2(219, 213));

			ImVec2 siz = ImGui::GetWindowSize();
			ImVec2 pos = ImGui::GetWindowPos();

			ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
			windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y + 0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), GetU32(255, 255, 255, 255), 1.5f);
			windowDrawList->AddLine(ImVec2(pos.x + 0, pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), GetU32(255, 255, 255, 255), 1.5f);

			CLocalEntity LocalEnt = CLocalEntity(offsets_modules::module_base, offsets::local_player);
			CBaseEntity BaseEnt = CBaseEntity();

			if (!LocalEnt.Entity)
				return STATUS_ERROR;;

			for (int i = 0; i < 200; i++)
			{
				uintptr_t entity = BaseEnt.GetEntity(i);
				
				if (!entity || entity == LocalEnt.Entity)
					continue;

				uintptr_t local_team = LocalEnt.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
				uintptr_t enemy_team = BaseEnt.m_iTeamNum(Classes::CBaseEntity::m_iTeamNum);
			
				if (enemy_team == local_team)
					continue;

				uintptr_t isEntityAlive = BaseEnt.isAlive(Classes::CPlayer::m_iHealth);
				uintptr_t isLocalEntityAlive = LocalEnt.IsAlive(Classes::CPlayer::m_iHealth);

				if (!isEntityAlive || !isLocalEntityAlive)
					continue;

				// put code here
			}

			pImGuiWindow->EndWindow();
		}

		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI RenderFPSWindow()
	{
		pImGuiWindow->BeginWindow(skCrypt("FPS").decrypt(), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(108, 24));
		ImGui::Text(skCrypt("FPS: %f").decrypt(), ImGui::GetIO().Framerate);
		pImGuiWindow->EndWindow();
		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI InitVisuals(CLocalEntity LocalEntity)
	{
		if (!visuals::enable_visuals)
			return STATUS_ERROR;

		if (visuals::enable_box) { pVisualFeatures->PlayerBox(LocalEntity); }
		if (visuals::enable_thirdperson) { pVisualFeatures->ThirdPerson(LocalEntity); }
		if (visuals::enable_outline) { pVisualFeatures->PlayerOutline(LocalEntity); }
		
		return STATUS_SUCCESS;
	}

public:	
	NTSTATUS WINAPI Initialize()
	{
		CLocalEntity LocalEntity = CLocalEntity(offsets_modules::module_base, offsets::local_player);
		this->InitVisuals(LocalEntity);
		return STATUS_SUCCESS;
	}
}; Visuals* pVisuals;