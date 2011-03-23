// Update.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
//#include "windows.h"
//#include "winuser.h"
#include "resource.h"
#include "update.h"



INT_PTR CALLBACK MainProc(HWND,UINT,WPARAM,LPARAM);
















int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL,MainProc);


	return 0;
}


INT_PTR CALLBACK MianProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return FALSE;
}
