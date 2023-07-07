#include "mem.h"

DWORD mem::GetProcId(const wchar_t* ProcName)
{
    DWORD ProcessId {};
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 ProcEntry;
        ProcEntry.dwSize = sizeof(ProcEntry);
        if (Process32First(hSnap, &ProcEntry))
        {
            do
            {
                if (!_wcsicmp(ProcName, ProcEntry.szExeFile))
                {
                    ProcessId = ProcEntry.th32ParentProcessID;
                    break;
                }

            } while (Process32Next(hSnap, &ProcEntry));
        }
        CloseHandle(hSnap);
        return ProcessId;
    }
    return 0;
}

uintptr_t mem::GetModuleBaseAddr(DWORD ProcId, wchar_t* ProcName)
{
    uintptr_t ModBaseAddr {};
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE, 0);
    MODULEENTRY32 ModuleEntry;
    ModuleEntry.th32ProcessID = sizeof(ModuleEntry);  //Get module number (PID)
    if (Module32First(hSnap, &ModuleEntry))
    {
        do
        {
            if (!_wcsicmp(ProcName, ModuleEntry.szExePath))
            {
                auto ModBaseAddr = reinterpret_cast<uintptr_t*>(ModuleEntry.modBaseAddr);
                break;
            }
        } while (Module32Next(hSnap, &ModuleEntry));

    }
    CloseHandle(hSnap);
    return ModBaseAddr;

}

uintptr_t mem::FindDMA(uintptr_t ptr, std::vector<unsigned int> Offset)
{
    uintptr_t Addr;
    for (auto i : Offset )
    {
        Addr = *(uintptr_t*)Addr;
        Addr += Offset[i];
    }

    return Addr;
}

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
    DWORD OldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &OldProtect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, OldProtect, &OldProtect);
}

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
    DWORD oldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    WriteProcessMemory(hProcess, dst, src, sizeof(src), nullptr);
    VirtualProtect(dst, size, oldProtect, &oldProtect);

}

void mem::Nop(BYTE* dst, unsigned int size)
{
    DWORD oldProtect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, oldProtect, &oldProtect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
    BYTE* NopArray = new BYTE[size];
    memset(NopArray, 0x90, size);

    PatchEx(dst, NopArray, size, hProcess);
    delete[] NopArray;

}
