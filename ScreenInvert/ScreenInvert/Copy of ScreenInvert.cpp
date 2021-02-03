#define TIMER_PRANK 5

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "common.h"
//#include "hook.h"
#include "resource.h"
		
static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HINSTANCE hExe = NULL;

static HDC hdcScr, hdcBmp, hdcBk;
static HBITMAP bmp, bmpbk;

DWORD DoPrank ( ProgData *pd );
DWORD UndoPrank (ProgData *pd );

static int cx, cy;
static DWORD tick1, tick2;

TCHAR title [ ] = TEXT ("Screen Invert v 1.0");
TCHAR cls [] = TEXT ("ScreenInvertV1");

LRESULT CALLBACK WndProcNew (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

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

            

            SetTimer ( hWnd, TIMER_PRANK, 30 * 1000, NULL );

            if ( bIsPrankOn == FALSE )
                {
                DoPrank ( &pd );
                bIsPrankOn = TRUE;
                }

            break;
        case WM_TIMER:
            if ( wParam == TIMER_PRANK )
                {
                if ( bIsPrankOn == FALSE )
                    {
                    DoPrank ( &pd );
                    bIsPrankOn = TRUE;
                    }
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
            

        //    if ( bIsPrankOn== TRUE )
          //      UndoPrank ( &pd );            
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }



DWORD DoPrank ( ProgData *pd )
    {
    HWND hwnd = pd->hDlg ;
    HRGN hrgn = CreateRectRgn ( 0,0,0,0 );
    GetWindowRgn ( GetDesktopWindow(), hrgn );

    cx = GetSystemMetrics ( SM_CXSCREEN );
    cy = GetSystemMetrics ( SM_CYSCREEN );

    HWND hProgMan  = FindWindow(TEXT("ProgMan"), NULL);	
	HWND hShellDef = FindWindowEx(hProgMan, NULL, TEXT("SHELLDLL_DefView"), NULL);
	HWND hDesktopWnd    = FindWindowEx(hShellDef, NULL, TEXT("SysListView32"), NULL);

    static int sav =0 ;
     //hwnd = GetDesktopWindow ();
    hwnd = hDesktopWnd;
    if (LockWindowUpdate (hwnd) )
        {
        
        hdcScr  = GetDCEx (NULL, hrgn, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;
        sav = SaveDC ( hdcScr );
        //hdcScr = GetDC ( NULL );
        hdcBmp  = CreateCompatibleDC (hdcScr) ;
        hdcBk  = CreateCompatibleDC (hdcScr) ;
        cx      = GetSystemMetrics (SM_CXSCREEN);
        cy      = GetSystemMetrics (SM_CYSCREEN);
        bmp = (HBITMAP)CreateCompatibleBitmap (hdcScr, cx, cy) ;
        bmpbk = (HBITMAP)CreateCompatibleBitmap (hdcScr, cx, cy) ;
        SelectObject (hdcBmp, (HGDIOBJ)bmp) ;
        SelectObject (hdcBk, (HGDIOBJ)bmpbk) ;

   
        MoveToEx(hdcScr,0, 0, NULL );
        LineTo (hdcScr, cx,cy );
        TextOut ( hdcScr, cx/2,cy/2, "Hello WOrld!", 13 );

        //BitBlt (hdcBmp,  0,  0, cx, cy, hdcScr, 0, 0, SRCCOPY) ;
        //BitBlt (hdcBk,  0,  0, cx, cy, hdcScr, 0, 0, SRCCOPY) ;
        
        //BitBlt (hdcScr,  0,  0, cx, cy, hdcBmp, 0, 0, NOTSRCCOPY) ;

        Sleep ( 3000 );
        //BitBlt (hdcScr,  0,  0, cx, cy, hdcBk, 0, 0, SRCCOPY) ;
        RestoreDC ( hdcScr, sav );
        LockWindowUpdate (NULL) ;
        DeleteDC (hdcBmp) ;
        DeleteDC (hdcBk) ;
        ReleaseDC (hwnd, hdcScr) ;
        DeleteObject (bmp) ;
        DeleteObject (bmpbk) ;

        }

 
    return 0;
    }




DWORD UndoPrank (ProgData *pd )
    {

    bIsPrankOn= FALSE;
    return 0;
    }