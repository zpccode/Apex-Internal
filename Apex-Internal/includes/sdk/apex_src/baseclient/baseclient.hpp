#pragma once
#include<Windows.h>
#include<cstdint>

enum class SignonState : int32_t {
	None,
	Challenge,
	Connected,
	StateNew,
	Prespawn,
	GettingData,
	Spawn,
	FirstSnap,
	Full,
	ChangeLevel,
};

class ClientClass;
class IBaseClient
{
public:
	virtual bool isConnected(void) = 0;
	virtual bool isInMatch(void) = 0;
	virtual ClientClass* dwGetAllClasses(void) = 0;
public:
	inline SignonState ReturnSignon(uintptr_t base, uintptr_t offset, uintptr_t offset_two) {
		return *(SignonState*)(base + offset + offset_two);
	}
	inline char level_buffer(uintptr_t base, uintptr_t offset, uintptr_t offset_two) {
		return *(char*)(base + offset + offset_two);
	}
	inline bool IsInGame(uintptr_t base, uintptr_t offset, uintptr_t offset_two) {
		SignonState sig_state = {};
		return sig_state == SignonState::Full && level_buffer(base, offset, offset_two) != '\0' && !!strcmp((const char*)level_buffer(base, offset, offset_two), "mp_lobby");
	}
};