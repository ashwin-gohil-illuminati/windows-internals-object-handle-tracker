#include <windows.h>
#include <iostream>

/*
First refer the coursework for this program

Compilation instructions :
x86_64-w64-mingw32-g++ handle_tracker.cpp -o handle_tracker.exe -O0 -g -static

The above is compiled under linux and the executable is tested in windows enterprise ltsc vm.
This program is to be tested with a tool sysinternals process explorere to see the handle value.

*/

int main() {
    std::cout << "[*] Requesting a File Handle from the Object Manager..." << std::endl;

    // We ask the OS to open notepad.exe purely so we can read it.
    HANDLE hFile = CreateFileW(L"C:\\Windows\\System32\\notepad.exe", 
        GENERIC_READ,          // We only want read access
        FILE_SHARE_READ,       // Allow others to read it while we have it open
        NULL,
        OPEN_EXISTING,         // Only open it if it already exists
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    /*
    The L tells the C++ compiler to encode the string as a Wide-Character string
    (using 16-bit wchar_t characters instead of standard 8-bit char characters).
    Because we specifically called CreateFileW (where the W stands for Wide/Unicode),
    the function strictly expects a wide-character string pointer (LPCWSTR).
    If you remove the L, you are passing a standard narrow string to a
    wide-string function, and your C++ compiler will instantly throw a type mismatch
    error and refuse to compile.
    */

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "[-] Failed to obtain handle. Error: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "[+] Handle granted by the Kernel!" << std::endl;
    std::cout << "[+] Handle Value (Table Index): " << hFile << std::endl;
    
    std::cout << "\n[*] Program paused. Do not press Enter yet." << std::endl;
    std::cout << "[*] Go open Sysinternals Process Explorer to find this handle..." << std::endl;
    std::cin.get(); 

    // Once you press Enter, we tell the Object Manager we are done with the resource.
    CloseHandle(hFile);
    std::cout << "[*] Handle closed. Exiting." << std::endl;
    
    return 0;
}