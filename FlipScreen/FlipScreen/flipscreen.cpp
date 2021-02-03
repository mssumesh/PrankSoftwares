#include <windows.h>
#include <tchar.h>
#include "common.h"
#include "resource.h"
		
#define TIMER_DO_PRANK 1

static ProgData pd;
static BOOL bIsPrankOn = FALSE;

DWORD DoPrank ( ProgData *pd );
DWORD ReversePrank ( ProgData *pd );

TCHAR title [ ] = TEXT ("Flip Screen v 1.0");
TCHAR cls [] = TEXT ("FlipScreenV1");


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
    {
	MSG msg;
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

	switch (message)
	    {
        case WM_CREATE:
            pd.hDlg = hWnd;
            Initialize ( &pd );
            //SendMessage ( hWnd, WM_COMMAND, (WPARAM)IDM_START, 0 );
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
                //case IDM_EXIT:
                //    //SendMessage ( hWnd, WM_COMMAND, (WPARAM)IDM_STOP, 0 );
                //    SendMessage ( hWnd, WM_CLOSE, 0, 0 );
                //    break;
                case IDM_ABOUT:
                    ShowAboutDialog ( &pd );
                    break;
                case IDM_PBOSS:
                    GotoPrankBoss ( &pd );
                    break;
                //case IDM_START:
                //    if ( bIsPrankOn == FALSE )
                //        {
                //        DoPrank ( &pd );
                //        bIsPrankOn = TRUE;
                //        }
                //    break;
                //case IDM_STOP:
                //    if ( bIsPrankOn == TRUE )
                //        {
                //        ReversePrank ( &pd );
                //        bIsPrankOn = FALSE;
                //        }
                //    break;
                }
            break;

	    case WM_DESTROY:
            CleanUp ( &pd );
		    PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }



DWORD DoPrank ( ProgData *pd )
    {

    TCHAR szMsg [ 4096 ];
    DEVMODE dm;
    // initialize the DEVMODE structure
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);

    if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
        {
        dm.dmDisplayOrientation = DMDO_180;
           
        long lRet = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
        //if (DISP_CHANGE_SUCCESSFUL != lRet)
        //    {
        //    switch ( lRet )
        //        {
        //       // case DISP_CHANGE_BADDUALVIEW:
        //         //   lstrcpy ( szMsg, TEXT("The settings change was unsuccessful because the system is DualView capable."));
        //           // break;
        //        case DISP_CHANGE_BADFLAGS:	
        //            lstrcpy ( szMsg, TEXT("An invalid set of flags was passed in."));
        //            break;
        //        case DISP_CHANGE_BADMODE:
        //            lstrcpy ( szMsg, TEXT("The graphics mode is not supported."));
        //            break;
        //        case DISP_CHANGE_BADPARAM:
        //            lstrcpy ( szMsg, TEXT("An invalid parameter was passed in. This can include an invalid flag or combination of flags."));
        //            break;
        //        case DISP_CHANGE_FAILED:
        //            lstrcpy ( szMsg, TEXT("The display driver failed the specified graphics mode."));
        //            break;
        //        case DISP_CHANGE_NOTUPDATED:
        //            lstrcpy ( szMsg, TEXT("Unable to write settings to the registry."));
        //            break;
        //        case DISP_CHANGE_RESTART:
        //            lstrcpy ( szMsg, TEXT("The computer must be restarted for the graphics mode to work."));
        //            break;
        //        }
        //    MessageBox ( pd->hDlg, szMsg, TEXT("Error"), MB_OK );
        //    }
        }
    return 0;
    }
//
//DWORD ReversePrank ( ProgData *pd )
//    {
//    DEVMODE dm;
//    TCHAR szMsg [ 4096 ];
//    // initialize the DEVMODE structure
//    ZeroMemory(&dm, sizeof(dm));
//    dm.dmSize = sizeof(dm);
//
//    if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
//        {
//        dm.dmDisplayOrientation = DMDO_DEFAULT;
//           
//        long lRet = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
//        //if (DISP_CHANGE_SUCCESSFUL != lRet)
//        //    {
//        //    switch ( lRet )
//        //        {
//        //       // case DISP_CHANGE_BADDUALVIEW:
//        //         //   lstrcpy ( szMsg, TEXT("The settings change was unsuccessful because the system is DualView capable."));
//        //           // break;
//        //        case DISP_CHANGE_BADFLAGS:	
//        //            lstrcpy ( szMsg, TEXT("An invalid set of flags was passed in."));
//        //            break;
//        //        case DISP_CHANGE_BADMODE:
//        //            lstrcpy ( szMsg, TEXT("The graphics mode is not supported."));
//        //            break;
//        //        case DISP_CHANGE_BADPARAM:
//        //            lstrcpy ( szMsg, TEXT("An invalid parameter was passed in. This can include an invalid flag or combination of flags."));
//        //            break;
//        //        case DISP_CHANGE_FAILED:
//        //            lstrcpy ( szMsg, TEXT("The display driver failed the specified graphics mode."));
//        //            break;
//        //        case DISP_CHANGE_NOTUPDATED:
//        //            lstrcpy ( szMsg, TEXT("Unable to write settings to the registry."));
//        //            break;
//        //        case DISP_CHANGE_RESTART:
//        //            lstrcpy ( szMsg, TEXT("The computer must be restarted for the graphics mode to work."));
//        //            break;
//        //        }
//        //    MessageBox ( pd->hDlg, szMsg, TEXT("Error"), MB_OK );
//        //    }
//        }
//    
//    return 0;
//    }