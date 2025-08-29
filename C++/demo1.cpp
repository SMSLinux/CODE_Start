#include <windows.h>
#include <iostream>

// 示例：查找、读取、修改目标进程内存
int main() {
    DWORD pid;
    std::cout << "请输入目标进程PID: ";
    std::cin >> pid;

    // 打开目标进程
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProcess) {
        std::cerr << "无法打开进程，错误码: " << GetLastError() << std::endl;
        return 1;
    }

    // 假设已知目标内存地址和数据类型
    LPCVOID targetAddress = (LPCVOID)0x00400000; // 示例地址
    int value = 0;

    // 读取内存
    SIZE_T bytesRead;
    if (ReadProcessMemory(hProcess, targetAddress, &value, sizeof(value), &bytesRead)) {
        std::cout << "读取到的值: " << value << std::endl;
    } else {
        std::cerr << "读取内存失败，错误码: " << GetLastError() << std::endl;
    }

    // 修改内存
    int newValue = 12345;
    SIZE_T bytesWritten;
    if (WriteProcessMemory(hProcess, (LPVOID)targetAddress, &newValue, sizeof(newValue), &bytesWritten)) {
        std::cout << "成功写入新值: " << newValue << std::endl;
    } else {
        std::cerr << "写入内存失败，错误码: " << GetLastError() << std::endl;
    }

    CloseHandle(hProcess);
    return 0;
}