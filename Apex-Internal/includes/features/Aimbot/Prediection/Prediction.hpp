#pragma once
#include"../../../sdk/apex_src/cusercmd/cusercmd.hpp"
#include"../../../sdk/utils/offsets/offsets.hpp"
#define STATUS_SUCCESS 1
#define STATUS_ERROR 0

extern "C" { namespace win_api
{
	inline float nt_abs(float x) {
		return SpoofReturn(fabsf, x);
	}
}}

class CPrediction
{
private:
	__declspec() VOID Prediction(UserCmd_t* pCmd) {

	}
	UserCmd_t* GetUserCmd(UserCmd_t* pCmd) {
		this->Prediction(pCmd);
		return pCmd;
	}
public:
	NTSTATUS WINAPI StartPred(UserCmd_t* pCmd) {
		if (!this->GetUserCmd(pCmd)){return STATUS_ERROR;}

		float currentAngleX = pCmd->m_vecViewAngles.x;
		float currentAngleY = pCmd->m_vecViewAngles.y;
		float currentAngleZ = pCmd->m_vecViewAngles.z;
		
		return STATUS_SUCCESS;
	}
	NTSTATUS WINAPI EndPred(UserCmd_t* pCmd) {
		return STATUS_SUCCESS;
	}
public:
	Vec2 SmoothCalc(Vec2 CurrentAngle, Vec2 AimAngle, float AimRate)
	{
		Vec2 Result;
		if (CurrentAngle.x != AimAngle.x)
		{
			if (CurrentAngle.x > AimAngle.x)
				Result.x = CurrentAngle.x - AimAngle.x;
			else
				Result.x = AimAngle.x - CurrentAngle.x;
			Result.x = Result.x * AimRate;
			if (CurrentAngle.x < AimAngle.x)
				Result.x = CurrentAngle.x + Result.x;
			if (CurrentAngle.x > AimAngle.x)
				Result.x = CurrentAngle.x - Result.x;
		}
		else
			Result.x = AimAngle.x;

		if (CurrentAngle.y != AimAngle.y)
		{
			if (CurrentAngle.y > AimAngle.y)
				Result.y = CurrentAngle.y - AimAngle.y;
			else
				Result.y = AimAngle.y - CurrentAngle.y;
			Result.y = Result.y * AimRate;
			if (CurrentAngle.y < AimAngle.y)
				Result.y = CurrentAngle.y + Result.y;
			if (CurrentAngle.y > AimAngle.y)
				Result.y = CurrentAngle.y - Result.y;
		}
		else
			Result.y = AimAngle.y;

		if (win_api::nt_abs(Result.x - CurrentAngle.x) > 30)
			return AimAngle;
		return Result;
	}
}; CPrediction* pPredicition;