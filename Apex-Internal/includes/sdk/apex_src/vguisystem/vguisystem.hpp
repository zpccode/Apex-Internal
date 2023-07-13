#pragma once
#include"../../../features/overlay/ImGui/imgui.h"

int ScreenWidth = GetSystemMetrics(SM_CXSCREEN); int screenX = ScreenWidth / 2;
int ScreenHeight = GetSystemMetrics(SM_CYSCREEN); int screenY = ScreenHeight / 2;

class Draw
{
public:
	__declspec() void DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const float aLineWidth) 
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddLine(aPoint1, aPoint2, aColor, aLineWidth);
	}
	
	__declspec() void DrawBox(float x, float y, float W, float H, const ImU32& Color, float thickness) 
	{
		auto vList = ImGui::GetBackgroundDrawList();
		
		float lineW = (W / 3);
		float lineH = (H / 3);

		auto col = ImGui::GetColorU32(Color);
		vList->AddLine(ImVec2(x, y - thickness / 2), ImVec2(x, y + lineH), col, thickness);
		vList->AddLine(ImVec2(x - thickness / 2, y), ImVec2(x + lineW, y), col, thickness);

		vList->AddLine(ImVec2(x + W - lineW, y), ImVec2(x + W + thickness / 2, y), col, thickness);//top right horizontal
		vList->AddLine(ImVec2(x + W, y - thickness / 2), ImVec2(x + W, y + lineH), col, thickness);

		vList->AddLine(ImVec2(x, y + H - lineH), ImVec2(x, y + H + (thickness / 2)), col, thickness);//bot left
		vList->AddLine(ImVec2(x - thickness / 2, y + H), ImVec2(x + lineW, y + H), col, thickness);

		vList->AddLine(ImVec2(x + W - lineW, y + H), ImVec2(x + W + thickness / 2, y + H), col, thickness);//bot right
		vList->AddLine(ImVec2(x + W, y + H - lineH), ImVec2(x + W, y + H + (thickness / 2)), col, thickness);
	}
	
	__declspec() void DrawCircle(float size, ImColor Color) 
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircle(ImVec2(ScreenWidth / 2, ScreenHeight / 2), size, Color, 100, 1.f);
	}
}; Draw* pDraw = new Draw();