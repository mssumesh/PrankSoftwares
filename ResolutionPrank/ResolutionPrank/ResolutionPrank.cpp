#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shlobj.h>
#include "common.h"
#include "resource.h"

#define TIMER_EJECT 1

static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HINSTANCE hExe = NULL;

DWORD DoPrank ( ProgData *pd );
DWORD UndoPrank ();

TCHAR title [ ] = TEXT ("Resolution Prank v 1.0");
TCHAR cls [] = TEXT ("ResolutionPrankV1");



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
                SetTimer ( hWnd, TIMER_EJECT, 3 * 60 * 1000, NULL );
                SendMessage ( hWnd, WM_TIMER, (WPARAM)TIMER_EJECT, 0);
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
            if ( wParam == TIMER_EJECT )
                DoPrank ( &pd );
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
            KillTimer ( hWnd, TIMER_EJECT );
            UndoPrank();
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
   
    DEVMODE dm;
    // initialize the DEVMODE structure
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    DWORD hor, ver;

    if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
        {
        //dm.dmDisplayOrientation = 2;
        hor = dm.dmPelsWidth;
        ver = dm.dmPelsHeight;

        dm.dmPelsHeight = 480 ;
        dm.dmPelsWidth = 640 ;
        dm.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH;
           
        long lRet = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
        }
                //long lRet = ChangeDisplaySettings(NULL,0);


    TCHAR keyval [] = TEXT("NoDispCPL");
    DWORD val = 1;
    HKEY hKey;
	DWORD dwDisposition;

	RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
    RegCreateKeyExA ( hKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );
    RegSetValueExA ( hKey, keyval, 0, REG_DWORD, (BYTE *)&val, sizeof ( DWORD ) );
    //RegDeleteValueA ( hKey, keyval );
	RegCloseKey ( hKey );

    RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
    RegCreateKeyExA ( hKey, "Software\\PrankBoss\\Resolution", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );
    lstrcpy ( keyval, TEXT("h"));
    RegSetValueExA ( hKey, keyval, 0, REG_DWORD, (BYTE *)&hor, sizeof ( DWORD ) );
    lstrcpy ( keyval, TEXT("v"));
    RegSetValueExA ( hKey, keyval, 0, REG_DWORD, (BYTE *)&ver, sizeof ( DWORD ) );
	RegCloseKey ( hKey );
    	
//    [HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System]
//"NoDispCPL"=dword:00000001

    return 0;
    }


DWORD UndoPrank ()
    {

    TCHAR keyval [] = TEXT("NoDispCPL");
    DWORD val = 1;
    HKEY hKey;
	DWORD dwDisposition;
    BOOL flag = TRUE;

	RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
    RegCreateKeyExA ( hKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );
    RegDeleteValueA ( hKey, keyval );
	RegCloseKey ( hKey );


    DWORD hor, ver, len, type, ret;
    type = REG_DWORD;
    len= sizeof(DWORD);
    RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
    RegCreateKeyExA ( hKey, "Software\\PrankBoss\\Resolution", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );    
    if (RegQueryValueExA ( hKey, "h",0, &type,(BYTE *)&hor, &len ))
        flag = FALSE;
    type = REG_DWORD;
    len= sizeof(DWORD);
    if (RegQueryValueExA (hKey,"v",0,&type,(BYTE *)&ver, &len) )
        flag = FALSE;
    RegCloseKey ( hKey );
    


    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);


    if ( flag != FALSE )
    
        {
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
        dm.dmPelsWidth = hor;
        dm.dmPelsHeight = ver;
        dm.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH;
        ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
        }
    
    
    return 0;
    }


