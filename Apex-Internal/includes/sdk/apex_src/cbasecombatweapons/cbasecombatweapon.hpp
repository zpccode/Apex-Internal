#pragma once
#include<cstdint>
#include<Windows.h>

enum weapon_idS
{
	MEELE = 18,

	// Sniper Ammo
	SENTINEL = 1,
	CHARGE_RIFLE = 86,
	LONGBOW = 88,

	// Light Ammo
	RE_45 = 85,
	P2020 = 101,
	R301 = 0,
	R99 = 100,
	ALTERNATOR = 84,

	//Heavy Ammo
	CAR_MP = 107,
	RAMPAGE = 14,
	_30_30 = 106,
	WINGMAN = 104,
	PROWLER = 98,
	HEMLOK = 93,
	FLATLINE = 91,

	// Elektro Ammo
	TRIPLE_TAKE = 103,
	DEVOTION = 87,
	HAVOC = 89,
	L_STAR = 95,

	// Pump Ammo
	MOSAMBIK = 97,
	PEACEKEEPER = 99,
	EVA_8_AUTO = 90,
	MASTIFF = 96,

	// BOW
	BOCEK = 2,

	//MYTHIC
	KRABER = 94,
	G7_SCOUT = 92,
	VOLT = 105,
	SPITFIRE = 102
};

struct WeaponSkinIDs
{
	enum WINGMAN
	{
		DEFAULT = 0,
		MIDNIGHT = 1,
		EYE_OF_THE_STORM = 2,
		TRIAL_BY_FIRE = 3,
		CODE_OF_HONOR = 4,
		OFF_THE_GRID = 5,
		BREAK_THE_ENEMY = 6,
		ARTIC_BLASTER = 7,
		ETERNAL_LIFE = 8,
		CRIMSON_KING = 9,
		DRAGONS_WING = 10,
		GLADIATORS_TROPHY = 11,

	};
};

class CBaseCombatWeapon
{
public:
	DWORD64 Handle = 0;
	explicit CBaseCombatWeapon(DWORD64 handle) {
		this->Handle = handle;
	}
public:
	_declspec() int GetWeaponID(uintptr_t offset) {
		if (!this->Handle)
			return -1;
		return *(int*)(this->Handle + offset);
	}
	__declspec() int GetAmmoInClip(uintptr_t offset) {
		if (!this->Handle)
			return -1;
		return *(int*)(this->Handle + offset);
	}
	__declspec() void ChangeSkin(int skin_id, uintptr_t offset, uintptr_t pointer_offset) {
		if (!this->Handle)
			return;
		*(int*)(this->Handle + offset) = skin_id;
		*(int*)(this->Handle + pointer_offset) = skin_id;
	}	
	__declspec() std::string GetWeaponName(uintptr_t offset) {
		auto weapon_id = this->GetWeaponID(offset);
	
		if (!weapon_id)
			return "";

		if (weapon_id == weapon_idS::ALTERNATOR)
			return "ALTERNATOR";
		else if (weapon_id == weapon_idS::BOCEK)
			return "BOCEK";
		else if (weapon_id == weapon_idS::CAR_MP)
			return "C.A.R.-MP";
		else if (weapon_id == weapon_idS::CHARGE_RIFLE)
			return "CHARGE-RIFLE";
		else if (weapon_id == weapon_idS::DEVOTION)
			return "DEVOTION";
		else if (weapon_id == weapon_idS::EVA_8_AUTO)
			return "EVA-8 AUTO";
		else if (weapon_id == weapon_idS::FLATLINE)
			return "FLATLINE";
		else if (weapon_id == weapon_idS::G7_SCOUT)
			return "G7 SCOUT";
		else if (weapon_id == weapon_idS::HAVOC)
			return "HAVOC";
		else if (weapon_id == weapon_idS::HEMLOK)
			return "HAMLOK";
		else if (weapon_id == weapon_idS::KRABER)
			return "KRABER";
		else if (weapon_id == weapon_idS::LONGBOW)
			return "LONGBOW";
		else if (weapon_id == weapon_idS::L_STAR)
			return "L-STAR";
		else if (weapon_id == weapon_idS::MASTIFF)
			return "MASTIFF";
		else if (weapon_id == weapon_idS::MOSAMBIK)
			return "MOZAMBIQUE";
		else if (weapon_id == weapon_idS::P2020)
			return "P2020";
		else if (weapon_id == weapon_idS::PEACEKEEPER)
			return "PEACEKEEPER";
		else if (weapon_id == weapon_idS::PROWLER)
			return "PROWLER";
		else if (weapon_id == weapon_idS::R301)
			return "R301";
		else if (weapon_id == weapon_idS::R99)
			return "R99";
		else if (weapon_id == weapon_idS::RAMPAGE)
			return "RAMPAGE";
		else if (weapon_id == weapon_idS::RE_45)
			return "RE-45 AUTO";
		else if (weapon_id == weapon_idS::SENTINEL)
			return "SENTINEL";
		else if (weapon_id == weapon_idS::SPITFIRE)
			return "SPITFIRE";
		else if (weapon_id == weapon_idS::TRIPLE_TAKE)
			return "TRIPLE TAKE";
		else if (weapon_id == weapon_idS::VOLT)
			return "VOLT-MP";
		else if (weapon_id == weapon_idS::WINGMAN)
			return "WINGMAN";
		else if (weapon_id == weapon_idS::_30_30)
			return "30-30 REPEATER";
		else if (weapon_id == weapon_idS::MEELE)
			return "MEELE";
		else
			return "UNKNOWN";
	}
};