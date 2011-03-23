




#include <windows.h>
#include <exdisp.h>		// Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above
#include <mshtml.h>		// Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above
#include <crtdbg.h>		// for _ASSERT()


long __cdecl DisplayHTMLPage(HWND hwnd, wchar_t* webPageName);
long __cdecl EmbedBrowserObject(HWND hwnd);
long __cdecl SetBrowserRect(HWND hwnd,HWND * pWnd);
