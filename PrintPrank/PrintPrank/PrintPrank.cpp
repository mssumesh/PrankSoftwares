#define TIMER_PRANK 5

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "common.h"
//#include "hook.h"
#include "resource1.h"
		
static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HWND hStart = NULL;
static HINSTANCE hExe = NULL;

static HDC hdc, hdcPrint;
static HWND hwndNew = NULL;

DWORD DoPrank ( ProgData *pd );
static int cx, cy;

TCHAR title [ ] = TEXT ("Print Prank v 1.0");
TCHAR cls [] = TEXT ("PrintPrankV1");

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

            
    
            SetTimer ( hWnd, TIMER_PRANK, 3 * 60 * 1000, NULL );

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

    static TCHAR szPrinterName [ 2048 ];
    static DWORD len = 2048;

    DOCINFO di;
    TCHAR szTextStr [] = TEXT ("You've been pranked by PrankBoss | PrankBoss.com");
    TCHAR szDoc [] = TEXT("Prank Boss: Printing");

    static BITMAP bmp;
    static HBITMAP hbmp;
    static int xbmp, ybmp, xPage, yPage;
    static HDC hdcMem;

    GetDefaultPrinter ( szPrinterName, &len );

    //OpenPrinter( szPrinterName, &hPrinter, NULL);
    //HANDLE	hPrinter = NULL;
    //PRINTER_INFO_2	*pPrinterData;
    //BYTE	pdBuffer[16384];
    //DWORD	cbBuf = sizeof (pdBuffer);
    //DWORD	cbNeeded = 0;
    //pPrinterData = (PRINTER_INFO_2 *)&pdBuffer[0];
    //GetPrinter( hPrinter,2,&pdBuffer[0], cbBuf,&cbNeeded);
    //ClosePrinter(hPrinter);
    
    //hdcPrint = CreateDC(szDriver, szPrinterName, pPrinterData->pPortName, NULL); 
    hdcPrint = CreateDC ( NULL, szPrinterName, NULL, NULL );
    //hdcMem = CreateCompatibleDC ( hdcPrint );
    //hbmp = LoadBitmap ( hExe, MAKEINTRESOURCE(IDBMP_LOGO));
    //GetObject ( hbmp, sizeof(BITMAP), &bmp );
    //xbmp = bmp.bmWidth;
    //ybmp =bmp.bmHeight;
    xPage = GetDeviceCaps (hdcPrint, HORZRES) ;
    yPage = GetDeviceCaps (hdcPrint, VERTRES) ;
    //BitBlt ( hdcPrint, xPage - xbmp / 2, 10, xbmp, ybmp, hdcMem, 0, 0, SRCCOPY );


    SIZE size;
    GetTextExtentPoint32 (hdcPrint, szTextStr, lstrlen (szTextStr), &size) ;
    static int x, y, r;
    x = ( (  xPage - 20  ) - size.cx ) / 2;
    r = (  yPage - 20 ) / ( size.cy + 5 );    
    y = 10;

    ZeroMemory ( &di, sizeof ( DOCINFO ) );
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = szDoc;

    //StartDoc (hdcPrint, &di);
    //StartPage (hdcPrint);
    //SetStretchBltMode(hdcPrint,HALFTONE);
    //StretchBlt(hdcPrint, ( xPage - xbmp ) / 2, 10, xbmp, ybmp, hdcMem, 0, 0, xbmp, ybmp, SRCCOPY );    
    
    SaveDC (hdcPrint) ;
    Escape(hdcPrint, STARTDOC, lstrlen (szTextStr), szDoc, NULL); 
    //Escape(hdcPrint, PASSTHROUGH, lstrlen (szTextStr), szDoc, NULL); 
    //SetTextAlign (hdcPrint, TA_BASELINE | TA_CENTER | TA_UPDATECP) ;
    //BitBlt ( hdcPrint, ( xPage - xbmp ) / 2, 10, xbmp, ybmp, hdcMem, 0, 0, SRCCOPY );    
    //y = 10 + ybmp + 10;

    for ( volatile int i = 0; i < r; i++ )
        {
        TextOut(hdcPrint, x, y, szTextStr, lstrlen (szTextStr)); 
        y = y + size.cy + 5;        
        }
    Escape(hdcPrint, NEWFRAME, 0, NULL, NULL); 
    Escape(hdcPrint, ENDDOC, 0, NULL, NULL); 
    RestoreDC (hdcPrint, -1) ;
    
    //EndPage (hdcPrint);
    //EndDoc (hdcPrint);

    DeleteDC ( hdcPrint );
        
    return 0;
    }

//
//
//
//LRESULT CALLBACK WndProcNew (HWND hwnd1, UINT message, WPARAM wParam, LPARAM lParam)
//{
//     HDC         hdc ;
//     PAINTSTRUCT ps ;
//     RECT        rect ;
//     static BITMAP bmp;
//     static HBITMAP hBmp;
//     static int xbmp, ybmp;
//     static HDC hdcMem;
//     
//     switch (message)
//     {
//     case WM_CREATE:
//          //PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
//          
//         hBmp = LoadBitmap ( pd.hInst, MAKEINTRESOURCE (IDBMP_POPUP));
//         GetObject (hBmp, sizeof (BITMAP), &bmp );
//         xbmp = bmp.bmWidth;
//         ybmp = bmp.bmHeight;
//
//         return 0 ;
//
//     case WM_PAINT:
//          hdc = BeginPaint (hwnd1, &ps) ;
//          hdcMem = CreateCompatibleDC ( hdc );
//          SelectObject ( hdcMem, hBmp );
//          
//          GetClientRect (hwnd1, &rect) ;
//          BitBlt ( hdc, ( rect.right - xbmp ) / 2, (rect.bottom - ybmp ) / 2, xbmp, ybmp, hdcMem, 0, 0, SRCCOPY );
//          
//          //DrawText (hdc, TEXT ("Hello, Windows 98!"), -1, &rect,
//           //         DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
//
//          DeleteDC ( hdcMem );
//          EndPaint (hwnd1, &ps) ;
//          return 0 ;
//
//     case WM_LBUTTONDOWN:
//         //SendMessage ( hwnd1, WM_DESTROY, 0, 0 );
//         SetWindowPos ( hwnd1, HWND_BOTTOM, 0, 0, cx, cy, SWP_HIDEWINDOW );
//         break;
//          
//     case WM_DESTROY:
//          DestroyWindow ( hwnd1 );
//          return 0 ;
//     }
//     return DefWindowProc (hwnd1, message, wParam, lParam) ;
//}
