#define TIMER_PRANK 5

#define TIMER_CANCEL 6
#define TIMER_RAINCHECK 7

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "common.h"
#include "hook.h"
#include "resource.h"
		
static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HINSTANCE hExe = NULL;



DWORD DoPrank ( ProgData *pd );
DWORD UndoPrank (ProgData *pd );

static int cx, cy;
static DWORD tick1, tick2;
static HWND hwndmy;

 static HDC hdcmem, hdcscr, hdcmy;
 static HBITMAP hbmp;

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

LRESULT CALLBACK WndProcNew(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {

    PAINTSTRUCT ps;
    if ( message == WM_PAINT )
        {
        hdcmy = BeginPaint ( hWnd, &ps );
        BitBlt ( hdcmy, 0, 0, cx, cy, hdcmem, 0, 0, NOTSRCCOPY );
        EndPaint ( hWnd, &ps );
        }
    return DefWindowProc ( hWnd, message, wParam, lParam );
    }

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {

    HWND hwnddesk;
    
    
	switch (message)
	    {
        case WM_CREATE:
            pd.hDlg = hWnd;
            pd.hCurs = NULL;
            Initialize ( &pd );

            cx = GetSystemMetrics ( SM_CXSCREEN );
            cy = GetSystemMetrics ( SM_CYSCREEN );
            //cx = cx + 16;
            //cy = cy + 16;

           
            hwnddesk = GetDesktopWindow();
            
            SetTimer ( hWnd, TIMER_PRANK, 2 * 60 * 1000, NULL );
            SetTimer ( hWnd, TIMER_RAINCHECK, 2 * 1000, NULL );

            hdcscr  = GetDCEx (hwnddesk, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE) ;
            hdcmem = CreateCompatibleDC ( hdcscr );
            hbmp = CreateCompatibleBitmap ( hdcscr, cx, cy );
            SelectObject ( hdcmem, hbmp );
            BitBlt ( hdcmem, 0, 0, cx, cy, hdcscr, 0, 0 , SRCCOPY );
            ReleaseDC ( hwnddesk, hdcscr );

            WNDCLASS     wndclass ;
            wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
            wndclass.lpfnWndProc   = WndProcNew ;
            wndclass.cbClsExtra    = 0 ;
            wndclass.cbWndExtra    = 0 ;
            wndclass.hInstance     = hExe;
            wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
            wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
            wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
            wndclass.lpszMenuName  = NULL ;
            wndclass.lpszClassName = TEXT("scrninvertv1") ;

            RegisterClass (&wndclass);
            
            hwndmy = CreateWindowEx ( WS_EX_TOPMOST,
                                TEXT("scrninvertv1"),                  // window class name
                              TEXT("Prankboss.com"), // window caption
                              WS_POPUP,         // window style
                              0,              // initial x position
                              0,              // initial y position
                              cx,              // initial x size
                              cy,              // initial y size
                              //300,300,100,100,
                              NULL,                       // parent window handle
                              NULL,                       // window menu handle
                              hExe,                  // program instance handle
                              NULL) ;                     // creation parameters

            hdcmy = GetWindowDC (hwndmy );
            BitBlt ( hdcmy, 0, 0, cx, cy, hdcmem, 0,0, NOTSRCCOPY );
         
            UpdateWindow (hwndmy) ;
            ShowWindow (hwndmy, SW_HIDE) ;
            
            SendMessage( hWnd, WM_TIMER, TIMER_PRANK, 0 );

            break;
        case WM_TIMER:
            if ( wParam == TIMER_PRANK )
                {
                DoPrank ( &pd );
                }
            else if ( wParam == TIMER_RAINCHECK )
                {
                tick2 = GetTickCount ();
                if ( ( ( tick2 - tick1 ) >= 7000 ) && ( bIsPrankOn == TRUE ) )
                    {
                    UndoPrank (&pd);
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
    //HIGHCONTRAST hc;
    //hc.cbSize = sizeof ( HIGHCONTRAST);
    //hc.dwFlags = HCF_HIGHCONTRASTON;

    if ( bIsPrankOn== FALSE )
        {
        HookKBLL();
        SetWindowPos ( hwndmy, HWND_TOPMOST, 0, 0, cx, cy, SWP_SHOWWINDOW );
        //SystemParametersInfo ( SPI_GETHIGHCONTRAST,sizeof(HIGHCONTRAST), &hc, SPIF_SENDCHANGE);
        //SystemParametersInfo ( SPI_SETHIGHCONTRAST,sizeof(HIGHCONTRAST), &hc, SPIF_SENDCHANGE);
        
        bIsPrankOn= TRUE;
        tick1 = GetTickCount ();
        }
    return 0;
    }




DWORD UndoPrank (ProgData *pd )
    {
     //HIGHCONTRAST hc;
    //hc.cbSize = sizeof ( HIGHCONTRAST);
    //hc.dwFlags = 0;

    if ( bIsPrankOn == TRUE )
        {
        bIsPrankOn= FALSE;
        UnHookKBLL ();
        //ShowWindow ( hwndmy, SW_HIDE );

        //SystemParametersInfo ( SPI_GETHIGHCONTRAST,sizeof(HIGHCONTRAST), &hc, SPIF_SENDCHANGE);
        //SystemParametersInfo ( SPI_SETHIGHCONTRAST,sizeof(HIGHCONTRAST), &hc, SPIF_SENDCHANGE);
        tick1 = 0;
        tick2 = 0;
        }
    return 0;
    }



