#include <windows.h>
#include <iostream>
#include <vector>

// 获取进程ID
DWORD GetProcessIdByHwnd(HWND hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

// 通过多级指针获取目标地址
uintptr_t GetAddress(HANDLE hProcess, uintptr_t baseAddr, const std::vector<uintptr_t>& offsets) {
    uintptr_t addr = baseAddr;
    for (size_t i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(hProcess, (LPCVOID)addr, &addr, sizeof(addr), nullptr);
        addr += offsets[i];
    }
    return addr;
}

int main() {
    // 1. 获取窗口句柄
    HWND hwnd = FindWindowA(NULL, "windowsmain");
    if (!hwnd) {
        std::cout << "窗口未找到" << std::endl;
        return 1;
    }

    // 2. 获取进程ID
    DWORD pid = GetProcessIdByHwnd(hwnd);
    if (!pid) {
        std::cout << "进程ID获取失败" << std::endl;
        return 1;
    }

    // 3. 打开进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cout << "无法打开进程" << std::endl;
        return 1;
    }

    // 4. 基址和偏移（示例：基址0x12345678，一级偏移0x10，二级偏移0x20）
    uintptr_t baseAddr = 0x12345678; // 需要用工具查找
    std::vector<uintptr_t> offsets = {0x10, 0x20};

    // 5. 获取目标地址
    uintptr_t targetAddr = GetAddress(hProcess, baseAddr, offsets);

    // 6. 读内存
    int value = 0;
    if (ReadProcessMemory(hProcess, (LPCVOID)targetAddr, &value, sizeof(value), nullptr)) {
        std::cout << "读取值: " << value << std::endl;
    } else {
        std::cout << "读取失败" << std::endl;
    }

    // 7. 写内存（示例：写入1234）
    int newValue = 1234;
    if (WriteProcessMemory(hProcess, (LPVOID)targetAddr, &newValue, sizeof(newValue), nullptr)) {
        std::cout << "写入成功" << std::endl;
    } else {
        std::cout << "写入失败" << std::endl;
    }

    CloseHandle(hProcess);
    return 0;
}