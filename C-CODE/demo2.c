#include <windows.h>
#include <stdio.h>
/*
注意：此代码只能在 Windows 平台下编译运行，Linux 环境下无法找到 windows.h，如需在 Windows 下学习和测试，请用 Windows 编译器（如 MinGW、MSVC）进行编译。
*/

// 通过多级指针偏移获取目标地址
uintptr_t GetFinalAddress(HANDLE hProcess, uintptr_t baseAddr, uintptr_t offsets[], int offsetCount) {
	uintptr_t addr = baseAddr;
	for (int i = 0; i < offsetCount; ++i) {
		if (!ReadProcessMemory(hProcess, (LPCVOID)addr, &addr, sizeof(addr), NULL)) {
			printf("读取偏移失败: %d\n", i);
			return 0;
		}
		addr += offsets[i];
	}
	return addr;
}

int main() {
	// 1. 查找窗口句柄（请替换为实际窗口标题）
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

	// 4. 设定基址和偏移（示例：基址0x12345678，一级偏移0x10，二级偏移0x20）
	uintptr_t baseAddr = 0x12345678;
	uintptr_t offsets[] = {0x10, 0x20}; // 一级、二级偏移
	int offsetCount = sizeof(offsets) / sizeof(offsets[0]);

	// 5. 获取最终地址
	uintptr_t finalAddr = GetFinalAddress(hProcess, baseAddr, offsets, offsetCount);
	if (finalAddr == 0) {
		printf("获取最终地址失败\n");
		CloseHandle(hProcess);
		return 1;
	}
	printf("最终内存地址: 0x%p\n", (void*)finalAddr);

	// 6. 读取内存
	int value = 0;
	SIZE_T bytesRead;
	if (ReadProcessMemory(hProcess, (LPCVOID)finalAddr, &value, sizeof(value), &bytesRead)) {
		printf("读取内存值: %d\n", value);
	} else {
		printf("读取内存失败\n");
	}

	// 7. 写入内存（将值改为100）
	int newValue = 100;
	SIZE_T bytesWritten;
	if (WriteProcessMemory(hProcess, (LPVOID)finalAddr, &newValue, sizeof(newValue), &bytesWritten)) {
		printf("写入内存成功\n");
	} else {
		printf("写入内存失败\n");
	}

	CloseHandle(hProcess);
	return 0;
}
