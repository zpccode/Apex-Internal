#pragma once
#include"../variables/variables.hpp"
#define STATUS_SUCCESS 1
#define STATUS_ERROR 0

class CMisc
{
private:
	NTSTATUS WINAPI UnlockAll() { return STATUS_SUCCESS; }
public:
	NTSTATUS WINAPI Initialize()
	{
		if (misc::unlock_all) { this->UnlockAll(); }
		return STATUS_SUCCESS;
	}
}; CMisc* pMisc = new CMisc();