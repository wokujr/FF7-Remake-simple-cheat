#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>

class mem
{
public:
	mem()
	{

	}

	DWORD GetProcId(const wchar_t* ProcName);

	//compare proccess id and module name
	uintptr_t GetModuleBaseAddr(DWORD ProcId, wchar_t* ProcName);

	//ReadProccess Memory with offset
	uintptr_t FindDMA(uintptr_t ptr, std::vector<unsigned int>Offset);

protected:
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);

};

