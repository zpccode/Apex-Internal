#pragma once
#include<Windows.h>
#include<cstdint>

class IClientEntityList
{
public:
	virtual bool isEntity(void) = 0;
	virtual bool isLocalEntity(void) = 0;
};