#ifndef _HOOK_INCLUDED
#define _HOOK_INCLUDED

DWORD HookKBLL ( void );
DWORD UnHookKBLL ( void );
__declspec(dllexport) LRESULT CALLBACK HookProc ( int nCode,    WPARAM wParam,  LPARAM lParam);


#endif
