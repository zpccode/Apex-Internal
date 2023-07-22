#pragma once
#include"../WndProc/WndProc.hpp"

extern "C" {namespace Cursor
{
	BOOL WINAPI cursor_hk(bool open) 
	{
		if (pOverlay->render_overlay)
			return FALSE;
		return pD3D11->oCursor(open);
	}

	BOOL WINAPI show_cursor(int x, int y) 
	{
		return SpoofReturn(__safecall(SetCursorPos).get(), x, y); 
	}

	NTSTATUS WINAPI init_hook()
	{
		if (NTMakeHook::NTCreateHook(&show_cursor, &cursor_hk, reinterpret_cast<LPVOID*>(&pD3D11->oCursor)) != MH_OK)
			return STATUS_ERROR;
		if (NTMakeHook::NTEnableHook(&show_cursor) != MH_OK)
			return STATUS_ERROR;
		return STATUS_SUCCESS;
	}

	NTSTATUS WINAPI remove_hook()
	{
		NTMakeHook::NTDisableHook(&show_cursor);
		NTMakeHook::NTRemoveHook(&show_cursor);
		return STATUS_SUCCESS;
	}
}}