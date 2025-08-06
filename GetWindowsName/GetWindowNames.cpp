#include <windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    const DWORD TITLE_SIZE = 1024;
    char windowTitle[TITLE_SIZE];

    // Get the window title
    GetWindowTextA(hwnd, windowTitle, TITLE_SIZE);

    // Only print windows with a visible title
    if (IsWindowVisible(hwnd) && strlen(windowTitle) > 0) {
        std::cout << windowTitle << std::endl;
    }

    return TRUE;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Processes running in the computer:\n";

    // Enumerate all windows
    EnumWindows(EnumWindowsProc, 0);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "\nProcess returned 0 (0x0)   execution time : ";
    std::cout << std::fixed << std::setprecision(6) << elapsed.count() << " s\n";
    std::cout << "Press any key to continue.\n";
    std::cin.get();  // Wait for key press

    return 0;
}
