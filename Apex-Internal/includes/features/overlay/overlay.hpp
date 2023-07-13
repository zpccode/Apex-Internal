#pragma once
#include"DX11/DX11.hpp"
#include"window/window.hpp"
#include"key_helper/key_helper.hpp"
#include"Tabs/tabs.hpp"
static int items = 0;

class Overlay
{
public:
	bool init_imgui = false;
	bool render_overlay = false;
	bool capture_cursor = false;
	bool render_cursor = false;
public:
	DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	bool close_button = true;
public:

	enum class MenuTabs
	{
		Aimbot,
		Triggerbot,
		Visuals,
		Binds,
		Misc
	};

	MenuTabs CurrentTab = MenuTabs::Aimbot;

public:
	__forceinline void InitImGui() 
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(pD3D11->hWindow);
		ImGui_ImplDX11_Init(pD3D11->pDevice, pD3D11->pContext);
	}

	__forceinline void InitStyle() 
	{
		pInitWindow->InitializeWindow();
		pInitStyle->InitializeStyle();
		pInitStyle->InitializeFonts();
	}

	__declspec() void Tabs(MenuTabs Tab)
	{
		switch (Tab)
		{
		case MenuTabs::Aimbot:
			pTabs->aimbot_tab();
			break;
		case MenuTabs::Triggerbot:
			pTabs->triggerbot_tab();
			break;
		case MenuTabs::Visuals:
			pTabs->visuals_tab();
			break;
		case MenuTabs::Misc:
			pTabs->misc_tab();
			break;
		}
	}

	std::string icon_text(std::string icon, std::string text)
	{
		auto add_strings = icon + text;
		return add_strings;
	}

	__forceinline void MenuTab()
	{
		// skCrypt(%d-%m-%Y | %X).decrypt();
		int height = 40;
		int width = 100;

		pImGuiFeatures->Columns(2);
		pImGuiFeatures->SetColumns(1, width + 30);

		static MenuTabs Tab = MenuTabs::Aimbot;
		{
			static ImVec4 InActive = ImVec4(0.09f, 0.09, 0.09f, 0.88f);
			static ImVec4 Active = ImVec4(0.0f, 0.5f, 0.5f, 0.9f);
			
			pImGuiFeatures->Spacing3();
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
			pImGuiFeatures->PushFont(pInitStyle->IconFont);
		
			pImGuiFeatures->PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Aimbot ? Active : InActive);
			if (pImGuiFeatures->Button(this->icon_text(ICON_MD_SETTINGS, skCrypt(" Aimbot").decrypt()).c_str(), ImVec2(width, height)))
				Tab = MenuTabs::Aimbot;

			pImGuiFeatures->Spacing1();

			pImGuiFeatures->PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Triggerbot ? Active : InActive);
			if (pImGuiFeatures->Button(this->icon_text(ICON_MD_ZOOM_IN, skCrypt(" Trigger").decrypt()).c_str(), ImVec2(width, height)))
				Tab = MenuTabs::Triggerbot;

			pImGuiFeatures->Spacing1();

			pImGuiFeatures->PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Visuals ? Active : InActive);
			if (pImGuiFeatures->Button(this->icon_text(ICON_MD_BRUSH, skCrypt(" Visuals").decrypt()).c_str(), ImVec2(width, height)))
				Tab = MenuTabs::Visuals;

			pImGuiFeatures->Spacing1();

			pImGuiFeatures->PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Misc ? Active : InActive);
			if (pImGuiFeatures->Button(this->icon_text(ICON_MD_KEYBOARD, skCrypt(" Misc  ").decrypt()).c_str(), ImVec2(width, height)))
				Tab = MenuTabs::Misc;

			pImGuiFeatures->PopFont();
			pImGuiFeatures->PopStyleColor(4);
			pImGuiFeatures->PopStyleVar(1);
		}

		pImGuiFeatures->NextColumns();
		{
			this->Tabs(Tab);
		}
	}

	__forceinline void RenderOverlay()
	{
		pImGuiWindow->ProcessBackground();
		
		if (pImGuiWindow->BeginWindow(skCrypt("Synchronize | Solutions").decrypt(), &this->close_button, this->window_flags)) 
		{
			pImGuiWindow->SetSize(pInitWindow->WindowSize);
			this->MenuTab();
			pImGuiWindow->EndWindow();
		}
	}
}; Overlay* pOverlay = new Overlay();