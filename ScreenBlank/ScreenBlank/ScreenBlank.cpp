#define TIMER_PRANK 5

#define TIMER_CANCEL 6

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "common.h"
#include "hook.h"
#include "resource.h"
		
static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HWND hStart = NULL;
static HINSTANCE hExe = NULL;

static HDC hdcWin, hdcBmp;
static HWND hwndNew = NULL;

DWORD DoPrank ( ProgData *pd );
static int cx, cy;
static DWORD tick1, tick2;

TCHAR title [ ] = TEXT ("Screen Blank v 1.0");
TCHAR cls [] = TEXT ("ScreenBlankV1");

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

            cx = GetSystemMetrics ( SM_CXSCREEN );
            cy = GetSystemMetrics ( SM_CYSCREEN );
            cx = cx + 16;
            cy = cy + 16;

            SetTimer ( hWnd, TIMER_PRANK, 2 * 60 * 1000, NULL );

            if ( bIsPrankOn == FALSE )
                {
                DoPrank ( &pd );
                bIsPrankOn = TRUE;
                }
            break;
        case WM_TIMER:
            if ( wParam == TIMER_PRANK )
                {
                DoPrank ( &pd );

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
            UnHookKBLL();
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
    
    HWND hwnd = NULL;
    TCHAR szCls[] = TEXT ("SBPopupPhv1") ;
    hwnd = FindWindow ( TEXT("SBPopupPhv1"), NULL );
    bIsPrankOn = TRUE;
    HookKBLL();
    if ( hwnd != NULL )
        {
        SetWindowPos ( hwnd, HWND_TOPMOST, 0, 0, cx, cy, SWP_SHOWWINDOW );
        }
    else
        {
        WNDCLASS     wndclass ;

        wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
        wndclass.lpfnWndProc   = WndProcNew ;
        wndclass.cbClsExtra    = 0 ;
        wndclass.cbWndExtra    = 0 ;
        wndclass.hInstance     = pd->hInst;
        wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
        wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
        wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
        wndclass.lpszMenuName  = NULL ;
        wndclass.lpszClassName = szCls ;

        RegisterClass (&wndclass);
        hwndNew = CreateWindow ( szCls,                  // window class name
                          TEXT("Prankboss.com"), // window caption
                          WS_POPUP,         // window style
                          0,              // initial x position
                          0,              // initial y position
                          cx,              // initial x size
                          cy,              // initial y size
                          NULL,                       // parent window handle
                          NULL,                       // window menu handle
                          pd->hInst,                  // program instance handle
                          NULL) ;                     // creation parameters
     
        ShowWindow (hwndNew, SW_SHOW) ;
        UpdateWindow (hwndNew) ;
        SetWindowPos ( hwndNew, HWND_TOPMOST, 0, 0, cx, cy, SWP_SHOWWINDOW );
        }
    
    tick1 = GetTickCount ();
    return 0;
    }




LRESULT CALLBACK WndProcNew (HWND hwnd1, UINT message, WPARAM wParam, LPARAM lParam)
{
     static HDC         hdc ;
     static PAINTSTRUCT ps ;
     static RECT        rect ;
     static BITMAP bmp;
     static HBITMAP hBmp;
     static int xbmp, ybmp;
     static HDC hdcMem;
     static int i, j;


     
     switch (message)
     {
     case WM_CREATE:
          //PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
          
         hBmp = LoadBitmap ( pd.hInst, MAKEINTRESOURCE (IDBMP_BLACK));
         GetObject (hBmp, sizeof (BITMAP), &bmp );
         xbmp = bmp.bmWidth;
         ybmp = bmp.bmHeight;
         tick1 = GetTickCount ();

         //zHookKBLL();
         SetTimer ( hwnd1, TIMER_CANCEL, 3 * 1000, NULL );
         

         return 0 ;


     case WM_PAINT:
          hdc = BeginPaint (hwnd1, &ps) ;
          hdcMem = CreateCompatibleDC ( hdc );
          SelectObject ( hdcMem, hBmp );
          
          GetClientRect (hwnd1, &rect) ;
          //for ( i = 0; i < cx/xbmp; i+= xbmp )
              //for ( j = 0; j < cy/ybmp; j+= ybmp )
                //BitBlt ( hdc, 0, 0, cx, cy, hdcMem, 0, 0, BLAC );
                StretchBlt ( hdc, 0, 0, cx, cy, hdcMem, 0, 0, xbmp, ybmp, BLACKNESS );
          
          //DrawText (hdc, TEXT ("Hello, Windows 98!"), -1, &rect,
           //         DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

          DeleteDC ( hdcMem );
          EndPaint (hwnd1, &ps) ;
          return 0 ;

     case WM_TIMER:
         if ( wParam == TIMER_CANCEL )
             {
             tick2 = GetTickCount ();
             if ( ( ( tick2 - tick1 ) > 5 * 1000 ) & ( bIsPrankOn == TRUE ) )
                {
                SetWindowPos ( hwnd1, HWND_BOTTOM, 0, 0, cx, cy, SWP_HIDEWINDOW );
                tick1 = tick2;
                UnHookKBLL();
                bIsPrankOn = FALSE;
                }
             }
         break;

     case WM_LBUTTONDOWN:
         //SendMessage ( hwnd1, WM_DESTROY, 0, 0 );
         tick2 = GetTickCount ();
         if ( ( ( tick2 - tick1 ) > 5 * 1000 ) & ( bIsPrankOn == TRUE ) )
             {
             SetWindowPos ( hwnd1, HWND_BOTTOM, 0, 0, cx, cy, SWP_HIDEWINDOW );
             tick1 = tick2;
             UnHookKBLL();
             bIsPrankOn = FALSE;
             }
             
         break;

     case WM_KEYDOWN:
         tick2 = GetTickCount ();
         if ( ( ( tick2 - tick1 ) > 5 * 1000 ) & ( bIsPrankOn == TRUE ) )
             {
             SetWindowPos ( hwnd1, HWND_BOTTOM, 0, 0, cx, cy, SWP_HIDEWINDOW );
             tick1 = tick2;
             UnHookKBLL();
             bIsPrankOn = FALSE;
             }
         break;

          
     case WM_DESTROY:
         KillTimer ( hwnd1, TIMER_CANCEL );
          DestroyWindow ( hwnd1 );
          return 0 ;
     }
     return DefWindowProc (hwnd1, message, wParam, lParam) ;
}
