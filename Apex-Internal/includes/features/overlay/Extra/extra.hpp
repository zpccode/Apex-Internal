#include"../ImGui/imgui.h"
#include<Windows.h>

class ImGuiExtra
{
public:
	__forceinline void CheckBox(LPCSTR szFeature, bool* toggle) { ImGui::Checkbox(szFeature, toggle); }
	__forceinline void Slider(LPCSTR szName, INT* toggle, INT min, INT max, LPCSTR format = "%d") { ImGui::SliderInt(szName, toggle, min, max, format); }
	__forceinline bool FloatSlider(LPCSTR szName, FLOAT* toggle, FLOAT min, FLOAT max, LPCSTR format = "%i", FLOAT power = (1.0f)) {
		return ImGui::SliderFloat(szName, toggle, min, max, format, power);
	}
	__forceinline void WindowSize(ImVec2 Size) { ImGui::SetWindowSize(Size, ImGuiCond_Once); }
public:
	bool ComboBox(LPCSTR label, INT* current_item, LPCSTR const items[], INT items_count) { 
		return ImGui::Combo(label, current_item, items, items_count, 0);
	}
	__declspec() void Columns(int id) { ImGui::Columns(id); }
	__declspec() void SetColumns(int index, float offset) { ImGui::SetColumnOffset(index, offset); }
	__declspec() void NextColumns() { ImGui::NextColumn(); }
public:
	__declspec() void Spacing1() { ImGui::Spacing(); }
	__declspec() void Spacing3() { ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); }
public:
	__declspec() void PushStyleColor(ImGuiCol idx, const ImVec4& clr) { ImGui::PushStyleColor(idx, clr); }
	__declspec() bool Button(LPCSTR name, ImVec2 size) { return ImGui::Button(name, size); }
	__declspec() void PopStyleColor(INT index) { ImGui::PopStyleColor(index); }
public:
	__declspec() void Separator() { ImGui::Separator(); }
public:
	__declspec() void PushStyleVar(ImGuiStyleVar idx, const ImVec2& size) { ImGui::PushStyleVar(idx, size); }
	__declspec() void PopStyleVar(INT index) { ImGui::PopStyleVar(index); }
public:
	__declspec() void IconButton(LPCSTR icon, ImVec2 size);
	__declspec() void PushFont(ImFont* font) { ImGui::PushFont(font); }
	__declspec() void PopFont() { ImGui::PopFont(); }
}; ImGuiExtra* pImGuiFeatures = new ImGuiExtra();

class ImGuiWindowEx
{
public:
	__forceinline void ProcessBackground() {
		auto background = ImGui::GetBackgroundDrawList();
		background->AddRectFilled(ImVec2{ 0,0 }, ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImColor(15, 15, 15, 200));
	}
	__forceinline void SetSize(ImVec2 Size) {
		pImGuiFeatures->WindowSize(Size);
	}
	__forceinline bool BeginWindow(LPCSTR name, bool* open = (bool*)0, ImGuiWindowFlags flags = 0){return ImGui::Begin(name, open, flags);}
	__forceinline void EndWindow() { ImGui::End(); }
}; ImGuiWindowEx* pImGuiWindow = new ImGuiWindowEx();

class ImGuiChildWindow
{
public:
	bool BeginChildFrame(const char* id, const ImVec2 &size = ImVec2(0,0), bool border = false, ImGuiWindowFlags flags = 0) { return ImGui::BeginChild(id, size, border, flags); }
	__declspec() void EndChildFrame() { ImGui::EndChild(); }
}; ImGuiChildWindow* pImGuiChildWindow = new ImGuiChildWindow();

class ImGuiTabs
{
public:
	bool BeginTabBar(LPCSTR str_id, ImGuiTabBarFlags flags = 0) { return ImGui::BeginTabBar(str_id, flags); }
	__declspec() void EndTabBar() { ImGui::EndTabBar(); }
public:
	bool BeginTabItem(LPCSTR name, bool* open = (bool*)0, ImGuiTabItemFlags flags = 0) { return ImGui::BeginTabItem(name, open, flags); }
	__declspec() void EndTabItem() { ImGui::EndTabItem(); }
}; ImGuiTabs* pImGuiTabs = new ImGuiTabs();

#include<time.h>
class ImGuiTime
{
public:
	LPCSTR GetTime(LPCSTR szFormat) {
		time_t now = time(0);
		struct tm tstruct;
		char buf[50];
		tstruct = *localtime(&now);
		__safecall(strftime)(buf, sizeof(buf), szFormat, &tstruct);
		return buf;
	}
}; ImGuiTime* pImGuiTime = new ImGuiTime();

class ImGuiImage
{
public:
	__declspec() void RenderImage(LPCSTR str_id, size_t size, bool* show = (bool*)0) {
		// TODO: Later
	}
};