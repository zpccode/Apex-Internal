#pragma once
#include"../../../modules/modules.hpp"

class KeyInput {
private:
	int vk_insert = VK_INSERT;
public:
	__forceinline bool GetKeyState(int key_code)
	{
		typedef BOOL(__stdcall* GetKeyInput)(int);
		GetKeyInput _KeyInput = reinterpret_cast<GetKeyInput>(__safecall(GetAsyncKeyState)(key_code));
		return _KeyInput;
	}

	__forceinline bool isPressed()
	{
		int key_code = 0;
		if (this->GetKeyState(key_code) == true) { return true; }
		return false;
	}
}; KeyInput* pKeyInput = new KeyInput();