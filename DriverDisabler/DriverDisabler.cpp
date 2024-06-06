#include "2.h"
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

bool DisableDLLFunctions(int processID, const wchar_t* dllName)
{
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (!processHandle) {
        std::cerr << "Failed to open process" << std::endl;
        return false;
    }

    HMODULE dllModule = GetModuleHandleW(dllName);
    if (!dllModule) {
        std::cerr << "Failed to get module handle for " << dllName << std::endl;
        return false;
    }

    LPVOID function1Address = (LPVOID)GetProcAddress(dllModule, "Function1");
    LPVOID function2Address = (LPVOID)GetProcAddress(dllModule, "Function2");
    if (!function1Address || !function2Address) {
        std::cerr << "Failed to get procedure address" << std::endl;
        return false;
    }

    BYTE patchBytes[] = { 0xC2, 0x0C, 0x00 };
    if (!WriteProcessMemory(processHandle, function1Address, &patchBytes, sizeof(patchBytes), NULL)) {
        std::cerr << "Failed to write to process memory (Function1)" << std::endl;
        return false;
    }
    if (!WriteProcessMemory(processHandle, function2Address, &patchBytes, sizeof(patchBytes), NULL)) {
        std::cerr << "Failed to write to process memory (Function2)" << std::endl;
        return false;
    }

    return true;
}

DWORD GetProcessIDByName(const wchar_t* processName)
{
    DWORD processID = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);
        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                    processID = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processID;
}

int main()
{
    const wchar_t* targetProcessName = L"Starme.exe";
    DWORD targetProcessID = 0;
    while (true)
    {
        targetProcessID = GetProcessIDByName(targetProcessName);
        if (targetProcessID != 0)
        {
            break;
        }
        Sleep(1000);
    }
    Sleep(1000);
    const wchar_t* targetDLL = L"mytarget.dll";
    if (DisableDLLFunctions(targetProcessID, targetDLL)) {
        std::cout << "DLL function bypass successful" << std::endl;
    }
    else {
        std::cerr << "DLL function bypass failed" << std::endl;
    }
    return 0;
}