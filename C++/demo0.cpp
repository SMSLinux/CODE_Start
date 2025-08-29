#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

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

int main() {
    // 1. 查找窗口
    HWND hwnd = FindWindowA(NULL, "windowsmain");
    if (!hwnd) {
        std::cout << "窗口未找到" << std::endl;
        return 1;
    }

    // 2. 获取进程ID
    DWORD pid = GetProcessIdByHwnd(hwnd);
    if (!pid) {
        std::cout << "无法获取进程ID" << std::endl;
        return 1;
    }

    // 3. 打开进程
    HANDLE hProcess = OpenTargetProcess(pid);
    if (!hProcess) {
        std::cout << "无法打开进程" << std::endl;
        return 1;
    }

    // 4. 示例：读取和写入内存
    // 假设目标地址和数据类型
    uintptr_t targetAddress = 0x12345678; // 需要替换为实际地址
    int value = 0;

    // 读取
    if (ReadMemory(hProcess, (LPCVOID)targetAddress, &value, sizeof(value))) {
        std::cout << "读取到的值: " << value << std::endl;
    } else {
        std::cout << "读取失败" << std::endl;
    }

    // 修改
    int newValue = 999;
    if (WriteMemory(hProcess, (LPVOID)targetAddress, &newValue, sizeof(newValue))) {
        std::cout << "写入成功" << std::endl;
    } else {
        std::cout << "写入失败" << std::endl;
    }

    CloseHandle(hProcess);
    return 0;
}