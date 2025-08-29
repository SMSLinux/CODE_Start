// demo0.cpp : 读取和修改 Windows 注册表中的 LUA（User Account Control）设置
#include <windows.h>
#include <iostream>

// 读取注册表中的LUA（User Account Control）设置
bool ReadLUAStatus(bool& enabled) {
    HKEY hKey;
    DWORD value = 0;
    DWORD valueSize = sizeof(value);
    LONG result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0,
        KEY_READ,
        &hKey
    );
    if (result != ERROR_SUCCESS) {
        std::cerr << "无法打开注册表键，错误码: " << result << std::endl;
        return false;
    }

    result = RegQueryValueExA(
        hKey,
        "EnableLUA",
        nullptr,
        nullptr,
        reinterpret_cast<LPBYTE>(&value),
        &valueSize
    );
    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        std::cerr << "无法读取EnableLUA，错误码: " << result << std::endl;
        return false;
    }

    enabled = (value != 0);
    return true;
}

// 修改注册表中的LUA（User Account Control）设置
bool WriteLUAStatus(bool enable) {
    HKEY hKey;
    LONG result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0,
        KEY_SET_VALUE,
        &hKey
    );
    if (result != ERROR_SUCCESS) {
        std::cerr << "无法打开注册表键，错误码: " << result << std::endl;
        return false;
    }

    DWORD value = enable ? 1 : 0;
    result = RegSetValueExA(
        hKey,
        "EnableLUA",
        0,
        REG_DWORD,
        reinterpret_cast<const BYTE*>(&value),
        sizeof(value)
    );
    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        std::cerr << "无法写入EnableLUA，错误码: " << result << std::endl;
        return false;
    }

    return true;
}

int main() {
    bool luaEnabled = false;
    if (ReadLUAStatus(luaEnabled)) {
        std::cout << "当前LUA状态: " << (luaEnabled ? "启用" : "禁用") << std::endl;
    }

    // 示例：切换LUA状态
    bool newStatus = !luaEnabled;
    if (WriteLUAStatus(newStatus)) {
        std::cout << "已将LUA状态修改为: " << (newStatus ? "启用" : "禁用") << std::endl;
    } else {
        std::cout << "修改LUA状态失败。" << std::endl;
    }

    return 0;
}