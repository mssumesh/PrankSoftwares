#include <windows.h>
#include <tchar.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include "common.h"
#include "resource.h"

static BOOL IsAboutOn = FALSE;

ATOM MyRegisterClass(HINSTANCE hInstance, ProgData *pd )
    {
	WNDCLASSEX wcex;
    ZeroMemory ( &wcex, sizeof ( WNDCLASSEX));

	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, IDI_APPLICATION );//MAKEINTRESOURCE(IDI_BIG));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_RESPAWN);
	//wcex.lpszClassName	= TEXT("invertscreenv1");
    wcex.lpszClassName = pd->cls;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    //wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    pd->hInst = hInstance;

	return RegisterClassEx(&wcex);
    }


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, ProgData *pd)
    {
    HWND hWnd;
    hWnd = CreateWindow( pd->cls, pd->title, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);

    return TRUE;
    }




DWORD SetAutoStart ( BOOL bAutoON, ProgData *pd )
	{
	static char szPath [ 2048 ];
    static TCHAR keyval [ 1024 ];


	static HKEY hKey;
	static DWORD dwDisposition;
	GetModuleFileNameA ( NULL, szPath, 2048 );
    lstrcpy ( keyval, pd->cls );

	if ( ERROR_SUCCESS == RegOpenKeyExA ( HKEY_CURRENT_USER, 
											NULL, 0, KEY_ALL_ACCESS, &hKey ) )
		{
		RegCreateKeyExA ( hKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );
		if ( bAutoON == TRUE )
			RegSetValueExA ( hKey, keyval, 0, REG_SZ, (BYTE *)szPath, 
							(lstrlenA ( szPath ) + 1 ) * sizeof ( char ) );
		else
			RegDeleteValueA ( hKey, keyval );
	
		RegCloseKey ( hKey );
		}

	return 1;
	}

BOOL IsSingleInstance ( ProgData *pd )
    {
    pd->hMutex = NULL;
	pd->hMutex = CreateMutex(NULL, TRUE, pd->cls);
	if( pd->hMutex != NULL )
		{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
			{
			return FALSE;
			}
		}
    return TRUE;
    }


DWORD AddSystrayIcon ( ProgData *pd )
	{
	
	NOTIFYICONDATA nid;
	ZeroMemory ( &nid, sizeof ( NOTIFYICONDATA ) );

	nid.cbSize = sizeof ( NOTIFYICONDATA );
	nid.hIcon = pd->hSmall;
	nid.uCallbackMessage = WM_USRSYSTRAY;
	lstrcpy ( nid.szTip, pd->title );
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uID = IDI_SMALL;
	nid.hWnd = pd->hDlg;

	Shell_NotifyIcon ( NIM_ADD, &nid );

    return 0;
	}

DWORD DelSystrayIcon ( ProgData *pd )
	{

	NOTIFYICONDATA  nid;
    ZeroMemory ( &nid, sizeof ( NOTIFYICONDATA ) );

    nid.cbSize = sizeof ( NOTIFYICONDATA );
	nid.hIcon = pd->hSmall;
	nid.uCallbackMessage = WM_USRSYSTRAY;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uID = IDI_SMALL;
	nid.hWnd = pd->hDlg;

    Shell_NotifyIcon( NIM_DELETE, &nid );
    
    return 0;
	}


DWORD Initialize ( ProgData *pd )
    {
    if ( !IsSingleInstance ( pd ) )
        SendMessage ( pd->hDlg, WM_CLOSE, 0, 0 );

    CreateShortCut ( pd );
    pd->hSmall = (HICON) LoadImage ( pd->hInst, MAKEINTRESOURCE ( IDI_SMALL ),
										IMAGE_ICON, NULL, NULL, LR_DEFAULTSIZE );
	pd->hBig = (HICON) LoadImage ( pd->hInst, MAKEINTRESOURCE ( IDI_BIG ),
										IMAGE_ICON, NULL, NULL, LR_DEFAULTSIZE );
	AddSystrayIcon ( pd );

    SetAutoStart ( TRUE, pd );

    return 0;
    }
DWORD CleanUp ( ProgData *pd )
    {

    if ( pd->hMutex != NULL )
        {
        ReleaseMutex ( pd->hMutex );
        CloseHandle ( pd->hMutex );
        }
    
    DelSystrayIcon ( pd );


    DestroyIcon ( pd->hBig );
    DestroyIcon ( pd->hSmall );
    DestroyMenu ( pd->hMenu );

    return 0;
    }

