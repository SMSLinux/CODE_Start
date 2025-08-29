#include <windows.h>
#include <stdio.h>

int main() {
    HWND hwnd = FindWindowA(NULL, "MainWindow");
    if (hwnd) {
        printf("窗口句柄: %p\n", hwnd);
    } else {
        printf("未找到窗口\n");
    }
    return 0;
}