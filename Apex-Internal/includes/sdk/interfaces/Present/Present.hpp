#pragma once
#include"../Cursor/Cursor.hpp"
extern "C" { namespace present
{
	__forceinline HRESULT __stdcall present_hk(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!pOverlay->init_imgui)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&pD3D11->pDevice)))
			{
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.WantCaptureMouse || io.WantCaptureKeyboard || io.WantTextInput;

				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				io.IniFilename = NULL;

				pD3D11->pDevice->GetImmediateContext(&pD3D11->pContext);
				DXGI_SWAP_CHAIN_DESC desc;

				pSwapChain->GetDesc(&desc);
				pD3D11->hWindow = desc.OutputWindow;

				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

				D3D11_RENDER_TARGET_VIEW_DESC RenderTarget = {};
				spoof_call::SafeMemSet(&RenderTarget, 0, sizeof(RenderTarget));

				RenderTarget.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				RenderTarget.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

				pD3D11->pDevice->CreateRenderTargetView(pBackBuffer, &RenderTarget, &pD3D11->pRenderTargetView);
				pBackBuffer->Release();

				ImGui::GetIO().ImeWindowHandle = pD3D11->hWindow;
				ImGui_ImplDX11_CreateDeviceObjects();

				pD3D11->oWndProc = (oWndProc_t)spoof_call::NtSetWindowLongPtr(pD3D11->hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc::Detour);
				pOverlay->InitImGui();
				pOverlay->InitStyle();
				pOverlay->init_imgui = true;
			}
			else
				return pD3D11->oPresent(pSwapChain, SyncInterval, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::GetIO().MouseDrawCursor = pOverlay->render_overlay;
		ImGui::GetIO().WantCaptureMouse = pOverlay->render_overlay;

		pOverlay->render_cursor = ImGui::GetIO().WantCaptureMouse;
		pOverlay->capture_cursor = ImGui::GetIO().MouseDrawCursor;

		if (spoof_call::GetKey(VK_INSERT) & 0x1) { pOverlay->render_overlay = !pOverlay->render_overlay; }

		if (pOverlay->render_overlay) { pOverlay->RenderOverlay(); }

		if (aimbot::draw_aim_fov) { pDraw->DrawCircle(aimbot::aim_fov, ImColor(255, 255, 255, 255)); }

		if (visuals::render_radar_option) { pVisuals->RenderRadar(); }

		if (visuals::render_frame_window) { pVisuals->RenderFPSWindow(); }

		if (pOverlay->render_cursor || pOverlay->capture_cursor) { Cursor::init_hook(); }
		else { Cursor::remove_hook(); }

		ImGui::EndFrame();
		ImGui::Render();
		pD3D11->pContext->OMSetRenderTargets(1, &pD3D11->pRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return pD3D11->oPresent(pSwapChain, SyncInterval, Flags);
	}

	NTSTATUS WINAPI hook_present(LPVOID hBuffer)
	{
		bool init_hook = false;
		while (!init_hook)
		{
			if (pDX11->Initialize() == true)
			{
				pInitHook->pCreateHook->make_hook(8, (LPVOID*)&pD3D11->oPresent, present_hk);
				init_hook = true;
			}
		}
		return TRUE;
	}
}}