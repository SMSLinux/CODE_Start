// demo01.cpp : 精简版内存读写示例
#include <windows.h>
#include <iostream>
#include <vector>

DWORD GetProcessIdByHwnd(HWND hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

HANDLE OpenTargetProcess(DWORD pid) {
    return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
}

bool ReadMemory(HANDLE hProcess, LPCVOID address, void* buffer, SIZE_T size) {
    return ReadProcessMemory(hProcess, address, buffer, size, nullptr);
}

bool WriteMemory(HANDLE hProcess, LPVOID address, const void* buffer, SIZE_T size) {
    return WriteProcessMemory(hProcess, address, buffer, size, nullptr);
}

uintptr_t GetFinalAddress(HANDLE hProcess, uintptr_t base, const std::vector<uintptr_t>& offsets) {
    uintptr_t addr = base;
    for (auto off : offsets) {
        if (!ReadMemory(hProcess, (LPCVOID)addr, &addr, sizeof(addr))) return 0;
        addr += off;
    }
    return addr;
}

int main() {
    HWND hwnd = FindWindowA(nullptr, "windowsmain");
    if (!hwnd) return std::cout << "窗口未找到\n", 1;

    DWORD pid = GetProcessIdByHwnd(hwnd);
    if (!pid) return std::cout << "无法获取进程ID\n", 1;

    HANDLE hProcess = OpenTargetProcess(pid);
    if (!hProcess) return std::cout << "无法打开进程\n", 1;

    uintptr_t baseAddress = 0x12345678; // 替换为实际基址
    std::vector<uintptr_t> offsets = {0x10}; // 替换为实际一级偏移

    uintptr_t finalAddress = GetFinalAddress(hProcess, baseAddress, offsets);
    if (!finalAddress) return std::cout << "获取最终地址失败\n", CloseHandle(hProcess), 1;

    int value = 0;
    if (ReadMemory(hProcess, (LPCVOID)finalAddress, &value, sizeof(value)))
        std::cout << "读取到的值: " << value << std::endl;
    else
        std::cout << "读取失败" << std::endl;

    int newValue = 999;
    if (WriteMemory(hProcess, (LPVOID)finalAddress, &newValue, sizeof(newValue)))
        std::cout << "写入成功" << std::endl;
    else
        std::cout << "写入失败" << std::endl;

    CloseHandle(hProcess);
    return 0;
}
