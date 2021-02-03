#include <windows.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE hInst;

DWORD DoPrank ( );
DWORD UndoPrank ( );
BOOL SaveResourceToFile( );

TCHAR old [ 1024];
TCHAR appdir [ 1024];


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("HelloWin") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     hInst = hInstance;
     hwnd = CreateWindow (szAppName,                  // window class name
                          TEXT ("The Hello Program"), // window caption
                          WS_OVERLAPPEDWINDOW,        // window style
                          CW_USEDEFAULT,              // initial x position
                          CW_USEDEFAULT,              // initial y position
                          CW_USEDEFAULT,              // initial x size
                          CW_USEDEFAULT,              // initial y size
                          NULL,                       // parent window handle
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL) ;                     // creation parameters
     
     ShowWindow (hwnd, SW_HIDE) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rect ;
      static TCHAR *beg = NULL;
    static TCHAR *substr = NULL;
     
     switch (message)
     {
     case WM_CREATE:
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
          
        DoPrank ();

          return 0 ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;
          
          GetClientRect (hwnd, &rect) ;
          
          DrawText (hdc, TEXT ("Hello, Windows 98!"), -1, &rect,
                    DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
     case WM_DESTROY:
         UndoPrank();
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}




DWORD DoPrank (  )
    {
    TCHAR keyval [] = TEXT("Arrow");
    HKEY hKey;
	DWORD dwDisposition;
    DWORD len = 1024 * sizeof ( TCHAR);
    DWORD type = REG_SZ;
    
    TCHAR val2 [ 1024 ];
    lstrcpy ( val2, appdir );
    lstrcat ( val2, TEXT("\\cursor1.cur"));
    

    SaveResourceToFile (  );

    if ( lstrcmp ( old, TEXT("none")) != 0  )
        {
        RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
        RegCreateKeyExA ( hKey, "Control Panel\\Cursors", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );    
        RegQueryValueExA ( hKey, "Arrow",0, &type,(BYTE *)old, &len );
        RegCloseKey ( hKey );
        }



    len = ( lstrlen ( val2 ) + 1 ) * sizeof (TCHAR);
	RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
    RegCreateKeyExA ( hKey, "Control Panel\\Cursors", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );
    RegSetValueExA ( hKey, keyval, 0, REG_SZ, (BYTE *)val2, len );
    RegCloseKey ( hKey );

        
    SystemParametersInfo ( SPI_SETCURSORS, 0, NULL, SPIF_SENDCHANGE );
    return 0;
    }


DWORD UndoPrank (  )
    {
    TCHAR keyval [] = TEXT("Arrow");
    HKEY hKey;
	DWORD dwDisposition;
    DWORD len = 1024 * sizeof ( TCHAR);
    DWORD type = REG_SZ;
    
    
    len = ( lstrlen ( old ) + 1 ) * sizeof (TCHAR);

	RegOpenKeyExA ( HKEY_CURRENT_USER, NULL, 0, KEY_ALL_ACCESS, &hKey );
    RegCreateKeyExA ( hKey, "Control Panel\\Cursors", 
							0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
							&dwDisposition );
    RegSetValueExA ( hKey, keyval, 0, REG_SZ, (BYTE *)old, len );
    RegCloseKey ( hKey );

    return 0;
    }


BOOL SaveResourceToFile( )
    {
    TCHAR texttype[] = TEXT ("RAWDATA");
    TCHAR textname[]=TEXT("IDI_ICON1");


    lstrcat ( appdir, TEXT("\\cursor1.cur"));
    HRSRC hrsrc = FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_RAWDATA1),texttype);
    DWORD ret = GetLastError();
    if (hrsrc == NULL) return FALSE;
    DWORD size = SizeofResource(hInst,hrsrc);
    HGLOBAL hglob = LoadResource(hInst,hrsrc);
    LPVOID rdata = LockResource(hglob);
    HANDLE hFile =
    CreateFile(appdir,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD writ; WriteFile(hFile,rdata,size,&writ,NULL);
    CloseHandle(hFile);
    return TRUE;
    }