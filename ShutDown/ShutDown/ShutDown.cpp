#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shlobj.h>
#include "common.h"
//#include "hook.h"
#include "resource.h"

#define TIMER_SHUTDOWN 1
		
//typedef DWORD (WINAPI *EXPSHLSHDWN)(); 

//extern "C" __declspec(dllimport)  LRESULT WINAPI GetMsgHookProc(int nCode, WPARAM wParam, LPARAM lParam); 

//typedef extern "C" __declspec(dllimport) DWORD (WINAPI *EXPSHLSHDWN)();
typedef DWORD (WINAPI * EXPSHLSHDWN)( DWORD );
EXPSHLSHDWN  SHShutDownDialog;
//typedef DWORD (WINAPI *TEST)(DWORD, DWORD); 
//TEST test;

static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HINSTANCE hExe = NULL;

DWORD DoPrank ( ProgData *pd );

TCHAR title [ ] = TEXT ("Shutdown Prank v 1.0");
TCHAR cls [] = TEXT ("ShutDownV1");


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
    {
	MSG msg;
    hExe = hInstance;
    lstrcpy ( pd.cls, cls );
    lstrcpy ( pd.title, title );
	MyRegisterClass(hInstance, &pd);

	if (!InitInstance (hInstance, nCmdShow, &pd))
        return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }

	return (int) msg.wParam;
    }


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {

    DWORD ret;
    
	switch (message)
	    {
        case WM_CREATE:
            pd.hDlg = hWnd;
            pd.hCurs = NULL;
            Initialize ( &pd );

            if ( bIsPrankOn == FALSE )
                {
                SetTimer ( hWnd, TIMER_SHUTDOWN, 3 * 60 * 1000, NULL );
                SendMessage ( hWnd, WM_TIMER, (WPARAM)TIMER_SHUTDOWN, 0);
                bIsPrankOn = TRUE;
                }
            break;

    
        case WM_USRSYSTRAY:
            switch ( lParam )
                {
                case WM_LBUTTONDBLCLK:
                    ShowAboutDialog ( &pd );
                    break;

                case WM_RBUTTONUP:
                    ShowPopupMenu ( &pd );
                    break;

                case WM_LBUTTONUP:
                    ShowPopupMenu ( &pd );
                    break;
                }
            break;

        case WM_TIMER:
            if ( wParam == TIMER_SHUTDOWN )
                DoPrank ( &pd );
            break;
        
        case WM_COMMAND:
            switch (wParam )
                {   
                case IDM_ABOUT:
                    ShowAboutDialog ( &pd );
                    //SendMessage ( hWnd, WM_DESTROY, 0, 0 );
                    break;
                case IDM_PBOSS:
                    GotoPrankBoss ( &pd );
                    break;

                }
            break;

	    case WM_DESTROY:
            CleanUp ( &pd );
            KillTimer ( hWnd, TIMER_SHUTDOWN );
            bIsPrankOn = FALSE;
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }



DWORD DoPrank ( ProgData *pd )
    {

    //OSVERSIONINFO vi;
   
    //GetVersionEx ( &vi );
   
    //if ( vi.dwMajorVersion > 5 )
    //    {



    //    }
    //else
    //    {


    //    }


    HANDLE hToken;              // handle to process token 
    TOKEN_PRIVILEGES tkp;       // pointer to token structure 
    //BOOL fResult;               // system shutdown flag 
    //TCHAR msg [] = TEXT("To Uninstall this prank go to http://www.prankboss.com");


    //if (!OpenProcessToken(GetCurrentProcess(), 
      //                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
    //    return 0; 
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
    
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 
    //if (GetLastError() != ERROR_SUCCESS) 
      //  return 0; 
    //fResult = InitiateSystemShutdown( NULL, msg, 30, FALSE, TRUE);
    //RestartDialog ( pd->hDlg, NULL, EWX_REBOOT );
    //if (!fResult) 
      //  return 0; 
    int ordinal = 0x0000003c;
    TCHAR temp [ 16 ];
    ltoa ( ordinal, temp, 16 );
    HMODULE hModule = LoadLibraryW( L"shell32.dll" ); 
    SHShutDownDialog = (EXPSHLSHDWN)GetProcAddress( hModule, (const char*)0x3c ); 
    
    SHShutDownDialog ( 1 );

    tkp.Privileges[0].Attributes = 0; 
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
                            (PTOKEN_PRIVILEGES) NULL, 0); 
    

    //int ordinal = 60;
    //HMODULE hModule = LoadLibrary( TEXT("c:\\Shell32.dll") ); 
    //SHShutDownDialog = (EXPSHLSHDWN)GetProcAddress( hModule, (LPCTSTR)ordinal ); 
    //
    ////HMODULE hModule = LoadLibrary( TEXT("user32.dll") ); 
    ////test = (TEST)GetProcAddress( hModule, TEXT("#0292") ); 
    //
    //SHShutDownDialog ( 1 );

    /*ShellExectue (TEXT("rundll32.exe shell32.dll shutdown -i")*/


    
   /* int ordinal = 60;
    HMODULE hModule = LoadLibrary( TEXT("c:\\Shell32.dll") ); 
    SHShutDownDialog = (EXPSHLSHDWN)GetProcAddress( hModule, (LPCTSTR)ordinal );
    SHShutDownDialog ( 1 );*/

    return 0;
    }



