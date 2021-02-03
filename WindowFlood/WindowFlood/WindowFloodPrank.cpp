#define TIMER_PRANK 5
#define TIMER_RAINCHECK 6
#define WM_UNINST_PRANK WM_USER+ 2

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "common.h"
//#include "hook.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
		
static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HINSTANCE hExe = NULL;

BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam );

DWORD DoPrank (  );
DWORD UndoPrank (  );

static TCHAR title [ ] = TEXT ("Window Flood Prank v 1.0");
static TCHAR cls [] = TEXT ("WindowFloodPrankV1");
static TCHAR appdir [ 1024];

static int cx = 0;
static int cy = 0;

static DWORD tick1 = 0;
static DWORD tick2 = 0;



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
    static TCHAR *beg = NULL;
    static TCHAR *substr = NULL;
    
	switch (message)
	    {
        case WM_CREATE:
            pd.hDlg = hWnd;
            pd.hCurs = NULL;
            Initialize ( &pd );

            SetTimer ( hWnd, TIMER_PRANK, 1 * 60 * 1000, NULL );
            //SetTimer ( hWnd, TIMER_RAINCHECK, 2 * 1000, NULL );

            GetModuleFileName ( NULL, appdir, 1024 );
            beg = appdir;
            while ( true )
                {
                beg = _tcsstr ( beg, TEXT ("\\"));
                if ( beg!= NULL )
                    {
                    substr = beg;
                    beg += lstrlen ( TEXT("\\"));
                    }
                else
                    break;
                }
            if ( substr != NULL )
                *substr = TEXT ('\0');

            cx = GetSystemMetrics ( SM_CXSCREEN );
            cy = GetSystemMetrics ( SM_CYSCREEN );

            SendMessage ( hWnd, WM_TIMER, TIMER_PRANK, 0 );
            break;

        case WM_TIMER:
            if ( wParam == TIMER_PRANK )
                DoPrank (  );
            /*else if ( wParam == TIMER_RAINCHECK )
                {
                tick2 = GetTickCount ();
                if ( ( ( tick2 - tick1 ) >= 7000 ) && ( bIsPrankOn == TRUE ) )
                    {
                    UndoPrank ();
                    }
                //}*/
            break;

        case WM_UNINST_PRANK:
            UndoPrank ();
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

        
        
        case WM_COMMAND:
            switch (wParam )
                {   
                case IDM_ABOUT:
                    ShowAboutDialog ( &pd );
                    break;
                case IDM_PBOSS:
                    GotoPrankBoss ( &pd );
                    break;

                }
            break;

	    case WM_DESTROY:
            CleanUp ( &pd );
            KillTimer ( hWnd, TIMER_PRANK );
            //KillTimer ( hWnd, TIMER_RAINCHECK );
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }


DWORD DoPrank (  )
    {

    TileWindows ( NULL, NULL, NULL, NULL, NULL );   
    bIsPrankOn = TRUE;
   
    
    return 0;
    }


DWORD UndoPrank (  )
    {

   
    EnumWindows ( EnumWindowsProc, (LPARAM)0 );
    CascadeWindows ( NULL, NULL, NULL, NULL, NULL );
    bIsPrankOn = FALSE;
 
    return 0;
    }




BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
    {

    
    if ( ( IsIconic ( hwnd )) && (IsWindowVisible ( hwnd )))
        {
        ShowWindow ( hwnd, SW_MAXIMIZE );
        }
    
    return TRUE;
    }