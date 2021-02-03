#define _WIN32_WINNT 0x0400
#include <windows.h>

#include "hook.h"
#include "resource.h"

HHOOK hHook;
HINSTANCE hExe;
int mystate = 0;
HCURSOR hCurs = NULL;
HCURSOR hPrev = NULL;

__declspec(dllexport) LRESULT CALLBACK HookProc ( int nCode,    WPARAM wParam,  LPARAM lParam)
    {
    
    HMODULE hMod = GetModuleHandle (0);
    if  ((nCode == HC_ACTION) && (wParam == WM_SETCURSOR))
        {
       
        //PlaySound ( MAKEINTRESOURCE(IDWAVE_MOUSE), hMod, SND_NODEFAULT | SND_ASYNC | SND_RESOURCE );
        SetCursor ( hCurs );
        return 666;

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
            hCurs = LoadCursor ( GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR));
            hPrev = SetCursor ( hCurs );
       
        }
    return 0;
    }

DWORD UnHookMouseLL ( void )
    {
    UnhookWindowsHookEx(hHook);
    mystate=0;
    SetCursor (hPrev);

    DestroyCursor ( hCurs );

    return 0;
    }
