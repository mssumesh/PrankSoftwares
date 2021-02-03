
#ifndef COMMON_H_DEFINED
#define COMMON_H_DEFINED


#pragma comment (lib, "shlwapi.lib");
#pragma comment (lib, "shell32.lib");




#define PRANK_INTERVAL 3 // repeat prank in 3 minutes. 
//Change this value to change the interval of prank repetition


#define WIN32_LEAN_AND_MEAN		

#define WM_USRSYSTRAY WM_USER + 1

//#define IDM_EXIT 1
//#define IDM_STOP 2
//#define IDM_START 3
#define IDM_ABOUT 0
#define IDM_PBOSS 2


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
    HCURSOR hCurs;
    };

ATOM				MyRegisterClass(HINSTANCE hInstance, ProgData *pd );
BOOL				InitInstance(HINSTANCE hInstance, int nCmdShow, ProgData *pd  );
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc ( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );

DWORD SetAutoStart ( BOOL bAutoON, ProgData *pd );
BOOL IsSingleInstance ( ProgData *pd );
DWORD AddSystrayIcon ( ProgData *pd );
DWORD DelSystrayIcon ( ProgData *pd );
DWORD Initialize ( ProgData *pd );
DWORD CleanUp ( ProgData *pd );
DWORD ShowPopupMenu ( ProgData *pd );
DWORD ShowAboutDialog ( ProgData *pd );
DWORD CreateShortCut ( ProgData *pd );
DWORD GotoPrankBoss ( ProgData *pd );

#endif