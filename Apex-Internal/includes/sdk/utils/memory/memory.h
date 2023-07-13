#pragma once
#include<Windows.h>
#include<math.h>
#include<memory>

class ProcMem
{
public:
	template<class Type>
	__inline Type WriteMem(Type Source, Type WriteAddr, Type value) {
		return *(Type*)(Source + WriteAddr) = value;
	}
public:
	template<class Type>
	__inline Type ReadMem(Type Source, Type ReadAddr) {
		return *(Type*)(Source + ReadAddr);
	}
}; ProcMem* pProcMem;