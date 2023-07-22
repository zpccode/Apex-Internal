#pragma once
#include"../../../modules/modules.hpp"

class KeyInput
{
public:
	BOOL WINAPI GetKeyState(INT KEY_CODE) {
		return SpoofReturn(__safecall(GetAsyncKeyState).get(), KEY_CODE);
	}
	BOOL WINAPI ExitThreadKey(INT KEY_CODE) {
		return FALSE;
	}
}; KeyInput* pKeyInput;