DWORD ShowPopupMenu ( ProgData *pd ) 
	{
	POINT pt;
	
	pd->hMenu = CreatePopupMenu ( );
    
	InsertMenu( pd->hMenu, 0, MF_BYPOSITION | MF_STRING, IDM_ABOUT, TEXT("About" ) );
	//InsertMenu( pd->hMenu, 1, MF_BYPOSITION | MF_STRING, IDM_START, TEXT("PrankBoss.com" ) );
	//InsertMenu( pd->hMenu, 2, MF_BYPOSITION | MF_STRING, IDM_STOP, TEXT("Stop" ) );
	InsertMenu( pd->hMenu, 1, MF_SEPARATOR, 0, NULL );
    InsertMenu( pd->hMenu, 2, MF_BYPOSITION | MF_STRING, IDM_PBOSS, TEXT("PrankBoss.com" ) );
	//InsertMenu( pd->hMenu, 4, MF_BYPOSITION | MF_STRING, IDM_EXIT, TEXT("Exit" ) );
	
	SetMenuDefaultItem( pd->hMenu, IDM_ABOUT, FALSE );
    GetCursorPos ( &pt );
	TrackPopupMenu( pd->hMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, pd->hDlg, NULL );
    
    return 0;
    }

DWORD ShowAboutDialog ( ProgData *pd )
    {
    if ( IsAboutOn == FALSE )
		{
        IsAboutOn = TRUE;
		DialogBox ( pd->hInst, MAKEINTRESOURCE(IDD_ABOUT), 0, DlgProc );
        
        }
    return 0;
    }

BOOL CALLBACK DlgProc ( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
	{
    TCHAR stc [ 256 ];
    SYSTEMTIME st;
    TCHAR temp [ 8 ];

	switch ( message )
		{
		case WM_INITDIALOG:
            lstrcpy ( stc, TEXT("Copyright© ["));
            GetLocalTime ( &st );
            ltoa ( st.wYear, temp, 10 );            
            lstrcat ( stc, temp );
            lstrcat ( stc, TEXT("] PrankBoss.com"));
            SetDlgItemText ( hDlg, IDS_CRIGHT, stc );
			return TRUE;

		case WM_COMMAND:
			switch (wParam)
				{
                case IDB_OK:
                    SendMessage ( hDlg, WM_CLOSE, 0, 0 );
                    break;
                }
            return TRUE;
            
		case WM_CLOSE:
            IsAboutOn = FALSE;
            EndDialog ( hDlg, 0 );
			return TRUE;
		}
	return FALSE;
	}



DWORD CreateShortCut ( ProgData *pd )
    {

    TCHAR path [ 2048 ];
    TCHAR buff [ 2048 ];
    

    DWORD len = 0;
    DWORD dwTemp = 0;
    lstrcpy ( buff, TEXT ("[InternetShortcut]\x0d\x0aURL=http://www.prankboss.com/\x0d\x0aIconIndex=2\x0d\x0aIconFile=") );
    GetModuleFileName ( NULL, path, 2048 );
    lstrcat ( buff, path );

    if ( S_OK == ( SHGetFolderPath ( NULL, CSIDL_DESKTOP, NULL, SHGFP_TYPE_CURRENT, path ) ))
        {
        PathAppend ( path, TEXT("Undo Prank.url") );

        HANDLE hFile = CreateFile ( path, GENERIC_READ | GENERIC_WRITE, 
		    		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			    	FILE_ATTRIBUTE_NORMAL, NULL );
	    if ( hFile != INVALID_HANDLE_VALUE )
		    {
		    len = lstrlen ( buff )  * sizeof ( TCHAR );
		    WriteFile ( hFile, (PVOID)buff, len, &dwTemp, NULL );
		    CloseHandle ( hFile );
		    }
        }

    return 0;
    }

DWORD GotoPrankBoss ( ProgData *pd )
    {
    TCHAR url [] = TEXT ("http://www.prankboss.com");

    ShellExecute ( pd->hDlg, TEXT ("open"), url, NULL, NULL, SW_MAXIMIZE );

    return 0;
    }