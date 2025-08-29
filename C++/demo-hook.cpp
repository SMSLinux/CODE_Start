#include <windows.h>
#include <iostream>
#include "MinHook.h"

/*
// MinHook.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define MH_OK 0

typedef void* LPVOID;

int MH_Initialize(void);
int MH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal);
int MH_EnableHook(LPVOID pTarget);

#ifdef __cplusplus
}
#endif

注意：此代码只能在 Windows 平台下编译运行，Linux 环境下无法找到 windows.h，如需在 Windows 下学习和测试，请用 Windows 编译器（如 MinGW、MSVC）进行编译。
*/


// 原始 MessageBoxA 函数指针
typedef int (WINAPI* MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
MessageBoxA_t fpMessageBoxA = nullptr;

// Hook 后的 MessageBoxA
int WINAPI HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    // 修改消息内容
    return fpMessageBoxA(hWnd, "内容已被HOOK修改！", lpCaption, uType);
}

DWORD WINAPI MainThread(LPVOID) {
    // 初始化 MinHook
    if (MH_Initialize() != MH_OK) return 1;

    // 创建 hook
    if (MH_CreateHook(&MessageBoxA, &HookedMessageBoxA, reinterpret_cast<LPVOID*>(&fpMessageBoxA)) != MH_OK) return 1;

    // 启用 hook
    if (MH_EnableHook(&MessageBoxA) != MH_OK) return 1;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
    }
    return TRUE;
}
