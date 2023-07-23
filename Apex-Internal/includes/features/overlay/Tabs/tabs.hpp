#pragma once
#include"../Extra/extra.hpp"
#include"../../visuals/visuals.hpp"

class Frames
{
public:
	__declspec() void main_aimbot_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);
		
		ImGui::SetCursorPosX(115);
		ImGui::Text(skCrypt("Aimbot Settings").decrypt());
		pImGuiFeatures->Separator();

		pImGuiFeatures->Spacing1();
		// aim options
		pImGuiFeatures->CheckBox(skCrypt("Enable Aimbot").decrypt(), &aimbot::enable_aimbot);
		pImGuiFeatures->Spacing1();
		
		pImGuiFeatures->CheckBox(skCrypt("Draw Fov").decrypt(), &aimbot::draw_aim_fov);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Smooth Aim").decrypt(), &aimbot::smooth_aim);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Slient Aim").decrypt(), &aimbot::slient_aim);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Enable NPCS").decrypt(), &aimbot::npcs);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("No Recoil").decrypt(), &aimbot::no_recoil);
		pImGuiFeatures->Spacing1();

		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
	__declspec() void second_aimbot_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(105);
		ImGui::Text(skCrypt("Aimbot Misc Settings").decrypt());
		pImGuiFeatures->Separator();

		pImGuiFeatures->Spacing1();

		ImGui::SliderFloat(skCrypt("Aim Fov").decrypt(), &aimbot::aim_fov, 1, 360, skCrypt("%.0f").decrypt());
		pImGuiFeatures->Spacing1();

		ImGui::SliderFloat(skCrypt("Smooth Value").decrypt(), &aimbot::smooth_value, 1, 10, skCrypt("%.0f").decrypt());
		
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->ComboBox(skCrypt("Player Hitboxes").decrypt(), &aimbot::hitboxes::player_hitbox_items, aimbot::hitboxes::player_hitbox_chars, IM_ARRAYSIZE(aimbot::hitboxes::player_hitbox_chars));
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->ComboBox(skCrypt("NPC Hitboxes").decrypt(), &aimbot::hitboxes::npcs_hitbox_items, aimbot::hitboxes::npcs_hitbox_chars, IM_ARRAYSIZE(aimbot::hitboxes::npcs_hitbox_chars));
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->ComboBox(skCrypt("Aim Type").decrypt(), &aimbot::aim_type::aim_items, aimbot::aim_type::aim_chars, IM_ARRAYSIZE(aimbot::aim_type::aim_chars));

		switch (aimbot::hitboxes::player_hitbox_items)
		{
		case 0:
			aimbot::hitbox_set::head_enabled = true;
			break;
		case 1:
			aimbot::hitbox_set::head_enabled = false;
			aimbot::hitbox_set::neck_enabled = true;
			break;
		case 2:
			aimbot::hitbox_set::head_enabled = false;
			aimbot::hitbox_set::neck_enabled = false;
			aimbot::hitbox_set::body_enabled = true;
			break;
		case 3:
			aimbot::hitbox_set::head_enabled = false;
			aimbot::hitbox_set::neck_enabled = false;
			aimbot::hitbox_set::body_enabled = false;
			aimbot::hitbox_set::legs_enabled = true;
			break;
		case 4:
			aimbot::hitbox_set::head_enabled = false;
			aimbot::hitbox_set::neck_enabled = false;
			aimbot::hitbox_set::body_enabled = false;
			aimbot::hitbox_set::legs_enabled = false;
			aimbot::hitbox_set::feet_enabled = true;
		}

		switch (aimbot::aim_type::aim_items)
		{
		case 0:
			aimbot::fov_closest = true;
			break;
		case 1:
			aimbot::fov_closest = false;
			aimbot::fov_aim = true;
			break;
		case 2:
			aimbot::fov_aim = false;
			aimbot::fov_closest = false;
			aimbot::cursor = true;
			break;
		}

		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
public:
	__declspec() void main_trigger_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(115);
		ImGui::Text(skCrypt("Triggerbot Settings").decrypt());
		pImGuiFeatures->Separator();


		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
	__declspec() void second_trigger_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(105);
		ImGui::Text(skCrypt("Triggerbot Misc Settings").decrypt());
		pImGuiFeatures->Separator();

		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
