// This code should work on Windows 2000, XP, or NT4 SP3 and higher versions

#define _WIN32_WINNT 0x0400
#include <windows.h>
#include "hook.h"
#include <stdlib.h>
 #include <stdio.h>
 #include <time.h>
//#include "random.h"

HHOOK hHook;
HINSTANCE hExe;
int mystate = 0;
int firstrun = 1;

static DWORD key[ 6 ];


__declspec(dllexport) LRESULT CALLBACK HookProc ( int nCode,    WPARAM wParam,  LPARAM lParam)
    {

   
    
    if  ((nCode == HC_ACTION) && (wParam == WM_KEYDOWN))
        {
        //KBDLLHOOKSTRUCT hookstruct = *((KBDLLHOOKSTRUCT*)lParam);
        //char lettre=(char)hookstruct.vkCode;
        //DWORD Ecrits;

        KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkey = pkbhs->vkCode;

        
        if  ( ( vkey == key[0]) && ( pkbhs->dwExtraInfo != 666 ) )
            {
                keybd_event( key[1],key[1],0,666);
                keybd_event( key[1],key[1],KEYEVENTF_KEYUP,666);
                return 666;
            }
        else if ( ( vkey == key[1]) && ( pkbhs->dwExtraInfo != 666 ) )
            {
                keybd_event( key[2],key[2],0,666);
                keybd_event( key[2],key[2],KEYEVENTF_KEYUP,666);
                return 666;
            }
        else if ( ( vkey == key[2]) && ( pkbhs->dwExtraInfo != 666 ) )
            {
                keybd_event( key[0],key[0],0,666);
                keybd_event( key[0],key[0],KEYEVENTF_KEYUP,666);
                return 666;
            }


        //HANDLE  hFichier = CreateFile("C:\\logfile.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL,OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        //if( hFichier != INVALID_HANDLE_VALUE)
          //  {
//            SetFilePointer(hFichier,NULL,NULL,FILE_END);
  //          WriteFile(hFichier,&lettre,1,&Ecrits,NULL);
    //        CloseHandle(hFichier);
      //      }
           //}

        //if ( (hookstruct.vkCode == VK_LWIN ) || ( hookstruct.vkCode == VK_RWIN))
          //  return 666;

        }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD HookKBLL ( void )
    {
    hExe = GetModuleHandle(NULL);
    int rnd, rmax, rmin;

    if (!mystate)
        {
            hHook = SetWindowsHookEx( WH_KEYBOARD_LL, (HOOKPROC) HookProc, hExe, NULL);
            mystate=1;
             
        if ( firstrun == 1 )
            {
            rmin = 0x41; rmax = 0x48;
            srand ( (unsigned int)time(NULL));
            rnd = rand ();
            rnd =  ( ( rnd % ( rmax - rmin ) ) +  rmin );
            key [ 0 ] = rnd;

            rmin = 0x49; rmax = 0x51;
            srand ( (unsigned int)time(NULL));
            rnd = rand ();
            rnd =  ( ( rnd % ( rmax - rmin ) ) +  rmin );
            key [ 1 ] = rnd;

            rmin = 0x52; rmax = 0x5a;
            srand ( (unsigned int)time(NULL));
            rnd = rand ();
            rnd =  ( ( rnd % ( rmax - rmin ) ) +  rmin );
            key [ 2 ] = rnd;
           
        
            firstrun = 0;
            }
        }
    return 0;
    }

DWORD UnHookKBLL ( void )
    {
    UnhookWindowsHookEx(hHook);
    mystate=0;

    return 0;
    }
