// This code should work on Windows 2000, XP, or NT4 SP3 and higher versions

#define _WIN32_WINNT 0x0400
#include <windows.h>
#include "hook.h"

HHOOK hHook;
HINSTANCE hExe;
int mystate = 0;


__declspec(dllexport) LRESULT CALLBACK HookProc ( int nCode,    WPARAM wParam,  LPARAM lParam)
    {
    //if  ((nCode == HC_ACTION) && (wParam == WM_KEYDOWN))
      //  {
        return 666;
        //}
    //return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD HookKBLL ( void )
    {
    hExe = GetModuleHandle(NULL);
    if (!mystate)
        {
        hHook = SetWindowsHookEx( WH_KEYBOARD_LL, (HOOKPROC) HookProc, hExe, NULL);
        mystate=1;
        }
    return 0;
    }

DWORD UnHookKBLL ( void )
    {
    UnhookWindowsHookEx(hHook);
    mystate=0;

    return 0;
    }