public:
	__declspec() void main_visuals_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(115);
		ImGui::Text(skCrypt("Visual Settings").decrypt());
		pImGuiFeatures->Separator();
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox("Enable Visuals", &visuals::enable_visuals);
		//pImGuiFeatures->CheckBox(skCrypt("Third Person").decrypt(), &visuals::);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Enable Radar").decrypt(), &visuals::render_radar_option);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Enable Outline").decrypt(), &visuals::enable_outline);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Enable Item Esp").decrypt(), &visuals::item_esp);
		pImGuiFeatures->Spacing1();
		
		pImGuiFeatures->CheckBox(skCrypt("Enable Weapon Chams").decrypt(), &visuals::weapon_chams);
		pImGuiFeatures->Spacing1();

		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
	__declspec() void second_visuals_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(105);
		ImGui::Text(skCrypt("Visual Misc Settings").decrypt());
		pImGuiFeatures->Separator();

		pImGuiFeatures->CheckBox(skCrypt("SkinChanger").decrypt(), &visuals::skin_roller);
		pImGuiFeatures->Spacing1();

		pImGuiFeatures->CheckBox(skCrypt("Weapon Skin Changer").decrypt(), &visuals::init_skin_changer);
		pImGuiFeatures->Spacing1();
		ImGui::SliderInt(skCrypt("Weapon Skin Id").decrypt(), &visuals::weapon_skin_id, 0, 100, "%.0f");
		pImGuiFeatures->Spacing1();

		ImGui::SliderFloat(skCrypt("View Fov").decrypt(), &visuals::view_fov, 75, 180, "%.0f");
		pImGuiFeatures->Spacing1();

		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
public:
	__declspec() void main_misc_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(125);
		ImGui::Text(skCrypt("Misc Settings").decrypt());
		pImGuiFeatures->Separator();

		pImGuiFeatures->CheckBox(__("Unlock All"), &misc::unlock_all);


		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
	__declspec() void second_misc_frame(const char* id, const ImVec2& size = ImVec2(0, 0), bool border = false)
	{
		pImGuiFeatures->PushStyleColor(ImGuiCol_Border, ImColor(53, 53, 53, 255).Value);
		pImGuiChildWindow->BeginChildFrame(id, size, border);

		ImGui::SetCursorPosX(115);
		ImGui::Text(skCrypt("Misc Extra Settings").decrypt());
		pImGuiFeatures->Separator();

		pImGuiFeatures->CheckBox(__("Fps Window"), &misc::render_frame_window);

		pImGuiChildWindow->EndChildFrame();
		pImGuiFeatures->PopStyleColor(1);
	}
}; Frames* pFrames;

class tabs
{
public:
	__declspec() void aimbot_tab() 
	{
		pImGuiFeatures->PushFont(pInitStyle->VerdanaSmall);
		pFrames->main_aimbot_frame(skCrypt("##Aimbot").decrypt(), ImVec2(318, 409), true);
		ImGui::SameLine();
		ImGui::SetCursorPosX(462);
		pFrames->second_aimbot_frame(skCrypt("##AimbotMisc").decrypt(), ImVec2(318, 409), true);
		pImGuiFeatures->PopFont();
	}
	__declspec() void triggerbot_tab() 
	{
		pImGuiFeatures->PushFont(pInitStyle->VerdanaSmall);
		pFrames->main_trigger_frame(skCrypt("##Trigger").decrypt(), ImVec2(318, 409), true);
		ImGui::SameLine();
		ImGui::SetCursorPosX(462);
		pFrames->second_trigger_frame(skCrypt("##TriggerMisc").decrypt(), ImVec2(318, 409), true);
		pImGuiFeatures->PopFont();
	}
public:
	__declspec() void visuals_tab() 
	{
		pImGuiFeatures->PushFont(pInitStyle->VerdanaSmall);
		pFrames->main_visuals_frame(skCrypt("##Visuals").decrypt(), ImVec2(318, 409), true);
		ImGui::SameLine();
		ImGui::SetCursorPosX(462);
		pFrames->second_visuals_frame(skCrypt("##VisualsMisc").decrypt(), ImVec2(318, 409), true);
		pImGuiFeatures->PopFont();
	}
	__declspec() void misc_tab() 
	{
		pImGuiFeatures->PushFont(pInitStyle->VerdanaSmall);
		pFrames->main_misc_frame(skCrypt("##Misc").decrypt(), ImVec2(318, 409), true);
		ImGui::SameLine();
		ImGui::SetCursorPosX(462);
		pFrames->second_misc_frame(skCrypt("##MiscMisc").decrypt(), ImVec2(318, 409), true);
		pImGuiFeatures->PopFont();
	}
}; tabs* pTabs = new tabs();
