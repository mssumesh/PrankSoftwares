#include <windows.h>
#include <tchar.h>
#include "resource.h"
		

struct ProgData
    {
    TCHAR cls [ 32 ];
    TCHAR title [ 32 ];
    HANDLE hMutex;
    HWND hDlg;
    HICON hSmall;
    HICON hBig;
    HINSTANCE hInst;
    HMENU hMenu;
    };

ATOM				MyRegisterClass(HINSTANCE hInstance, ProgData *pd );
BOOL				InitInstance(HINSTANCE hInstance, int nCmdShow, ProgData *pd  );
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

static ProgData pd;

DWORD ReversePrank ( ProgData *pd );
DWORD SetAutoStart ( BOOL bAutoON, ProgData *pd );

TCHAR title [ ] = TEXT ("ShutDown Prank Uninstall v 1.0");
TCHAR cls [] = TEXT ("ShutDownUninstV1");
TCHAR cls1 [] = TEXT ("ShutDownV1");


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

    HWND hFlip;

	switch (message)
	    {
        case WM_CREATE:
            hFlip = FindWindow ( cls1, NULL );
            if ( hFlip != NULL )
                SendMessage ( hFlip, WM_DESTROY, 0, 0 );
            //ReversePrank ( &pd );
            lstrcpy ( pd.cls, cls1 );
            SetAutoStart ( FALSE, &pd );
            MessageBox ( pd.hDlg, TEXT ("ShutDown Prank Successfully Uninstalled!"), TEXT("Uninstall Complete!"), MB_OK | MB_ICONINFORMATION );
            SendMessage ( hWnd, WM_DESTROY, 0, 0 );
            break;
        
	    case WM_DESTROY:
		    PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
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
//        if ( dm.dmDisplayOrientation == DMDO_180 )
//            dm.dmDisplayOrientation = DMDO_DEFAULT;
//           
//        long lRet = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
//        }
//    
//    return 0;
//    }


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
	char szPath [ 2048 ];
    TCHAR keyval [ 1024 ];


	HKEY hKey;
	DWORD dwDisposition;
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

