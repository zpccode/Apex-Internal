#pragma once
#include"../../../sdk/apex_src/cusercmd/cusercmd.hpp"
#include"../../../sdk/utils/offsets/offsets.hpp"
#define STATUS_SUCCESS 1
#define STATUS_ERROR 0

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
}; CPrediction* pPredicition;