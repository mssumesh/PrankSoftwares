#define TIMER_PRANK 5

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

DWORD DoPrank ( ProgData *pd );

TCHAR title [ ] = TEXT ("Key Press v 1.0");
TCHAR cls [] = TEXT ("KeyPressV1");


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

            HookKBLL ();
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
            UnHookKBLL ();
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }



DWORD DoPrank ( ProgData *pd )
    {
    
        
    return 0;
    }



