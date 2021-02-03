#define _WIN32_WINNT 0x0400
#include <windows.h>

#include "hook.h"
#include "resource.h"

HHOOK hHook;
HINSTANCE hExe;
int mystate = 0;

__declspec(dllexport) LRESULT CALLBACK HookProc ( int nCode,    WPARAM wParam,  LPARAM lParam)
    {
    
    HMODULE hMod = GetModuleHandle (0);
    if  ((nCode == HC_ACTION) && (wParam == WM_LBUTTONDOWN))
        {
       
        PlaySound ( MAKEINTRESOURCE(IDWAVE_MOUSE), hMod, SND_NODEFAULT | SND_ASYNC | SND_RESOURCE );

        }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
    }



DWORD HookMouseLL ( void )
    {
    hExe = GetModuleHandle(NULL);
    int rnd, rmax, rmin;

    if (!mystate)
        {
            hHook = SetWindowsHookEx( WH_MOUSE_LL, (HOOKPROC) HookProc, hExe, NULL);
            mystate=1;
             
       
        }
    return 0;
    }

DWORD UnHookMouseLL ( void )
    {
    UnhookWindowsHookEx(hHook);
    mystate=0;

    return 0;
    }
