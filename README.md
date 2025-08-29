# CODE_Start

本项目适合 C 语言新手学习，主要内容为通过 Win32 API 获取运行软件窗口的句柄，并演示如何查找、修改、获取运行软件的内存。

## 功能说明

1. **查找窗口句柄**  
   使用 `FindWindowA` 等 Win32 API 查找指定窗口的句柄。

2. **查找进程ID**  
   通过窗口句柄获取进程ID。

3. **打开进程并获取进程句柄**  
   使用 `OpenProcess` 打开目标进程，获取操作内存所需的句柄。

4. **读取进程内存**  
   使用 `ReadProcessMemory` 读取目标进程的内存数据。

5. **写入进程内存**  
   使用 `WriteProcessMemory` 修改目标进程的内存数据。

## 示例代码

```c
#include <windows.h>
#include <stdio.h>

int main() {
    // 1. 查找窗口句柄
    HWND hwnd = FindWindowA(NULL, "MainWindow");
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
```

> 注意：上述代码仅能在 Windows 平台下编译运行，且需要有足够权限操作目标进程。实际使用时请根据目标软件实际窗口标题和内存地址进行调整。

## 运行方法

1. 用支持 Win32 API 的编译器（如 Windows 下的 gcc 或 MSVC）编译本项目。
2. 运行可执行文件，按提示操作。

---
