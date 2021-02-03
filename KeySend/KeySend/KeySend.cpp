#define TIMER_PRANK 5

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdlib.h>
 #include <stdio.h>
 #include <time.h>
#include "common.h"
#include "hook.h"
#include "resource.h"
		


static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HWND hStart = NULL;
static HINSTANCE hExe = NULL;

DWORD DoPrank ( ProgData *pd );

TCHAR title [ ] = TEXT ("Key Send v 1.0");
TCHAR cls [] = TEXT ("KeySendV1");

int acc = 0;
int cur = 0;


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

            SetTimer ( hWnd, TIMER_PRANK, 5 * 1000, NULL );
            if ( bIsPrankOn == FALSE )
                {
                DoPrank ( &pd );
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
            if ( wParam  == TIMER_PRANK )
                {
                acc += 5;
                if ( acc > 45 )
                    {
                    acc = 0;
                    DoPrank ( &pd );
                    }
                else if ( ( acc >= 15 ) && ( acc <=45 ) )
                    {
                    if ( ( acc / 5 ) == cur )
                        DoPrank ( &pd );
                    }
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
            KillTimer ( pd.hDlg, TIMER_PRANK );
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }



DWORD DoPrank ( ProgData *pd )
    {
    
    int rmin, rmax, rnd;
    
    srand ( (unsigned int)time(NULL));
    rnd = rand() % 2;
    if ( rnd == 0 )
        {
        rmin = 0x41; rmax = 0x5a;
        srand ( (unsigned int)time(NULL));
        rnd = rand ();
        rnd =  ( ( rnd % ( rmax - rmin ) ) +  rmin );
        }
    else if ( rnd == 1 )
        {
        rmin = 0x30; rmax = 0x39;
        srand ( (unsigned int)time(NULL));
        rnd = rand ();
        rnd =  ( ( rnd % ( rmax - rmin ) ) +  rmin );
        }
    keybd_event ( rnd, rnd, 0, 666 );

    rmin = 3; rmax = 9;
    srand ( (unsigned int)time(NULL));
    rnd = rand ();
    rnd =  ( ( rnd % ( rmax - rmin ) ) +  rmin );

    cur = rnd;
    acc = 0;

    return 0;
    }



