#define TIMER_PRANK 5
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

DWORD DoPrank (  );
DWORD UndoPrank (  );

static TCHAR title [ ] = TEXT ("Time Spoiler Prank v 1.0");
static TCHAR cls [] = TEXT ("TimePrankV1");
static TCHAR appdir [ 1024];

static WORD hour = 0;
static WORD minute = 0;
static int direction = 1; // 1 is positive ( delta was added to real time)
//0 is negative ( delta was subtracted from real time)


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

            SetTimer ( hWnd, TIMER_PRANK, 3 * 60 * 1000, NULL );

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

            SendMessage ( hWnd, WM_TIMER, TIMER_PRANK, 0 );
            break;

        case WM_TIMER:
            if ( wParam == TIMER_PRANK )
                DoPrank (  );
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
            //UndoPrank ();
            KillTimer ( hWnd, TIMER_PRANK );
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }


DWORD DoPrank (  )
    {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    LUID luid;
    TOKEN_PRIVILEGES tkpold;
    DWORD retlen = 0;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &luid); 
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof (TOKEN_PRIVILEGES), &tkpold, &retlen ); 
 
    SYSTEMTIME st;
    GetLocalTime ( &st );

    srand( (unsigned)time( NULL ) );

    int maxh, maxm;
    maxh = maxm = 0;

    if ( ( hour != 0 ) && ( minute != 0 ) )
        {
        if ( direction == 1 ) 
            {
            st.wHour -= hour;
            st.wMinute -= minute;
            }
        else
            {
            st.wHour += hour;
            st.wMinute += minute;
            }
        
        if ( st.wMinute >= 60 )
            {
            st.wMinute %= 60;
            st.wHour +=1 ;
            }
        if ( st.wHour >= 24 )
            st.wHour %= 24;
        }

    if ( direction == 1 )
        direction = 0;
    else
        direction = 1;

    if ( direction == 1 )
        {
        maxh = 23 - st.wHour;
        maxm = 59 - st.wMinute;
        }
    else
        {
        maxh = st.wHour;
        maxm = st.wMinute;
        }

    hour = rand() % maxh;
    minute = rand () % maxm;
        
    if ( direction == 0 ) 
        {
        st.wHour -= hour;
        st.wMinute -= minute;
        }
    else
        {
        st.wHour += hour;
        st.wMinute += minute;
        }

    if ( st.wMinute >= 60 )
        {
        st.wMinute %= 60;
        st.wHour +=1 ;
        }
    if ( st.wHour >= 24 )
        st.wHour %= 24;

    SetLocalTime ( &st );
        
        
    tkpold.Privileges[0].Attributes = 0; 
    AdjustTokenPrivileges(hToken, FALSE, &tkpold, sizeof (TOKEN_PRIVILEGES), &tkp, &retlen ); 
    CloseHandle ( hToken );

    return 0;
    }


DWORD UndoPrank (  )
    {

    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    LUID luid;
    TOKEN_PRIVILEGES tkpold;
    DWORD retlen = 0;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &luid); 
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof (TOKEN_PRIVILEGES), &tkpold, &retlen ); 
 
    SYSTEMTIME st;
    GetLocalTime ( &st );

    int maxh, maxm;
    maxh = maxm = 0;

    if ( ( hour != 0 ) && ( minute != 0 ) )
        {
        if ( direction == 1 ) 
            {
            st.wHour -= hour;
            st.wMinute -= minute;
            }
        else
            {
            st.wHour += hour;
            st.wMinute += minute;
            }
        
        if ( st.wMinute >= 60 )
            {
            st.wMinute %= 60;
            st.wHour +=1 ;
            }
        if ( st.wHour >= 24 )
            st.wHour %= 24;
        }
        

    

    SetLocalTime ( &st );
        
        
    tkpold.Privileges[0].Attributes = 0; 
    AdjustTokenPrivileges(hToken, FALSE, &tkpold, sizeof (TOKEN_PRIVILEGES), &tkp, &retlen ); 

    CloseHandle ( hToken );

    return 0;
    }

