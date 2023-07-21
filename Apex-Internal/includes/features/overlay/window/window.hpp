#pragma once
#include"../ImGui/imgui.h"
#include"../ImGui/imgui_impl_dx11.h"
#include"../ImGui/imgui_impl_win32.h"
#include"../ImGui/imgui_internal.h"
#include"fonts/icons_data.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	ImVec2 TitlePos = ImVec2(0.5, 0.5);
	ImVec2 WindowSize = ImVec2(788, 425);
public:
	float WindowRounding = 4.0f;
	float ChildRounding = 4.0f;
public:
	__forceinline void InitializeWindow()
	{
		ImGuiStyle* pStyle = &ImGui::GetStyle();
		pStyle->WindowTitleAlign = this->TitlePos;
		pStyle->WindowRounding = this->WindowRounding;
	}
}; Window* pInitWindow = new Window();

ImU32 GetU32(int r, int b, int g, int a)
{
	return ((a & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
		+ (r & 0xff);
}

class WindowStyle
{
public:
	ImFont* TitleFont = nullptr;
	ImFont* SectionFont = nullptr;
	ImFont* VerdanaSmall = nullptr;
	ImFont* IconFont = nullptr;
public:
	ImColor WindowBackground = ImColor(0,0,0,250);
	
	ImColor ChildBackground = ImColor(0,0,0, 240);
	ImColor WindowTitleBar = ImColor(0,0,0,250);
	ImColor WindowSeparator = ImColor(68,0,255,255);
	
	ImColor WindowBorder = ImColor(68, 0, 255, 255);
	ImColor WindowButtonActive = ImColor(85, 0, 255, 255);
	ImColor WindowButtonHovered = ImColor(85, 0, 255, 255);

	ImColor WindowSlider = ImColor(68, 0, 255, 255);
	ImColor WindowSliderGrab = ImColor(85, 0, 255, 255);

	ImColor WindowFrameBackground = ImColor(108, 106, 106, 255);
	ImColor WindowFrameHover = ImColor(85, 0, 255, 255);
	ImColor WindowFrameActive = ImColor(85, 0, 255, 255);

	ImColor WindowFrameExtra = ImColor(85, 0, 255, 255);
public:
	__forceinline void InitializeStyle()
	{
		ImGuiStyle* pStyle = &ImGui::GetStyle();
		pStyle->Colors[ImGuiCol_WindowBg] = this->WindowBackground;
		pStyle->Colors[ImGuiCol_TitleBg] = this->WindowTitleBar;

		pStyle->Colors[ImGuiCol_TitleBgActive] = this->WindowTitleBar;
		pStyle->Colors[ImGuiCol_Separator] = this->WindowSeparator;
		pStyle->Colors[ImGuiCol_Border] = this->WindowBorder;

		pStyle->Colors[ImGuiCol_Button] = this->WindowButtonActive;
		pStyle->Colors[ImGuiCol_ButtonActive] = this->WindowButtonActive;
		pStyle->Colors[ImGuiCol_ButtonHovered] = this->WindowButtonHovered;
		pStyle->Colors[ImGuiCol_ChildBg] = this->ChildBackground;

		pStyle->Colors[ImGuiCol_SliderGrab] = this->WindowSlider;
		pStyle->Colors[ImGuiCol_SliderGrabActive] = this->WindowSliderGrab;
		pStyle->Colors[ImGuiCol_FrameBg] = this->WindowFrameBackground;

		pStyle->Colors[ImGuiCol_FrameBgHovered] = this->WindowFrameHover;
		pStyle->Colors[ImGuiCol_FrameBgActive] = this->WindowFrameActive;
		
		pStyle->Colors[ImGuiCol_HeaderHovered] = this->WindowFrameExtra;
		pStyle->Colors[ImGuiCol_Header] = this->WindowFrameExtra;
		pStyle->Colors[ImGuiCol_HeaderActive] = this->WindowFrameExtra;

		pStyle->Colors[ImGuiCol_DragDropTarget] = this->WindowFrameExtra;
		pStyle->Colors[ImGuiCol_MenuBarBg] = this->WindowFrameExtra;
		pStyle->Colors[ImGuiCol_PopupBg] = this->WindowBackground;
	}
	__declspec() void InitializeFonts()
	{
		{
			const auto& io = ImGui::GetIO();
			auto fontConfig = ImFontConfig();
			fontConfig.OversampleH = 2;
			constexpr ImWchar fontRange[]{ 0x0020, 0x00FF,0x0400, 0x044F, 0 };
			this->VerdanaSmall = io.Fonts->AddFontFromFileTTF(skCrypt("C:\\Windows\\Fonts\\verdana.ttf").decrypt(), 12.0f, &fontConfig, fontRange);
			this->TitleFont = io.Fonts->AddFontFromFileTTF(skCrypt("C:\\Windows\\Fonts\\verdanab.ttf").decrypt(), 20.0f, &fontConfig, fontRange);

			constexpr ImWchar iconRange[]{ ICON_MIN_MD, ICON_MAX_MD, 0 };
			ImFontConfig iconConfig;
			iconConfig.MergeMode = true;
			iconConfig.PixelSnapH = true;
			IconFont = io.Fonts->AddFontFromMemoryCompressedTTF(MaterialFont_compressed_data, MaterialFont_compressed_size, 12.f, &iconConfig, iconRange);
			
			io.Fonts->Build();
		}
	}
}; WindowStyle* pInitStyle = new WindowStyle();