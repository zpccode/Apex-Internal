#pragma once
#include"../../../modules/modules.hpp"
#include"../engine/engine.hpp"
#include"../cbasecombatweapons/cbasecombatweapon.hpp"

enum HelmetTypes
{
	HELMSLOT_NONE = 0,
	HELMSLOT_GREY = 1,
	HELMSLOT_BLUE = 2,
	HELMSLOT_PURPLE = 3,
	HELMSLOT_GOLDEN = 4,
};
enum ArmorTypes
{
	ARMORSLOT_NONE = 0,
	ARMORSLOT_GREY = 1,
	ARMORSLOT_BLUE = 2,
	ARMORSLOT_PURPLE = 3,
	ARMORSLOT_GOLDEN = 4,
	ARMORSLOT_MYTHIC = 5
};

struct Bone {
	BYTE thing[0xCC];
	float x;
	BYTE thing2[0xC];
	float y;
	BYTE thing3[0xC];
	float z;
};

inline double GetCrossDist(double x1, double y1, double x2, double y2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

class CBaseEntity {
private:
	DWORD64 EntityList = NULL;
	DWORD64 BaseEntity = NULL;
	DWORD64 EntityHandle = NULL;
	
	int Index = 0;
	std::string ident = "";
	
	DWORD64 atIndex(int index) 
	{
		this->Index = index;
		return *(DWORD64*)(this->EntityList + (index << 5));
	}

#define NUM_ENT_ENTRIES		(1 << 12)
#define ENT_ENTRY_MASK		(NUM_ENT_ENTRIES - 1)

public:
	DWORD64 Entity;
	LPCSTR IdentifierC = ident.c_str();
	GlowMode GlowStyle = { 101, 101, 46, 90 };

	uintptr_t GetEntity(int index)
	{
		uintptr_t EntityListEx = *(uintptr_t*)(offsets_modules::module_base + offsets::cl_entitylist);
		uintptr_t BaseEntityEx = *(DWORD64*)(EntityListEx);
		return (uintptr_t)(EntityListEx + (index << 5));
	}

public:
	__declspec() bool IsNPC() {
		return (bool)(strcmp(this->IdentifierC, "npc_dummie"));
	}
	__declspec() bool isPlayer() {
		return (bool)(strcmp(this->IdentifierC, "player"));
	}
	__declspec() bool isDrone() {
		return !this->isPlayer() && !this->IsNPC() ? true : false;
	}
	__declspec() bool isAlive(uintptr_t offset) {
		return *(int*)(this->Entity + offset) != 0;
	}
public:
	inline std::string identifier() {
		return *(std::string*)(this->Entity + 0x589);
	}
public:
	__declspec() int m_iHealth(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__declspec() int m_iShieldHealth(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__declspec() int m_iTeamNum(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__declspec() int m_iBleedoutState(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
public:
	__inline int m_iArmorType(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__inline int m_iHelmetType(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
public:
	__inline bool IsVisible(float current_time, uintptr_t offset) {
		float last_visible_time = *(float*)(Entity + offset);
		return last_visible_time > 0.0f && fabsf(last_visible_time - current_time) < 0.1f;
	}
public:
	Vector3 Postition(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
public:
	__declspec() void EnableGlow(uintptr_t offset, uintptr_t offset_two, uintptr_t offset_three,
		float r, float g, float b, float a)
	{
		ColorRGB Clr = { r * a / 255, g * a / 255, b * a / 255 };
		*(GlowMode*)(this->Entity + offset) = GlowStyle;
		*(ColorRGB*)(this->Entity + offset_two) = Clr;
		*(int*)(this->Entity + offset_three) = 1;
	}
};

class CLocalEntity {
public:
	uintptr_t Entity = 0;
	explicit CLocalEntity(uintptr_t dwbase, uintptr_t offset) {
		Entity = (uintptr_t)(dwbase + offset);
	}
public:
	__declspec() Vector3 Position(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
	__declspec() Vector3 GetViewAngles(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
	__declspec() void SetViewAngles(uintptr_t offset, Vector3 Angles) {
		*(Vector3*)(Entity + offset) = Angles;
	}
	 Vector3 GetAimPunch(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
	__declspec() Vector3 GetBreathAngles(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
	__declspec() Vector3 GetRecoil(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
	__declspec() Vector3 GetCamPostition(uintptr_t offset) {
		return *(Vector3*)(this->Entity + offset);
	}
	__declspec() Vector3 BonePosition(int boneIndex, uintptr_t view_offset, uintptr_t offset) {
		Bone bone = {};
		Vector3 vec_bone = Vector3();
		Vector3 pos = Position(view_offset);

		ULONG64 bone_array = *(ULONG64*)(this->Entity + offset);
		ULONG64 bone_location = (boneIndex * 0x30);

		bone = *(Bone*)(bone_array + bone_location);
		vec_bone.x = bone.x + pos.x;
		vec_bone.y = bone.y + pos.y;
		vec_bone.z = bone.z + pos.z;
		return vec_bone;
	}
public:
	__declspec() int m_iHealth(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__declspec() int m_shieldHealth(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__declspec() int m_iTeamNum(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
	__declspec() int m_armorType(uintptr_t offset) {
		if (!this->Entity) return -1;
		return *(int*)(this->Entity + offset);
	}
	_declspec() int m_helmetType(uintptr_t offset) {
		if (!this->Entity) return -1;
		return *(int*)(this->Entity + offset);
	}
	__declspec() bool IsAlive(uintptr_t offset) {
		return (bool)(this->Entity + offset) != 0;
	}
public:
	__declspec() unsigned long ModelNamePtr() {
		if (!this->Entity) return -1;
		return *(unsigned long long*)(this->Entity + 0x30);
	}
	__declspec() char ModelName() {
		char module_name;
		module_name = *(char*)(this->ModelNamePtr());
		return module_name;
	}
	__declspec() int m_nModelIndex(uintptr_t offset) {
		return *(int*)(this->Entity + offset);
	}
public:
	CBaseCombatWeapon ActiveWeapon(uintptr_t module_base, uintptr_t offset, uintptr_t pointer_offset)
	{
		uintptr_t pWeapon = *(uintptr_t*)(this->Entity + offset);
		pWeapon &= 0xffff;
		uintptr_t pWeaponHandle = *(uintptr_t*)(module_base + pointer_offset + (pWeapon << 5));
		return CBaseCombatWeapon(pWeaponHandle);
	}
};
