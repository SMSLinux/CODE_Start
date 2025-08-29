#include <windows.h>
#include <stdio.h>

int main() {
    // 1. 查找窗口句柄（窗口标题请替换为目标软件实际标题）
    HWND hwnd = FindWindowA(NULL, "目标窗口标题");
    if (!hwnd) {
        printf("未找到窗口\n");
        return 1;
    }
    printf("窗口句柄: %p\n", hwnd);

    // 2. 获取进程ID
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    printf("进程ID: %lu\n", pid);

    // 3. 打开进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        printf("无法打开进程\n");
        return 1;
    }

    // 4. 读取进程内存（假设目标地址为0x12345678，读取4字节）
    int value = 0;
    SIZE_T bytesRead;
    if (ReadProcessMemory(hProcess, (LPCVOID)0x12345678, &value, sizeof(value), &bytesRead)) {
        printf("读取内存值: %d\n", value);
    } else {
        printf("读取内存失败\n");
    }

    // 5. 写入进程内存（将0x12345678地址的值改为100）
    int newValue = 100;
    SIZE_T bytesWritten;
    if (WriteProcessMemory(hProcess, (LPVOID)0x12345678, &newValue, sizeof(newValue), &bytesWritten)) {
        printf("写入内存成功\n");
    } else {
        printf("写入内存失败\n");
    }

    CloseHandle(hProcess);
    return 0;
}