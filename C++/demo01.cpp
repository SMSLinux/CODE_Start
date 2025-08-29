#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>

// 获取进程ID
DWORD GetProcessIdByHwnd(HWND hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

// 打开进程
HANDLE OpenTargetProcess(DWORD pid) {
    return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
}

// 读取内存
bool ReadMemory(HANDLE hProcess, LPCVOID address, void* buffer, SIZE_T size) {
    SIZE_T bytesRead;
    return ReadProcessMemory(hProcess, address, buffer, size, &bytesRead);
}

// 写入内存
bool WriteMemory(HANDLE hProcess, LPVOID address, const void* buffer, SIZE_T size) {
    SIZE_T bytesWritten;
    return WriteProcessMemory(hProcess, address, buffer, size, &bytesWritten);
}

// 通过多级指针获取最终地址
uintptr_t GetFinalAddress(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
    uintptr_t addr = baseAddress;
    for (size_t i = 0; i < offsets.size(); ++i) {
        if (!ReadMemory(hProcess, (LPCVOID)addr, &addr, sizeof(addr))) {
            return 0;
        }
        addr += offsets[i];
    }
    return addr;
}

int main() {
    HWND hwnd = FindWindowA(NULL, "windowsmain");
    if (!hwnd) {
        std::cout << "窗口未找到" << std::endl;
        return 1;
    }

    DWORD pid = GetProcessIdByHwnd(hwnd);
    if (!pid) {
        std::cout << "无法获取进程ID" << std::endl;
        return 1;
    }

    HANDLE hProcess = OpenTargetProcess(pid);
    if (!hProcess) {
        std::cout << "无法打开进程" << std::endl;
        return 1;
    }

    // 示例：基址和偏移
    uintptr_t baseAddress = 0x12345678; // 需要替换为实际基址
    std::vector<uintptr_t> offsets = {0x10}; // 一级偏移
    // std::vector<uintptr_t> offsets = {0x10, 0x20}; // 二级偏移

    uintptr_t finalAddress = GetFinalAddress(hProcess, baseAddress, offsets);
    if (finalAddress == 0) {
        std::cout << "获取最终地址失败" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    int value = 0;
    if (ReadMemory(hProcess, (LPCVOID)finalAddress, &value, sizeof(value))) {
        std::cout << "读取到的值: " << value << std::endl;
    } else {
        std::cout << "读取失败" << std::endl;
    }

    int newValue = 999;
    if (WriteMemory(hProcess, (LPVOID)finalAddress, &newValue, sizeof(newValue))) {
        std::cout << "写入成功" << std::endl;
    } else {
        std::cout << "写入失败" << std::endl;
    }

    CloseHandle(hProcess);
    return 0;
}