#include"../engine/engine.hpp"

enum class buttons
{
    IN_JUMP = (1 << 0),
    IN_GROUNDED = (1 << 1),
    IN_ATTACK = ( 1 << 2),
    IN_ATTACK2 = (1 << 3),
};

struct UserCmd_t
{
    uintptr_t pVft;                // 0x00
    int32_t   m_iCmdNumber;        // 0x04
    int32_t   m_iTickCount;        // 0x08
    Vector3 m_vecViewAngles;     // 0x0C
    Vector3 m_vecAimDirection;   // 0x18
    float     m_flForwardmove;     // 0x24
    float     m_flSidemove;        // 0x28
    float     m_flUpmove;          // 0x2C
    int32_t   m_iButtons;          // 0x30
    uint8_t   m_bImpulse;          // 0x34
    uint8_t   Pad1[3];
    int32_t   m_iWeaponSelect;     // 0x38
    int32_t   m_iWeaponSubtype;    // 0x3C
    int32_t   m_iRandomSeed;       // 0x40
    int16_t   m_siMouseDx;         // 0x44
    int16_t   m_siMouseDy;         // 0x46
    bool      m_bHasBeenPredicted; // 0x48
    uint8_t   Pad2[27];
}; // size is 100 or 0x64

struct Input_t
{
    uintptr_t m_pVftable;                   // 0x00
    bool      m_bTrackIRAvailable;          // 0x04
    bool      m_bMouseInitialized;          // 0x05
    bool      m_bMouseActive;               // 0x06
    bool      m_bJoystickAdvancedInit;      // 0x07
    uint8_t   Unk1[44];                     // 0x08
    uintptr_t m_pKeys;                      // 0x34
    uint8_t   Unk2[100];                    // 0x38
    bool      m_bCameraInterceptingMouse;   // 0x9C
    bool      m_bCameraInThirdPerson;       // 0x9D
    bool      m_bCameraMovingWithMouse;     // 0x9E
    Vector3 m_vecCameraOffset;            // 0xA0
    bool      m_bCameraDistanceMove;        // 0xAC
    int32_t   m_nCameraOldX;                // 0xB0
    int32_t   m_nCameraOldY;                // 0xB4
    int32_t   m_nCameraX;                   // 0xB8
    int32_t   m_nCameraY;                   // 0xBC
    bool      m_bCameraIsOrthographic;      // 0xC0
    Vector3 m_vecPreviousViewAngles;      // 0xC4
    Vector3 m_vecPreviousViewAnglesTilt;  // 0xD0
    float     m_flLastForwardMove;          // 0xDC
    int32_t   m_nClearInputState;           // 0xE0
    uint8_t   Unk3[0x8];                    // 0xE4
    uintptr_t m_pCommands;                  // 0xEC
    uintptr_t m_pVerifiedCommands;          // 0xF0
};