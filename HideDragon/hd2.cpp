
#define _WIN32_WINNT 0x0501
#include "hd.h"
//#include "../hooklib.h"
#include "resource.h"
#include "windowsx.h"
#include "excpt.h"
#include <commctrl.h>
#include <stdio.h>
#include <winnls.h>
#include <process.h>
#include <stdlib.h>
#include "unrar.h"
#include <wchar.h>
#include "DBClass.h"
#include "tlhelp32.h"
#include "HyperLink.h"
#include "ClearDlg.h"
#include "WebDB.h"
#include "UseProxyDlg.h"
#include <time.h>
#include <ShlObj.h>
#include "PictureEx.h"













extern HINSTANCE hInst;
extern HWND hMainDialog,hUsageTip,hListView,hList_BK,g_Tab;
extern OptionFile sOP;
extern int iExtWindow;
extern HMENU hTrayMenu;
extern int g_iLangCount;
extern HMENU g_hLangMenu;
extern UINT g_CurrLangID;
extern UINT g_OldLangID;
extern int TrayFlag;
extern HMENU hWLMenu;
extern int g_iUseLanguage;
extern HANDLE g_hUpdateHandle;
extern HANDLE g_hUpdateLangHandle;
extern HANDLE hNetworkThread;
extern HANDLE g_ChangeIconTitleHandle;
extern HANDLE g_AutoHideHandle;
extern int g_iTipCount;
extern wchar_t * g_pTipText[10][10];
extern WndAndHandle * sBKSel,* sW;
extern int iBaSW;
extern HIMAGELIST hImage_BK;
extern HICON hIcon_NoIcon;
extern UINT g_HD_Msg_GetIcon;
extern int iW_Sel;
extern HFONT hFont_New;
extern LOGFONT lf;
extern HWND g_TabWnd[10];
extern int g_iTabCount;
extern HWND g_hInfoPanel;
extern WndAndHandle * g_QuickHideAllTray;
extern CHyperLink m_lnk_adv;
extern CHyperLink m_lnk_hideip;
extern HICON hI1;
extern HCURSOR hHand;
extern HCURSOR hNorm;
extern URL_Array urlArray;





HMODULE hUser32;
typedef BOOL (WINAPI  GETLWA)(HWND,COLORREF *,BYTE *,DWORD *);
GETLWA * pGetLayeredWindowAttributes = NULL;







//Draw title and frame variables
int iMoveIn,iPressIn,iUpIn;

int g_oleemb = 0;


//MultiLanguage variables
WIN32_FIND_DATAW FFD;

//Backgound window hotkey
HWND hHK_BkWnd;
WORD wBKWnd;
UINT uModifier_BK = 0, uMF_BK = 0, uVK_BK = 0;
wchar_t cPID_BK[20];
sBKWHotkey * pBHHeader;

BKHList g_BKHList;



HMENU hTrayListMenu = NULL,hTrayListMenu_Sub = NULL;
HMENU hBKListMenu = NULL;
HMENU hBKListMenu_Sub = NULL;
HMENU hBKListMenu_Wizard = NULL,hBKListMenu_WizardSub = NULL;
HMENU hProcessListMenu,hProcessMenu_Sub;
HMENU hAutoHideListMenu,hAutoHideListMenu_Sub;





sIconTitle * pITHeader,* pIconHeader;


//Change Title and Icon
ITList g_ITList;
ITList g_ICList;

UINT_PTR g_uCheckTI = NULL;

UINT_PTR g_SetAdsTimer = NULL;
UINT_PTR g_SetWndAdsTimer = NULL;
UINT_PTR  g_ReConnTimer = NULL;



HIMAGELIST g_ImageList_Tab;



int g_ExitChangeIconThread = 0;


WndAndHandle * pTempBK = NULL;

HWND hBKDialog,hTitleDialog,hPropertyDialog, g_BKTipDialog;
int iShowFileDialog;
HWND g_hStatus_Link = NULL;



HWND g_AutoHideList;
HANDLE g_hOpenFileHandle = NULL;


HANDLE g_GetAdsLinkHandle;



extern HWND g_TrayList;
extern HWND g_ProcessList;






extern int WriteOptionFile(OptionFile);
extern int GetFileFromServer(char * cFileName);
extern VOID CALLBACK SetTipTextProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
extern int HideSelected(WndAndHandle **pWH,int iSize);
extern int ExtendWindow();
extern int ShowSelected(WndAndHandle * pWH,int iSize);
extern int iTakeToBk;
extern int g_UseIcon;












int ShowBackTipDialog()
{
	if(g_BKTipDialog == NULL /*&& IsWindow(g_BKTipDialog)*/)
	{
		DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_BACKTIP),NULL,(DLGPROC)BackTipDialog);
	}


	return 1;
}

int CloseBKTipDialog()
{
	if(g_BKTipDialog && IsWindow(g_BKTipDialog))
	{
		EndDialog(g_BKTipDialog,FALSE);
		g_BKTipDialog = NULL;
	}
	return 1;
}


HWND hHotKey_TIPSS;


INT_PTR CALLBACK BackTipDialog(HWND hwndDlg,UINT uMsg ,WPARAM wParam,LPARAM lParam)
{
	int i = 0;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		g_BKTipDialog = hwndDlg;
		hHotKey_TIPSS = GetDlgItem(hwndDlg,IDC_HOTKEY_TIPSS);
		SetWindowPos(hwndDlg,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		SendDlgItemMessage(hwndDlg,IDC_HOTKEY_TIPSS,HKM_SETHOTKEY,sOP.sSsHK.wHK_Res,0);
		EnableWindow(GetDlgItem(hwndDlg,IDC_HOTKEY_TIPSS),FALSE);
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOG_BACKTIP);
		}

		return FALSE;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hHotKey_TIPSS)
		{
			i ++;
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CLOSE:
		case IDCANCEL:
			g_BKTipDialog = NULL;
			SendMessage(hMainDialog,WM_COMMAND,MAKEWPARAM(IDC_REFRESH,0),NULL);
			EndDialog(hwndDlg,TRUE);
			return FALSE;
		case IDC_CHECK_DONTSHOWTIP:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_DONTSHOWTIP) == BST_CHECKED)
			{
				sOP.bShowBKTip = 0;
				WriteOptionFile(sOP);
			}
			break;
		default:
			return FALSE;
		}
		return TRUE;
	default:
		break;
	}
	
	return FALSE;
}


int DrawTitleBar(HDC hDC)
{
	DWORD dwErr = 0;
	HDC hCompDC = CreateCompatibleDC(hDC);
	if(!hCompDC)
	{
		OutputDebugString(L"CreateCompatibleDC Error.");
		dwErr = GetLastError();
		return 0;
	}
	//HBITMAP hBit_Title = CreateCompatibleBitmap(hCompDC,610,31);
//	HBITMAP hBit_Title = CreateBitmap(610,31,1,25,NULL);
	RECT rtTitle,rtWnd;
	GetWindowRect(hMainDialog,&rtWnd);
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
	HBITMAP hBit_Title = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_TITLEBAR));
	if(!hBit_Title)
	{
		DeleteDC(hCompDC);
		return 0;
	}
	HBITMAP hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_Title);
	BitBlt(hDC,rtTitle.left-4,rtTitle.top-4,rtTitle.right - rtTitle.left+10,
		GetSystemMetrics(SM_CYSIZE)+9,hCompDC,0,0,SRCCOPY);
	hBit_Title = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hOldBit);
	if(!DeleteObject(hBit_Title))
	{
		OutputDebugString(L"Delete Bitmap Object Error.");
	}

	//HBITMAP hBit_Minimize = CreateCompatibleBitmap(hCompDC,21,21);
	HBITMAP hBit_Minimize = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MINIMIZE));
	if(iMoveIn == 1 && iPressIn != 1)
	{
		HBITMAP hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_Minimize);
		BitBlt(hDC,rtTitle.right - 22,rtTitle.top + 1,rtTitle.right - rtTitle.left+10,
			GetSystemMetrics(SM_CYSIZE)+9,hCompDC,
			0,0,SRCCOPY);
		hBit_Minimize = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hOldBit);
		DeleteObject(hBit_Minimize);
	}
	else
	{
		DeleteObject(hBit_Minimize);
	}

	//HBITMAP hBit_PressMini = CreateCompatibleBitmap(hCompDC,21,21);
	HBITMAP hBit_PressMini = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MINIPRESS));
	if(iPressIn == 1)
	{
		HBITMAP hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_PressMini);
		BitBlt(hDC,rtTitle.right - 22,rtTitle.top+1,rtTitle.right - rtTitle.left+10,
			GetSystemMetrics(SM_CYSIZE)+9,hCompDC,
			0,0,SRCCOPY);
		hBit_PressMini = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hOldBit);
		DeleteObject(hBit_PressMini);
	}
	else
	{
		DeleteObject(hBit_PressMini);
	}

	//Draw bottom region/////////////////////////
	//HBITMAP hBit_Bottom = CreateCompatibleBitmap(hCompDC,610,4);
	HBITMAP hBit_Bottom = (HBITMAP)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BOTTOM));
	int iFrame = GetSystemMetrics(SM_CYFRAME);
	hOldBit = NULL;
	hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_Bottom);
	BitBlt(hDC,0,rtWnd.bottom-rtWnd.top - iFrame+1 ,rtWnd.right-rtWnd.left,
		iFrame,hCompDC,0,0,SRCCOPY);

	hBit_Bottom = (HBITMAP)SelectObject(hCompDC,hOldBit);
	DeleteObject(hBit_Bottom);

	//Draw left region//////////////////////////////////

	//HBITMAP hBit_LeftSide = CreateCompatibleBitmap(hCompDC,610,4);
	HBITMAP hBit_LeftSide;
	if(iExtWindow == 1)
	{
		hBit_LeftSide = (HBITMAP)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LEFTEXT));
	}
	else
	{
		hBit_LeftSide = (HBITMAP)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_LEFTSIDE));
	}
	int iXFrame = GetSystemMetrics(SM_CXFRAME);
	int iYFrame = GetSystemMetrics(SM_CYSIZE);
	hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_LeftSide);
	BitBlt(hDC,0,iYFrame+4,iXFrame,rtWnd.bottom-rtWnd.top-iYFrame,hCompDC,0,0,SRCCOPY);

	hBit_LeftSide = (HBITMAP)SelectObject(hCompDC,hOldBit);
	DeleteObject(hBit_LeftSide);



	//HBITMAP hBit_RightSide = CreateCompatibleBitmap(hCompDC,610,4);
	HBITMAP hBit_RightSide;
	if(iExtWindow == 1)
	{
		hBit_RightSide = (HBITMAP)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_RIGHTEXT));
	}
	else
	{
		hBit_RightSide = (HBITMAP)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_RIGHTSIDE));
	}
	
	iXFrame = GetSystemMetrics(SM_CXFRAME);
	iYFrame = GetSystemMetrics(SM_CYSIZE);
	hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_RightSide);
	BitBlt(hDC,rtWnd.right - rtWnd.left -iFrame +1 ,iYFrame+4,iXFrame,
		rtWnd.bottom-rtWnd.top-iYFrame,hCompDC,0,0,SRCCOPY);
	hBit_RightSide = (HBITMAP)SelectObject(hCompDC,hOldBit);
	DeleteObject(hBit_RightSide);




	DeleteDC(hCompDC);
	return 1;
}

HICON hCurrIcon, hUpIconI, hDnIconI, hUpIconA, hDnIconA;

// Width and height values:
#define MARGIN 2
#define ICON_WIDTH 32
#define ICON_HEIGHT 32


int DrawExpandBtn(DRAWITEMSTRUCT* pdis)
{
	static RECT rect;
	int iCount = 0;
//	TCHAR s[2048];
	
	// Icon handles:
	
	// Use copy of rectangle:
	rect = pdis->rcItem;
	
	if (iCount < 1)
	{
		// LoadIcon only loads once, but LoadImage does not, 
		// so in case you call the latter, use a static counter:
		iCount++;
		// Inactive buttons:
		hUpIconI = (HICON) LoadIcon(hInst, MAKEINTRESOURCE(ID_UP_ICONI));
		if (!hUpIconI)
		{


			DWORD dwErr = GetLastError();
		}
		
		hDnIconI = (HICON) LoadIcon(hInst, MAKEINTRESOURCE(ID_DN_ICONI));
		if (!hDnIconI)
		{
			DWORD dwErr = GetLastError();
		}
		
		// Active buttons:
		hUpIconA = (HICON) LoadIcon(hInst, MAKEINTRESOURCE(ID_UP_ICONA));
		if (!hUpIconA)
		{
			DWORD dwErr = GetLastError();
		}
		
		hDnIconA = (HICON) LoadIcon(hInst, MAKEINTRESOURCE(ID_DN_ICONA));
		if (!hDnIconA)
		{
			DWORD dwErr = GetLastError();
		}
	}
	
	// If the control's id is that of the "Up" button:
	if (IDC_STATIC_EXPANDINFO == pdis->CtlID) 
	{
		// If the button is selected, display the 
		// "active" icon, else the "inactive" icon:
		if (pdis->itemState & ODS_SELECTED)
		{
			hCurrIcon = hUpIconA;
		}
		else if(pdis->itemState & ODS_SELECTED)
		{
			hCurrIcon = hDnIconI;
		}
		else
		{
			hCurrIcon = hUpIconI;
		}
		

	}
	
	
	// Center the icon inside the control's rectangle:
	if (!DrawIconEx(
		pdis->hDC,
		(int) 0.5 * (rect.right - rect.left - ICON_WIDTH),
		(int) 0.5 * (rect.bottom - rect.top - ICON_HEIGHT),
		(HICON) hCurrIcon,
		ICON_WIDTH,
		ICON_HEIGHT,
		0, NULL, DI_NORMAL
	))
	{
		DWORD dwErr = GetLastError();
	}
	
	return 1;
}






int OnNCMouseMove(HDC hDC,LPARAM lParam)
{
	HDC hCompDC = CreateCompatibleDC(hDC);
	//HBITMAP hBit_Title = CreateCompatibleBitmap(hCompDC,21,21);
	HBITMAP hBit_Title;
	RECT rtTitle,rtWnd;
	GetWindowRect(hMainDialog,&rtWnd);
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
	int iX = GET_X_LPARAM(lParam);
	int iY = GET_Y_LPARAM(lParam);
	POINT pMouse;
	pMouse.x = iX;
	pMouse.y = iY;
	
	ScreenToClient(hMainDialog,&pMouse);
	pMouse.y = abs(pMouse.y);

	RECT rtButton;
	rtButton.left = rtTitle.right - 25;
	rtButton.right = rtTitle.right - rtTitle.left+0;
	rtButton.top = rtTitle.bottom-5;
	rtButton.bottom = rtButton.top+21;

	iMoveIn = 0;
	if(PtInRect(&rtButton,pMouse))
	{
		hBit_Title = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MINIMIZE));
		HBITMAP hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_Title);
		BitBlt(hDC,rtTitle.right - 22,rtTitle.top+1,rtTitle.right - rtTitle.left+10,GetSystemMetrics(SM_CYSIZE)+9,hCompDC,
			0,0,SRCCOPY);
		hBit_Title = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hOldBit);
		iMoveIn = 1;
		DeleteObject(hBit_Title);
	}
	else
	{
		hBit_Title = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_TITLEBAR));
		HBITMAP hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_Title);
		BitBlt(hDC,rtTitle.left-4,rtTitle.top-4,rtTitle.right - rtTitle.left+10,
		GetSystemMetrics(SM_CYSIZE)+9,hCompDC,0,0,SRCCOPY);
		hBit_Title = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hOldBit);
		iPressIn = 0;
		DeleteObject(hBit_Title);
	}
	
	

	DeleteDC(hCompDC);



	return 1;
}



int OnNCLBDown(HDC hDC,LPARAM lParam)
{
	HDC hCompDC = CreateCompatibleDC(hDC);
	//HBITMAP hBit_Title = CreateCompatibleBitmap(hCompDC,21,21);
	HBITMAP hBit_Title = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MINIPRESS));
	RECT rtTitle,rtWnd;
	GetWindowRect(hMainDialog,&rtWnd);
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

	int iX = GET_X_LPARAM(lParam);
	int iY = GET_Y_LPARAM(lParam);
	POINT pMouse;
	pMouse.x = iX;
	pMouse.y = iY;
	
	ScreenToClient(hMainDialog,&pMouse);
	pMouse.y = abs(pMouse.y);
	RECT rtButton;
	rtButton.left = rtTitle.right - 25;
	rtButton.right = rtTitle.right - rtTitle.left+0;
	rtButton.top = rtTitle.bottom-5;
	rtButton.bottom = rtButton.top+21;

	iPressIn = 0;
	if(PtInRect(&rtButton,pMouse))
	{
		HBITMAP hOldBit = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hBit_Title);
		BitBlt(hDC,rtTitle.right - 22,rtTitle.top + 1,rtTitle.right - rtTitle.left+10,
			GetSystemMetrics(SM_CYSIZE)+9,hCompDC,
			0,0,SRCCOPY);
		hBit_Title = (HBITMAP)SelectObject(hCompDC,(HGDIOBJ)hOldBit);
		iPressIn = 1;
		DeleteObject(hBit_Title);
		SendMessage(hMainDialog,WM_COMMAND,IDM_SHOWWINDOW,1);
		iPressIn = 0;
		iMoveIn = 0;
	}
	else 
	{
		DeleteObject(hBit_Title);
	}
	DeleteDC(hCompDC);

	return 1;
}

int OnNCLBUp(HDC hWndDC,LPARAM lParam)
{
//	RECT rtTitle,rtWnd;
//	GetWindowRect(hMainDialog,&rtWnd);
//	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
//	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
//	rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
//	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
//
//	int iX = GET_X_LPARAM(lParam); 
//	int iY = GET_Y_LPARAM(lParam);
//	POINT pMouse;
//	pMouse.x = iX;
//	pMouse.y = iY;
//	
//	ScreenToClient(hMainDialog,&pMouse);
//	pMouse.y = abs(pMouse.y);
//	RECT rtButton;
//	rtButton.left = rtTitle.right - 27;
//	rtButton.right = rtTitle.right - 27 + 21;
//	rtButton.top = rtTitle.top-1;
//	rtButton.bottom = rtTitle.top-1 + 21;
//
//	iUpIn = 0;
//	if(PtInRect(&rtButton,pMouse))
//	{
//		iUpIn = 1;
//		iPressIn = 0;
//	}

	

	return 1;
}


int CreateAutoReserveFile()
{
	HANDLE hARFile;
	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cARFileName,1024);
	PathRemoveFileSpec(cARFileName);
	wcscat(cARFileName,L"\\AutoReserveFile.arf");
	
	hARFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	delete [] cARFileName;
	
	if(hARFile == INVALID_HANDLE_VALUE)
	{
		//MessageBox(hMainDialog,"Create AutoReserveFileList Error","隐身专家",MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	if(GetFileSize(hARFile,0)) 
	{
		CloseHandle(hARFile);
		return 1;
	}
	long lSize = 0;
	char cBuff[sizeof(long)];
	*(long*)cBuff = lSize;
	DWORD dwWritten = 0;
	WriteFile(hARFile,cBuff,sizeof(long),&dwWritten,NULL);
	CloseHandle(hARFile);

	
	return 1;
}


int AddARFL(sFileItem * pItem)
{
	HANDLE hARFile;
	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cARFileName,1024);
	PathRemoveFileSpec(cARFileName);
	wcscat(cARFileName,L"\\AutoReserveFile.arf");
	try
	{
		hARFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		delete [] cARFileName;
		
		if(hARFile == INVALID_HANDLE_VALUE)
		{
			//MessageBox(hMainDialog,"Create AutoReserveFileList Error","隐身专家",MB_OK|MB_ICONINFORMATION);
			throw "Create AutoReserveFileList Error";
		}
		DWORD dwSize = GetFileSize(hARFile,NULL);
		if(dwSize == 0)
		{
			throw "A Error Occur,The ARFile Size Is 0";
		}
		sARFL sFC;
		sFC.lNum;
		char cBuff[sizeof(long)];
		DWORD dwReaded = 0;
		ReadFile(hARFile,cBuff,sizeof(long),&dwReaded,NULL);
		sFC.lNum = atol((char*)cBuff);
		if(sFC.lNum != 0)
		{
			if(IsInARFL(pItem))
			{
				throw("Already In File List");
			}
		}
		sFC.lNum ++;
		_ltoa(sFC.lNum,(char*)cBuff,10);
		DWORD dwWritten = 0;
		//_ltow(sFC.lNum,(wchar_t*)cBuff,10);
		SetFilePointer(hARFile,0,0,FILE_BEGIN);
		WriteFile(hARFile,cBuff,sizeof(long),&dwWritten,NULL);
		SetFilePointer(hARFile,0,0,FILE_END);
		_ltoa(pItem->lSize,(char*)cBuff,10);
		WriteFile(hARFile,cBuff,sizeof(long),&dwWritten,NULL);
		WriteFile(hARFile,pItem->cFileName,pItem->lSize,&dwWritten,NULL);
	}
	catch(...)
	{
		CloseHandle(hARFile);
		return 0;
	}
	CloseHandle(hARFile);
	
	return 1;
}





int IsInARFL(sFileItem * pItem)
{
	HANDLE hARFile;
	int i = 0;
	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cARFileName,1024);
	PathRemoveFileSpec(cARFileName);
	wcscat(cARFileName,L"\\AutoReserveFile.arf");
	char * cFileBuff = (char *)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	char * cTempBuff = cFileBuff;
	try
	{
		hARFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		delete [] cARFileName;
		if(hARFile == INVALID_HANDLE_VALUE)
		{
			//MessageBox(hMainDialog,"Create AutoReserveFileList Error","隐身专家",MB_OK|MB_ICONINFORMATION);
			throw "Create AutoReserveFileList Error";
		}
		DWORD dwSize = GetFileSize(hARFile,NULL);
		if(dwSize == 0)
		{
			//OutputDebugString("A Error Occur,The ARFile Size Is 0");
			throw "A Error Occur,The ARFile Size Is 0";
		}
		sARFL sFC;
		sFC.lNum;
		char  cBuff[sizeof(long)];
		DWORD dwReaded = 0;
		ReadFile(hARFile,cBuff,sizeof(long),&dwReaded,NULL);
		sFC.lNum = atol((char*)cBuff);
		if(sFC.lNum == 0)
		{
			//OutputDebugString("The ARF File Records Is Zero");
			throw "The ARF File Records Is Zero";
		}
		memset(cFileBuff,0,2048);
		SetFilePointer(hARFile,0,0,FILE_BEGIN);
		ReadFile(hARFile,cFileBuff,2048,&dwReaded,NULL);
		long lSize = 0;
		cFileBuff += sizeof(long);
		for(i = 0;i < sFC.lNum;i++)
		{
			cFileBuff += sizeof(long);
			if(memcmp(cFileBuff,pItem->cFileName,pItem->lSize)==0)
			{
				throw 1;
			}
			memcpy(cBuff,cFileBuff-sizeof(long),sizeof(long));
			lSize = atol(cBuff);
			cFileBuff += lSize;
		}
	}
	catch(char * cErr)
	{
		CloseHandle(hARFile);
		VirtualFree(cTempBuff,0,MEM_RELEASE);
		OutputDebugStringA(cErr);
		return 0;

	}
	catch(int)
	{
		CloseHandle(hARFile);
		VirtualFree(cTempBuff,0,MEM_RELEASE);
		return 1;
	}
	catch(...)
	{
		CloseHandle(hARFile);
		VirtualFree(cTempBuff,0,MEM_RELEASE);
		return 0;
	}
	CloseHandle(hARFile);
	VirtualFree(cTempBuff,0,MEM_RELEASE);

		
	return 0;
}


int DelItem(sFileItem * pItem)
{
	if(IsInARFL(pItem) == 0)
	{
		return 0;
	}
	

	HANDLE hARFile;
	int i = 0;
	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cARFileName,1024);
	PathRemoveFileSpec(cARFileName);
	wcscat(cARFileName,L"\\AutoReserveFile.arf");
	char * cFileBuff = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	char * cTemp = cFileBuff;
	try
	{
		hARFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		delete [] cARFileName;
		if(hARFile == INVALID_HANDLE_VALUE)
		{
			//MessageBox(hMainDialog,"Create AutoReserveFileList Error","隐身专家",MB_OK|MB_ICONINFORMATION);
			throw "Create AutoReserveFileList Error";
		}
		DWORD dwSize = GetFileSize(hARFile,NULL);
		if(dwSize == 0)
		{
			//OutputDebugString("A Error Occur,The ARFile Size Is 0");
			throw "A Error Occur,The ARFile Size Is 0";
		}
		sARFL sFC;
		sFC.lNum;
		char cBuff[sizeof(long)];
		DWORD dwReaded = 0;
		ReadFile(hARFile,cBuff,sizeof(long),&dwReaded,NULL);
		sFC.lNum = atol((char * )cBuff);
		if(sFC.lNum == 0)
		{
			//OutputDebugString("The ARF File Records Is Zero");
			throw "The ARF File Records Is Zero";
		}
		memset(cFileBuff,0,2048);
		SetFilePointer(hARFile,0,0,FILE_BEGIN);
		ReadFile(hARFile,cFileBuff,2048,&dwReaded,NULL);
		long lSize = 0;
		char * cTempBuff = cFileBuff;
		cFileBuff += sizeof(long);
		DWORD dwWritten = 0;
		for(i = 0;i < sFC.lNum;i++)
		{
			memcpy(cBuff,cFileBuff,sizeof(long));
			lSize = atol(cBuff);
			cFileBuff += sizeof(long);
			if(memcmp(cFileBuff,pItem->cFileName,pItem->lSize)==0)
			{
				memcpy(cFileBuff-sizeof(long),cFileBuff+lSize,
					dwSize-(cFileBuff+lSize - cTempBuff));
				memset((cTempBuff + dwSize) - sizeof(long)+lSize,0,
					(sizeof(long)+lSize));
				SetFilePointer(hARFile,0,0,FILE_BEGIN);
				WriteFile(hARFile,cTempBuff,dwSize - sizeof(long) - pItem->lSize,&dwWritten,NULL);
				SetEndOfFile(hARFile);
				sFC.lNum --;
				_ltoa(sFC.lNum,cBuff,10);
				SetFilePointer(hARFile,0,0,FILE_BEGIN);
				WriteFile(hARFile,cBuff,sizeof(long),&dwWritten,NULL);
				throw 1;
			}
			cFileBuff += lSize;
		}
	}
	catch(char * cErr)
	{
		CloseHandle(hARFile);
		VirtualFree(cTemp,0,MEM_RELEASE);
		OutputDebugStringA(cErr);
		return 0;

	}
	catch(int)
	{
		CloseHandle(hARFile);
		VirtualFree(cTemp,0,MEM_RELEASE);
		return 1;
	}
	catch(...)
	{
		CloseHandle(hARFile);
		VirtualFree(cTemp,0,MEM_RELEASE);
		return 0;
	}
	CloseHandle(hARFile);
	VirtualFree(cTemp,0,MEM_RELEASE);


	return 0;
}


int HMF_Create()
{
	HANDLE hHMFile;
	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cARFileName,1024);
	PathRemoveFileSpec(cARFileName);
	wcscat(cARFileName,L"\\HabitMemFile.hmf");
	
	hHMFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	delete [] cARFileName;	
	if(hHMFile == INVALID_HANDLE_VALUE)
	{
		//MessageBox(hMainDialog,"Create HabitMemoryFile Error","隐身专家",MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	if(GetFileSize(hHMFile,0))
	{
		CloseHandle(hHMFile);
		return 1;
	}
	long lSize = 0;
	char cBuff[sizeof(long)];
	*(long*)cBuff = lSize;
	DWORD dwWritten = 0;
	WriteFile(hHMFile,cBuff,sizeof(long),&dwWritten,NULL);
	CloseHandle(hHMFile);

	return 1;
}



int HMF_Modify(sHMF_Item * pItem,WORD dwFlag)
{

	HANDLE hHMFile;
	int i = 0;
	wchar_t *cHMFileName = (wchar_t*) new wchar_t[1024];
	memset(cHMFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cHMFileName,1024);
	PathRemoveFileSpec(cHMFileName);
	wcscat(cHMFileName,L"\\HabitMemFile.hmf");
	char * cFileBuff = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	char * cBegin = cFileBuff;
	try
	{
		hHMFile = CreateFile(cHMFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		delete [] cHMFileName;
		
		if(hHMFile == INVALID_HANDLE_VALUE)
		{
			//MessageBox(hMainDialog,"Create AutoReserveFileList Error","隐身专家",MB_OK|MB_ICONINFORMATION);
			throw "Create AutoReserveFileList Error";
		}
		DWORD dwSize = GetFileSize(hHMFile,NULL);
		if(dwSize == 0)
		{
			//OutputDebugString("A Error Occur,The ARFile Size Is 0");
			throw "A Error Occur,The ARFile Size Is 0";
		}
		sARFL sHC;
		sHC.lNum;
		char  cBuff[sizeof(long)];
		DWORD dwReaded = 0;
		ReadFile(hHMFile,cBuff,sizeof(long),&dwReaded,NULL);
		sHC.lNum = atol((char*)cBuff);
		if(sHC.lNum == 0 && (dwFlag == HMF_DELITEM||dwFlag == HMF_ADDTIMES || dwFlag == HMF_ISHAVEITEM))
		{
			//OutputDebugString("The ARF File Records Is Zero");
			throw "The ARF File Records Is Zero";
		}
		memset(cFileBuff,0,2048);
		SetFilePointer(hHMFile,0,0,FILE_BEGIN);

		if(dwFlag == HMF_ADDITEM)
		{
			ReadFile(hHMFile,cBuff,sizeof(long),&dwReaded,NULL);
			sHC.lNum = atol((char*)cBuff);
			if(sHC.lNum != 0)
			{
				if(HMF_Modify(pItem,HMF_ISHAVEITEM))
				{
					throw("Already In File List");
				}
			}
			sHC.lNum ++;
			_ltoa(sHC.lNum,(char*)cBuff,10);
			DWORD dwWritten = 0;
			//_ltow(sFC.lNum,(wchar_t*)cBuff,10);
			SetFilePointer(hHMFile,0,0,FILE_BEGIN);
			WriteFile(hHMFile,cBuff,sizeof(long),&dwWritten,NULL);
			SetFilePointer(hHMFile,0,0,FILE_END);
			ltoa(pItem->lSize,(char*)cBuff,10);
			WriteFile(hHMFile,cBuff,sizeof(long),&dwWritten,NULL);
			WriteFile(hHMFile,pItem->cName,pItem->lSize,&dwWritten,NULL);
			char cTimes[sizeof(int)];
			memset(cTimes,0,sizeof(int));
			itoa(pItem->iTimes,cTimes,10);
			WriteFile(hHMFile,cTimes,sizeof(int),&dwWritten,NULL);
			throw 1;
		}

		sFileItem * pARItem = NULL;
		ReadFile(hHMFile,cFileBuff,2048,&dwReaded,NULL);
		long lSize = 0;
		char * cTempBuff = cFileBuff;
		cFileBuff += sizeof(long);
		DWORD dwWritten = 0;
		char cTime[sizeof(int)];
		for(i = 0;i < sHC.lNum;i++)
		{
			memcpy(cBuff,cFileBuff,sizeof(long));
			lSize = atol(cBuff);
			cFileBuff += sizeof(long);
			if(memcmp(cFileBuff,pItem->cName,pItem->lSize)==0)
			{
				if(dwFlag == HMF_ISHAVEITEM)
				{
					throw 1;
				}
				if(dwFlag == HMF_ADDTIMES)
				{
					cFileBuff += lSize;
					memcpy(cTime,cFileBuff,sizeof(int));
					pItem->iTimes = atoi(cTime);
					pItem->iTimes ++;
					if(pItem->iTimes > 5)
					{
						pARItem = new sFileItem;
						pARItem->lSize = pItem->lSize * 2;
						pARItem->cFileName = pItem->cName;
						AddARFL(pARItem);
						pARItem->cFileName = NULL;
						delete pARItem;
					}
					itoa(pItem->iTimes,cTime,10);
					memcpy(cFileBuff,cTime,sizeof(int));
					SetFilePointer(hHMFile,0,0,FILE_BEGIN);
					WriteFile(hHMFile,cTempBuff,dwSize,&dwWritten,NULL);
					throw 1;
				}
				else if(dwFlag == HMF_DELITEM)
				{
					memcpy(cFileBuff-sizeof(long),cFileBuff+lSize+sizeof(int),
						dwSize-(cFileBuff+lSize+sizeof(int) - cTempBuff));
					memset((cTempBuff + dwSize) - sizeof(long)-lSize-sizeof(int),0,
						(sizeof(long)+lSize+sizeof(int)));
					SetFilePointer(hHMFile,0,0,FILE_BEGIN);
					WriteFile(hHMFile,cTempBuff,dwSize - sizeof(long) - pItem->lSize-sizeof(int),&dwWritten,NULL);
					SetEndOfFile(hHMFile);
					sHC.lNum --;
					itoa(sHC.lNum,cBuff,10);
					SetFilePointer(hHMFile,0,0,FILE_BEGIN);
					WriteFile(hHMFile,cBuff,sizeof(long),&dwWritten,NULL);
					throw 1;
				}

			}
			cFileBuff += lSize;
			cFileBuff += sizeof(int);
		}
		if(i >= sHC.lNum)
		{
			if(dwFlag == HMF_ISHAVEITEM)
			{
				throw "Not Have This Item Between IsHaveItem";
			}
			if(dwFlag == HMF_ADDTIMES)
			{
				throw "Not Have This Item Between Add Times";
			}
			if(dwFlag == HMF_DELITEM)
			{
				throw "Not Have This Item Between Del";
			}
		}
	}
	catch(char * cErr)
	{
		CloseHandle(hHMFile);
		VirtualFree(cBegin,0,MEM_RELEASE);
		OutputDebugStringA(cErr);
		return 0;
	}
	catch(int)
	{
		CloseHandle(hHMFile);
		VirtualFree(cBegin,0,MEM_RELEASE);
		return 1;
	}
	catch(...)
	{
		CloseHandle(hHMFile);
		VirtualFree(cBegin,0,MEM_RELEASE);
		return 0;
	}
	CloseHandle(hHMFile);
	VirtualFree(cBegin,0,MEM_RELEASE);
	
	return 0;
}





int IsUseSkin()
{
	return 0;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	int iCap = 0;
	if(osvi.dwMajorVersion >= 5 && osvi.dwMinorVersion >= 1 &&
		osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		iCap = GetSystemMetrics(SM_CYCAPTION);
		if(iCap == 26)
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
	
	return 0;
}



DWORD GetOSVersion()
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	int iCap = 0;

	return osvi.dwMajorVersion;
}


int ChangeWindowStyle(HWND hwndDlg,int iFlaged)
{
//	return 1;//FIXME
	LONG lStyle = 0;
	lStyle = GetWindowLong(hwndDlg,GWL_STYLE);
	if(iFlaged == 1)
	{
		lStyle |= WS_MINIMIZE|WS_SYSMENU;
	}
	else
	{
		lStyle ^= WS_MINIMIZE|WS_SYSMENU;
	}
	SetWindowLong(hwndDlg,GWL_STYLE,lStyle);

	return 1;
}


int GetSysLangLoadLang()
{
	LANGID SysLangID = GetSystemDefaultLangID();
	int iRet = LoadLanguageBaseID(SysLangID);
	
	
	return iRet;
}

int SetCurrentLanguage()
{
	int iRet = 0;
	wchar_t * cSettingFile = new wchar_t [1024];
	memset(cSettingFile,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cSettingFile,1024);
	//GetModuleFileName(hInst,cCurDir,1024);
	PathRemoveFileSpec(cSettingFile);

	//GetCurrentDirectory_HD(1024,cSettingFile);
	wcscat(cSettingFile,L"\\Language\\LangSetting.ini");
	HANDLE hSettingFile = CreateFileW(cSettingFile,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	


	if(hSettingFile == INVALID_HANDLE_VALUE)
	{
//		delete [] cSettingFile;
		if(WritePrivateProfileStringW(L"Setting",L"CurrentLanguage",L"English",cSettingFile) == 0)
		{
			OutputDebugString(L"Write Language Error.");
		}
		if(WritePrivateProfileStringW(L"Setting",L"CurrentLanguageID",L"First",cSettingFile) == 0)
		{
			OutputDebugString(L"Write Language Error.");
		}

		//return iRet;
	}
	else
	{
		CloseHandle(hSettingFile);
	}
	wchar_t szSection[20],*szValue = new wchar_t[1024],szKey[30];
	memset(szSection,0,20 * sizeof(wchar_t));
	memset(szKey,0,30 * sizeof(wchar_t));
	memset(szValue,0,1024 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");
	wcscpy(szKey,L"CurrentLanguage");


	if(GetPrivateProfileStringW(szSection,L"CurrentLanguageID",L"",szValue,1024,cSettingFile) != 0)
	{
		if(wcscmp(szValue,L"First") == 0)
		{
			int iRet = GetSysLangLoadLang();
			if(iRet == 1)
			{
				delete [] cSettingFile;
				delete [] szValue;

				return 1;
			}
		}
	}


	if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,1024,cSettingFile) != 0)
	{
		wchar_t * cFilePath = new wchar_t[1024];
		memset(cFilePath,0,1024 * sizeof(wchar_t));
		iRet = LoadLanguage(szValue,cFilePath);
		delete [] cFilePath;
	}
	delete [] cSettingFile;
	delete [] szValue;
	
	return iRet;
}

int LoadLanguage(wchar_t * pLanguage /* = NULL*/,wchar_t * pFilePath)
{
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");

	wchar_t szKey[] = L"Language",*szLang;
	DWORD dwSize = 1000;
	
	if(pLanguage != NULL)
	{
		szLang = pLanguage;
	}
	else
	{
	}
	
	
	wchar_t * szFilePath = new wchar_t[1024];
	memset(szFilePath,0,1024 * sizeof(wchar_t));
	wchar_t * cIniFile = new wchar_t[1024];
	memset(cIniFile,0,1024 * sizeof(wchar_t));
	GetCurrentDirectory_HD(1024,cIniFile);
	wcscat(cIniFile,L"\\Language\\*.ini");
	bool bFound = false;
	HANDLE hFindFile = FindFirstFileW(cIniFile,&FFD);
	BOOL bRet = TRUE;
	wchar_t * szValue = new wchar_t[1024];
	wchar_t * szLanguageID = new wchar_t[1024];	
	while(bRet && hFindFile != INVALID_HANDLE_VALUE)
	{

		memset(szLanguageID,0,1024 * sizeof(wchar_t));
		memset(szValue,0,1024 * sizeof(wchar_t));
		GetCurrentDirectory_HD(1024,szFilePath);
		wcscat(szFilePath,L"\\Language\\");
		wcscat(szFilePath,FFD.cFileName);
		if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,
			1024,szFilePath) != 0)
		{
			if(GetPrivateProfileStringW(szSection,L"LanguageID",L"",szLanguageID,
				1024,szFilePath) != 0)
			{

			}
			
			if(wcscmp(szValue,szLang) == 0)
			{
				if(pFilePath != NULL)
				{
					wcscpy(pFilePath,szFilePath);
				}
				bFound = true;
				GetCurrentDirectory_HD(1024,szFilePath);
				wcscat(szFilePath,L"\\Language\\");
				wcscat(szFilePath,L"LangSetting.ini");

				if(WritePrivateProfileString(szSection,L"CurrentLanguage",szValue,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}
				if(WritePrivateProfileString(szSection,L"CurrentLanguageID",szLanguageID,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}
								
// 				delete [] szValue;
// 				szValue = NULL;
// 				delete [] szLanguageID;
// 				szLanguageID = NULL;

				break;
			}
		}
		bRet = FindNextFile(hFindFile,&FFD);
		DWORD dwErr = 0;
		if(bRet == 0)
		{
			dwErr = GetLastError();
		}

	}
	
	delete [] szValue;
	
	delete [] szLanguageID;
	FindClose(hFindFile);
	delete [] szFilePath;
	delete [] cIniFile;
	


	if(!bFound)
	{
		return 0;
	}
	return 1;
}





int SetDialogTextW(HWND * pDialog,UINT uID)
{
	wchar_t szSection [20];
	memset(szSection,0,20*sizeof(wchar_t));
	wcscpy(szSection,L"String");

	
	wchar_t * szKey,cDlgID[20];
	memset(cDlgID,0,20*sizeof(wchar_t));
	szKey = new wchar_t[512];
	memset(szKey,0,512*sizeof(wchar_t));

	wchar_t * szDefault = L"";

	//读对话框标题
	swprintf(szKey,L"IDD%d_Title",uID);
	wchar_t * pLangFilePath = new wchar_t[1024];
	memset(pLangFilePath,0,1024*sizeof(wchar_t));
	GetCurrentDirectory_HD(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wchar_t * pFileName = new wchar_t[1024];
	wcscat(pLangFilePath,FFD.cFileName);
	wchar_t * pData = new wchar_t[1024];
	memset(pData,0,1024*sizeof(wchar_t));
	DWORD dwSize = 1024;

	if(GetPrivateProfileStringW(szSection,szKey,szDefault,
			pData,dwSize,pLangFilePath) != 0)
	{
		SetWindowTextW(*pDialog,pData);
		
	}

	delete [] pData;
	delete [] pLangFilePath;
	delete [] szKey;
	delete [] pFileName;
	//写入各个子控件的标题文字
	EnumChildWindows(*pDialog,EnumDlgChildProcW,(LPARAM)&uID);

	return 1;
}






BOOL CALLBACK EnumDlgChildProcW(HWND hwnd,LPARAM lParam)
{
	UINT uDlgID = *(UINT*)lParam;
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"String");

	wchar_t * szDefault = L"";
	DWORD dwSize = 1024;
	wchar_t * pData = new wchar_t[dwSize];


	wchar_t * pLangFilePath = new wchar_t[1024];
	GetCurrentDirectory_HD(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wcscat(pLangFilePath,FFD.cFileName);

	wchar_t * szKey = new wchar_t[512];
	memset(szKey,0,512 * sizeof(wchar_t));
	int iCtrlID = GetDlgCtrlID(hwnd);
	swprintf(szKey,L"IDD%d_%d",uDlgID,iCtrlID);


	if(GetPrivateProfileStringW(szSection,szKey,szDefault,
			pData,dwSize,pLangFilePath) != 0)
	{
		SetWindowTextW(hwnd,pData);
	}

	if(wcscmp(szKey,L"IDD10_1004") == 0)
	{
		if(TrayFlag == 1)
		{
			wcscpy(szKey,L"IDD10_1664");
			if(GetPrivateProfileStringW(szSection,szKey,szDefault,
					pData,dwSize,pLangFilePath) != 0)
			{
				SetWindowTextW(hwnd,pData);
			}
		}
	}

	if(wcscmp(szKey,L"IDD181_1226") == 0)
	{
		if(sOP.bUseMouse == 0 && sOP.bUseMiddle == 0)
		{
			wcscpy(szKey,L"IDS_MOUSE_STATUS_DISABLED");
			if(GetPrivateProfileStringW(szSection,szKey,szDefault,
					pData,dwSize,pLangFilePath) != 0)
			{
				SetWindowTextW(hwnd,pData);
			}
		}
	}


	delete [] pLangFilePath;
	delete [] pData;
	delete [] szKey;
	
	return TRUE;
}



wchar_t * LoadItemStringW(wchar_t * pItemID,wchar_t * pText,wchar_t * pDefault/* = NULL*/)
{	
//	WIN32_FIND_DATAW FFD;
//	memset(&FFD,0,sizeof(WIN32_FIND_DATAW));
	wchar_t pLangFilePath[1024];// = new wchar_t;
	memset(pLangFilePath,0,1024*sizeof(wchar_t));
	GetModuleFileName(NULL,pLangFilePath,1024);
	PathRemoveFileSpec(pLangFilePath);
	//GetCurrentDirectory_HD(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wcscat(pLangFilePath,FFD.cFileName);

	memset(pText,0,1024*sizeof(wchar_t));
	if(GetPrivateProfileStringW(L"String",pItemID,L"",
		pText,1024,pLangFilePath) != 0)
	{
		//delete [] pLangFilePath;
		if(wcswcs(pText,L"http://"))
		{
			if(wcswcs(pText,L"www.hidedragon.com") == NULL)
			{
				memset(pText,0,1024*sizeof(wchar_t));
				return L"";
			}
		}
		return pText;
	}
	else
	{
		//delete [] pLangFilePath;
		if(pDefault)
		{
			return pDefault;
		}
		else
		{
			return L"";
		}
	}
}


int SetMenuString()
{
	wchar_t * pWMenuText = new wchar_t[1024];
	memset(pWMenuText,0,1024*sizeof(wchar_t));
	pWMenuText = LoadItemStringW(L"IDM_EXIT",pWMenuText);
	ModifyMenuW(hTrayMenu,IDM_EXIT,MF_BYCOMMAND|MF_STRING,IDM_EXIT,pWMenuText);
	
	ModifyMenuW(hTrayMenu,IDM_SHOWWINDOW,MF_BYCOMMAND|MF_STRING,IDM_SHOWWINDOW,
		LoadItemStringW(L"IDM_SHOWWINDOW",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_HIDETIP,MF_BYCOMMAND|MF_STRING,IDM_HIDETIP,
	LoadItemStringW(L"IDM_HIDETIP",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_OPTION,MF_BYCOMMAND|MF_STRING,IDM_OPTION,
	LoadItemStringW(L"IDM_OPTION",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_SPECBK,MF_BYCOMMAND|MF_STRING,IDM_SPECBK,
	LoadItemStringW(L"IDM_SPECBK",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_ABOUT,MF_BYCOMMAND|MF_STRING,IDM_ABOUT,
	LoadItemStringW(L"IDM_ABOUT",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_REGISTER,MF_BYCOMMAND|MF_STRING,IDM_REGISTER,
	LoadItemStringW(L"IDM_REGISTER",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_SUGGEST,MF_BYCOMMAND|MF_STRING,IDM_SUGGEST,
	LoadItemStringW(L"IDM_SUGGEST",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_BLOG,MF_BYCOMMAND|MF_STRING,IDM_BLOG,
	LoadItemStringW(L"IDM_BLOG",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_UPDATE,MF_BYCOMMAND|MF_STRING,IDM_UPDATE,
	LoadItemStringW(L"IDM_UPDATE",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_USERMANUAL,MF_BYCOMMAND|MF_STRING,IDM_USERMANUAL,
	LoadItemStringW(L"IDM_USERMANUAL",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_HIDEICON,MF_BYCOMMAND|MF_STRING,IDM_HIDEICON,
	LoadItemStringW(L"IDM_HIDEICON",pWMenuText));
	ModifyMenuW(hTrayMenu,IDM_VIDEOTURORIAL,MF_BYCOMMAND|MF_STRING,IDM_VIDEOTURORIAL,
		LoadItemStringW(L"IDM_VIDEOTURORIAL",pWMenuText));
	
	



	HMENU hMainMenu = GetMenu(hMainDialog);
	ModifyMenuW(hMainMenu,0,MF_BYPOSITION|MF_STRING,0,LoadItemStringW(L"IDM_MAIN_FILE",pWMenuText,L"&File"));
	ModifyMenuW(hMainMenu,1,MF_BYPOSITION|MF_STRING,1,LoadItemStringW(L"IDM_MAIN_JOB",pWMenuText,L"&Job"));
	ModifyMenuW(hMainMenu,2,MF_BYPOSITION|MF_STRING,2,LoadItemStringW(L"IDM_MAIN_DISGUISE",pWMenuText,L"&Disguise"));
	ModifyMenuW(hMainMenu,3,MF_BYPOSITION|MF_STRING,3,LoadItemStringW(L"IDM_MAIN_BACKGROUND",pWMenuText,L"&Disguise"));
	ModifyMenuW(hMainMenu,4,MF_BYPOSITION|MF_STRING,4,LoadItemStringW(L"IDM_MAIN_TOOL",pWMenuText,L"&Tool"));
	ModifyMenuW(hMainMenu,5,MF_BYPOSITION|MF_STRING,5,LoadItemStringW(L"IDM_MAIN_HELP",pWMenuText,L"&Help"));
	HMENU hFileMenu = GetSubMenu(hMainMenu,0);
	ModifyMenuW(hFileMenu,IDM_ONTOP,MF_BYCOMMAND|MF_STRING,IDM_ONTOP,
		LoadItemStringW(L"IDM_ONTOP",pWMenuText,L"Always On To&p"));
	ModifyMenuW(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND|MF_STRING,IDM_HIDEICON,LoadItemStringW(L"IDM_HIDEICON",pWMenuText));
	ModifyMenuW(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND|MF_STRING,IDM_HIDEICON,LoadItemStringW(L"IDM_HIDEICON",pWMenuText));
	ModifyMenuW(hFileMenu,IDM_HIDEHDPROCESS,MF_BYCOMMAND|MF_STRING,
		IDM_HIDEHDPROCESS,LoadItemStringW(L"IDM_HIDEHDPROCESS",pWMenuText));

	
	ModifyMenuW(hFileMenu,IDM_HIDETIP,MF_BYCOMMAND|MF_STRING,IDM_HIDETIP,LoadItemStringW(L"IDM_HIDETIP",pWMenuText));
	ModifyMenuW(hFileMenu,IDM_EXIT,MF_BYCOMMAND|MF_STRING,IDM_EXIT,LoadItemStringW(L"IDM_EXIT",pWMenuText));
	ModifyMenuW(hFileMenu,IDM_NEWCOMMERMODEL,MF_BYCOMMAND|MF_STRING,IDM_NEWCOMMERMODEL,
		LoadItemStringW(L"IDM_NEWCOMMERMODEL",pWMenuText));
	ModifyMenuW(hFileMenu,IDM_EXPANDINFOPANEL,MF_BYCOMMAND|MF_STRING,IDM_EXPANDINFOPANEL,
		LoadItemStringW(L"IDM_EXPANDINFOPANEL",pWMenuText));

	HMENU hJobMenu = GetSubMenu(hMainMenu,1);
	ModifyMenuW(hJobMenu,IDM_RESERVESEL,MF_BYCOMMAND|MF_STRING,IDM_RESERVESEL,LoadItemStringW(L"IDM_RESERVESEL",pWMenuText));
	ModifyMenuW(hJobMenu,IDM_HIDESEL,MF_BYCOMMAND|MF_STRING,IDM_HIDESEL,LoadItemStringW(L"IDM_HIDESEL",pWMenuText));
	ModifyMenuW(hJobMenu,IDM_RESERVE,MF_BYCOMMAND|MF_STRING,IDM_RESERVE,LoadItemStringW(L"IDM_RESERVE",pWMenuText));
	ModifyMenuW(hJobMenu,IDM_AUTORESERVE,MF_BYCOMMAND|MF_STRING,IDM_AUTORESERVE,LoadItemStringW(L"IDM_AUTORESERVE",pWMenuText));
	ModifyMenuW(hJobMenu,IDM_PROPERTY,MF_BYCOMMAND|MF_STRING,IDM_PROPERTY,LoadItemStringW(L"IDM_PROPERTY",pWMenuText));
	


	HMENU hDisguishMenu = GetSubMenu(hMainMenu,2);
	ModifyMenuW(hDisguishMenu,0,MF_BYPOSITION|MF_STRING,0,LoadItemStringW(L"IDM_SETWNDALPHA",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_TRANSPARENT_RESTORE,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_RESTORE,
		LoadItemStringW(L"IDM_TRANSPARENT_RESTORE",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_TRANSPARENT_LOW,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_LOW,
		LoadItemStringW(L"IDM_TRANSPARENT_LOW",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_TRANSPARENT_MIDDLELOW,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MIDDLELOW,
		LoadItemStringW(L"IDM_TRANSPARENT_MIDDLELOW",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_TRANSPARENT_MIDDLE,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MIDDLE,
		LoadItemStringW(L"IDM_TRANSPARENT_MIDDLE",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_TRANSPARENT_MIDDLEHIGH,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MIDDLEHIGH,
		LoadItemStringW(L"IDM_TRANSPARENT_MIDDLEHIGH",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_TRANSPARENT_MOSTHIGH,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MOSTHIGH,
		LoadItemStringW(L"IDM_TRANSPARENT_MOSTHIGH",pWMenuText));

	ModifyMenuW(hDisguishMenu,IDM_CHANGEICON,MF_BYCOMMAND|MF_STRING,IDM_CHANGEICON,
		LoadItemStringW(L"IDM_CHANGEICON",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_CHANGETITLE,MF_BYCOMMAND|MF_STRING,IDM_CHANGETITLE,
		LoadItemStringW(L"IDM_CHANGETITLE",pWMenuText));
	ModifyMenuW(hDisguishMenu,IDM_RESTOREICONTITLE,MF_BYCOMMAND|MF_STRING,
		IDM_RESTOREICONTITLE,LoadItemStringW(L"IDM_RESTOREICONTITLE",pWMenuText));	
	ModifyMenuW(hDisguishMenu,IDM_REMEMBER,MF_BYCOMMAND|MF_STRING,
		IDM_REMEMBER,LoadItemStringW(L"IDM_REMEMBER",pWMenuText));	


	

	HMENU hBackMenu = GetSubMenu(hMainMenu,3);
	ModifyMenuW(hBackMenu,IDM_TOBK,MF_BYCOMMAND|MF_STRING,IDM_TOBK,LoadItemStringW(L"IDM_TOBK",pWMenuText));
	ModifyMenuW(hBackMenu,IDM_BKWNDHOTKEY,MF_BYCOMMAND|MF_STRING,IDM_BKWNDHOTKEY,LoadItemStringW(L"IDM_BKWNDHOTKEY",pWMenuText));
	ModifyMenuW(hBackMenu,IDM_UNREGISTERHK,MF_BYCOMMAND|MF_STRING,IDM_UNREGISTERHK,LoadItemStringW(L"IDM_UNREGISTERHK",pWMenuText));
	ModifyMenuW(hBackMenu,IDM_SPECBK,MF_BYCOMMAND|MF_STRING,IDM_SPECBK,LoadItemStringW(L"IDM_SPECBK",pWMenuText));




	HMENU hToolMenu = GetSubMenu(hMainMenu,4);
	ModifyMenuW(hToolMenu,IDM_OPTION,MF_BYCOMMAND|MF_STRING,IDM_OPTION,LoadItemStringW(L"IDM_OPTION",pWMenuText));
	ModifyMenuW(hToolMenu,IDM_SPECBK,MF_BYCOMMAND|MF_STRING,IDM_SPECBK,LoadItemStringW(L"IDM_SPECBK",pWMenuText));
	ModifyMenuW(hToolMenu,IDM_UPDATE,MF_BYCOMMAND|MF_STRING,IDM_UPDATE,LoadItemStringW(L"IDM_UPDATE",pWMenuText));
	ModifyMenuW(hToolMenu,2,MF_BYPOSITION|MF_STRING,2,LoadItemStringW(L"IDM_CHANGELANGUAGE",pWMenuText));
	
	
	
	HMENU hHelpMenu = GetSubMenu(hMainMenu,5);
	ModifyMenuW(hHelpMenu,0,MF_BYPOSITION|MF_STRING,0,LoadItemStringW(L"IDM_QUICKSTART",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_ABOUT,MF_BYCOMMAND|MF_STRING,IDM_ABOUT,LoadItemStringW(L"IDM_ABOUT",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_USERMANUAL,MF_BYCOMMAND|MF_STRING,IDM_USERMANUAL,
		LoadItemStringW(L"IDM_USERMANUAL",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_VIDEOTURORIAL,MF_BYCOMMAND|MF_STRING,IDM_VIDEOTURORIAL,
		LoadItemStringW(L"IDM_VIDEOTURORIAL",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_SUGGEST,MF_BYCOMMAND|MF_STRING,IDM_SUGGEST,LoadItemStringW(L"IDM_SUGGEST",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_BLOG,MF_BYCOMMAND|MF_STRING,IDM_BLOG,LoadItemStringW(L"IDM_BLOG",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_BUYONLINE,MF_BYCOMMAND|MF_STRING,IDM_BUYONLINE,LoadItemStringW(L"IDM_BUYONLINE",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_REGISTER,MF_BYCOMMAND|MF_STRING,IDM_REGISTER,LoadItemStringW(L"IDM_REGISTER",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_XPQH,MF_BYCOMMAND|MF_STRING,IDM_XPQH,LoadItemStringW(L"IDM_XPQH",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_XPBK,MF_BYCOMMAND|MF_STRING,IDM_XPBK,LoadItemStringW(L"IDM_XPBK",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_XPDISGUISE,MF_BYCOMMAND|MF_STRING,IDM_XPDISGUISE,LoadItemStringW(L"IDM_XPDISGUISE",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_HELP_CLEAN,MF_BYCOMMAND|MF_STRING,IDM_HELP_CLEAN,LoadItemStringW(L"IDM_HELP_CLEAN",pWMenuText));
	ModifyMenuW(hHelpMenu,IDM_HELP_HIDEIP,MF_BYCOMMAND|MF_STRING,IDM_HELP_HIDEIP,LoadItemStringW(L"IDM_HELP_HIDEIP",pWMenuText));



	
	ModifyMenuW(hWLMenu,IDM_AUTORESERVE,MF_BYCOMMAND|MF_STRING,IDM_AUTORESERVE,
		LoadItemStringW(L"IDM_AUTORESERVE",pWMenuText));
	ModifyMenuW(hWLMenu,15,MF_BYPOSITION|MF_STRING,0,
		LoadItemStringW(L"IDM_SETWNDALPHA",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_TRANSPARENT_RESTORE,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_RESTORE,
		LoadItemStringW(L"IDM_TRANSPARENT_RESTORE",pWMenuText));
	ModifyMenuW(hWLMenu,IDM_TRANSPARENT_LOW,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_LOW,
		LoadItemStringW(L"IDM_TRANSPARENT_LOW",pWMenuText));
	ModifyMenuW(hWLMenu,IDM_TRANSPARENT_MIDDLELOW,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MIDDLELOW,
		LoadItemStringW(L"IDM_TRANSPARENT_MIDDLELOW",pWMenuText));
	ModifyMenuW(hWLMenu,IDM_TRANSPARENT_MIDDLE,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MIDDLE,
		LoadItemStringW(L"IDM_TRANSPARENT_MIDDLE",pWMenuText));
	ModifyMenuW(hWLMenu,IDM_TRANSPARENT_MIDDLEHIGH,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MIDDLEHIGH,
		LoadItemStringW(L"IDM_TRANSPARENT_MIDDLEHIGH",pWMenuText));
	ModifyMenuW(hWLMenu,IDM_TRANSPARENT_MOSTHIGH,MF_BYCOMMAND|MF_STRING,IDM_TRANSPARENT_MOSTHIGH,
		LoadItemStringW(L"IDM_TRANSPARENT_MOSTHIGH",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_TOBK,MF_BYCOMMAND|MF_STRING,IDM_TOBK,
		LoadItemStringW(L"IDM_TOBK",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_BKWNDHOTKEY,MF_BYCOMMAND|MF_STRING,IDM_BKWNDHOTKEY,
		LoadItemStringW(L"IDM_BKWNDHOTKEY",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_UNREGISTERHK,MF_BYCOMMAND|MF_STRING,IDM_UNREGISTERHK,
		LoadItemStringW(L"IDM_UNREGISTERHK",pWMenuText));


	ModifyMenuW(hWLMenu,IDM_CHANGEICON,MF_BYCOMMAND|MF_STRING,IDM_CHANGEICON,
		LoadItemStringW(L"IDM_CHANGEICON",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_CHANGETITLE,MF_BYCOMMAND|MF_STRING,IDM_CHANGETITLE,
		LoadItemStringW(L"IDM_CHANGETITLE",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_RESTOREICONTITLE,MF_BYCOMMAND|MF_STRING,IDM_RESTOREICONTITLE,
		LoadItemStringW(L"IDM_RESTOREICONTITLE",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_PROPERTY,MF_BYCOMMAND|MF_STRING,IDM_PROPERTY,
		LoadItemStringW(L"IDM_PROPERTY",pWMenuText));

	ModifyMenuW(hWLMenu,IDM_SETTOTOP,MF_BYCOMMAND|MF_STRING,IDM_SETTOTOP,
		LoadItemStringW(L"IDM_SETTOTOP",pWMenuText));
	ModifyMenuW(hWLMenu,IDM_ADDAUTOHIDE,MF_BYCOMMAND|MF_STRING,IDM_ADDAUTOHIDE,
		LoadItemStringW(L"IDM_ADDAUTOHIDE",pWMenuText));






	ModifyMenuW(hTrayListMenu_Sub,IDM_TAB_TRAY_LIST,MF_BYCOMMAND|MF_STRING,IDM_TAB_TRAY_LIST,
		LoadItemStringW(L"IDM_TAB_TRAY_LIST",pWMenuText));

// 	ModifyMenuW(hTrayListMenu_Sub,IDM_TAB_TRAY_CHANGEICON,MF_BYCOMMAND|MF_STRING,IDM_TAB_TRAY_CHANGEICON,
// 		LoadItemStringW(L"IDM_TAB_TRAY_CHANGEICON",pWMenuText));


	ModifyMenuW(hTrayListMenu_Sub,IDM_TAB_TRAY_ADDAUTOHIDE,MF_BYCOMMAND|MF_STRING,IDM_TAB_TRAY_ADDAUTOHIDE,
		LoadItemStringW(L"IDM_TAB_TRAY_ADDAUTOHIDE",pWMenuText));



	ModifyMenuW(hBKListMenu_Sub,IDM_BKITEM_SHOW,MF_BYCOMMAND|MF_STRING,IDM_BKITEM_SHOW,
		LoadItemStringW(L"IDM_BKITEM_SHOW",pWMenuText));

	ModifyMenuW(hBKListMenu_WizardSub,IDM_BKTAB_BKWIZARD,MF_BYCOMMAND|MF_STRING,IDM_BKTAB_BKWIZARD,
		LoadItemStringW(L"IDM_BKTAB_BKWIZARD",pWMenuText));



	ModifyMenuW(hProcessMenu_Sub,IDM_PROCESSLIST_HIDE,MF_BYCOMMAND|MF_STRING,IDM_PROCESSLIST_HIDE,
		LoadItemStringW(L"IDM_PROCESSLIST_HIDE",pWMenuText));

	ModifyMenuW(hProcessMenu_Sub,IDM_PROCESSLIST_END,MF_BYCOMMAND|MF_STRING,IDM_PROCESSLIST_END,
		LoadItemStringW(L"IDM_PROCESSLIST_END",pWMenuText));



	ModifyMenuW(hAutoHideListMenu_Sub,IDM_AUTOHIDE_ADD,MF_BYCOMMAND|MF_STRING,IDM_AUTOHIDE_ADD,
		LoadItemStringW(L"IDM_AUTOHIDE_ADD",pWMenuText));
	
	ModifyMenuW(hAutoHideListMenu_Sub,IDM_AUTOHIDE_DEL,MF_BYCOMMAND|MF_STRING,IDM_AUTOHIDE_DEL,
		LoadItemStringW(L"IDM_AUTOHIDE_DEL",pWMenuText));




	
	HMENU hLangMenu = GetSubMenu(hToolMenu,2);
	//hLangMenu = GetSubMenu(hLangMenu,0);
	ModifyMenuW(hLangMenu,IDM_ACQUIRELANGUAGE,MF_BYCOMMAND|MF_STRING,IDM_ACQUIRELANGUAGE,
			LoadItemStringW(L"IDM_ACQUIRELANGUAGE",pWMenuText));



	


	delete [] pWMenuText;
	DrawMenuBar(hMainDialog);
	InvalidateRect(hMainDialog,NULL,TRUE);
	return 1;
}

int SetDlgItemStringW(HWND hwnd,UINT uDlgID,UINT uCtrlID)
{
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"String");

	wchar_t * szDefault = L"";
	DWORD dwSize = 1024;
	wchar_t * pData = new wchar_t[dwSize];


	wchar_t * pLangFilePath = new wchar_t[1024];
	GetCurrentDirectory_HD(1024,pLangFilePath);
	wcscat(pLangFilePath,L"\\Language\\");
	wcscat(pLangFilePath,FFD.cFileName);

	wchar_t * szKey = new wchar_t[512];
	memset(szKey,0,512 * sizeof(wchar_t));
	UINT iCtrlID = uCtrlID;
	swprintf(szKey,L"IDD%d_%d",uDlgID,iCtrlID);


	if(GetPrivateProfileStringW(szSection,szKey,szDefault,
			pData,dwSize,pLangFilePath) != 0)
	{
		SetWindowTextW(hwnd,pData);
	}
	else
	{
		delete [] pLangFilePath;
		delete [] pData;
		delete [] szKey;

		return 0;
	}

	delete [] pLangFilePath;
	delete [] pData;
	delete [] szKey;
	
	return 1;
}




int MakeLanguageMenu()
{
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");

	wchar_t szKey[] = L"Language";
	DWORD dwSize = 1000;	
	HMENU hToolMenu = GetSubMenu(GetMenu(hMainDialog),4);

	HMENU hLangMenu = GetSubMenu(hToolMenu,2);
	g_hLangMenu = hLangMenu;

	int i = 0,iMenuCount = 0;
	iMenuCount = GetMenuItemCount(hLangMenu);
	for(i = 0;i < iMenuCount-1;i ++)
	{
		DeleteMenu(hLangMenu,0,MF_BYPOSITION);
	}
	
	wchar_t * pMenuText = new wchar_t[1024];
	ModifyMenuW(hLangMenu,IDM_ACQUIRELANGUAGE,MF_BYCOMMAND|MF_STRING,IDM_ACQUIRELANGUAGE,
			LoadItemStringW(L"IDM_ACQUIRELANGUAGE",pMenuText,L"&Get More Language pack now..."));
	delete [] pMenuText;

	MENUITEMINFOW MII;
	WIN32_FIND_DATAW FFD2;
	wchar_t * szFilePath = new wchar_t[1024];
	memset(szFilePath,0,1024 * sizeof(wchar_t));
	wchar_t * cIniFile = new wchar_t[1024];
	memset(cIniFile,0,1024 * sizeof(wchar_t));
	GetCurrentDirectory_HD(1024,cIniFile);
	wcscat(cIniFile,L"\\Language\\*.ini");
	bool bFound = false;
	int iLangCount = 0;
	UINT g_LangMenuID = 40300;
	HANDLE hFindFile = FindFirstFileW(cIniFile,&FFD2);
	DWORD dwErr = 0;
	BOOL bRet = TRUE;
	while(bRet && hFindFile != INVALID_HANDLE_VALUE)
	{
		wchar_t szValue[1024];
		memset(szValue,0,1024 * sizeof(wchar_t));
		GetCurrentDirectory_HD(1024,szFilePath);
		wcscat(szFilePath,L"\\Language\\");
		wcscat(szFilePath,FFD2.cFileName);
		if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,
			1024,szFilePath) != 0)
		{
			memset(&MII,0,sizeof(MII));
			MII.cbSize = sizeof(MII);
			MII.fMask = MIIM_STRING|MIIM_ID|MIIM_TYPE;
			MII.fType = MFT_STRING;
			MII.wID = g_LangMenuID + iLangCount;
			MII.cch = wcslen(szValue);
			MII.dwTypeData =  szValue;
			
//			if(InsertMenuItem(hLangMenu,0,TRUE,&MII) == 0)
//			{
//				dwErr = GetLastError();
//			}
			if(iLangCount == 0)
			{
				InsertMenuW(hLangMenu,IDM_ACQUIRELANGUAGE,MF_BYCOMMAND|MF_SEPARATOR,0,NULL);
			}
			if(wcscmp(FFD.cFileName,FFD2.cFileName) == 0)
			{
				InsertMenuW(hLangMenu,0,MF_BYPOSITION|MF_STRING|MF_CHECKED,g_LangMenuID + iLangCount,szValue);
				g_CurrLangID = g_LangMenuID + iLangCount;
				g_OldLangID = g_CurrLangID;
			}
			else
			{
				InsertMenuW(hLangMenu,0,MF_BYPOSITION|MF_STRING,g_LangMenuID + iLangCount,szValue);
			}
			dwErr = GetLastError();
			iLangCount ++;
			g_iLangCount = iLangCount;
		}
		bRet = FindNextFileW(hFindFile,&FFD2);
	}
	FindClose(hFindFile);
	delete [] szFilePath;
	delete [] cIniFile;
	
	return 1;
}


int HDMessageBoxW(wchar_t * cStringID,wchar_t * cDefault,HWND hParentWnd,UINT uType)
{
	wchar_t * pText = new wchar_t[1024];
	memset(pText,0, 1024 * sizeof(wchar_t));
	wchar_t * pTitle = new wchar_t [1024];
	wcscpy(pTitle,LoadItemStringW(L"IDS_MAINDIALOG_TITLE",pTitle,L"HideDragon"));
	if(hParentWnd == NULL)
	{
		hParentWnd = hMainDialog;
	}
	int iRet = MessageBoxW(hParentWnd,LoadItemStringW(cStringID,pText,cDefault),pTitle,uType);
	delete [] pTitle;
	delete [] pText;
	
	
	return iRet;
}







int LoadLanguageBaseID(LANGID LangID)
{
	wchar_t szSection[20];
	memset(szSection,0,20 * sizeof(wchar_t));
	wcscpy(szSection,L"Setting");


	wchar_t szKey[] = L"LanguageID",*szLang;
	DWORD dwSize = 1000;
	
	wchar_t pLanguageID[128];
	memset(pLanguageID,0,128 * sizeof(wchar_t));
	swprintf(pLanguageID,L"%04X",LangID);
	

	
	if(pLanguageID != NULL)
	{
		szLang = pLanguageID;
	}
	else
	{
	}
	
	
	wchar_t * szFilePath = new wchar_t[1024];
	memset(szFilePath,0,1024 * sizeof(wchar_t));
	wchar_t * cIniFile = new wchar_t[1024];
	memset(cIniFile,0,1024 * sizeof(wchar_t));
	GetCurrentDirectory_HD(1024,cIniFile);
	wcscat(cIniFile,L"\\Language\\*.ini");
	bool bFound = false;
	HANDLE hFindFile = FindFirstFileW(cIniFile,&FFD);
	BOOL bRet = TRUE;
	while(bRet && hFindFile != INVALID_HANDLE_VALUE)
	{
		wchar_t * szValue = new wchar_t[1024];
		wchar_t * szCurrentLang = new wchar_t[1024];
		memset(szCurrentLang,0,1024 * sizeof(wchar_t));
		memset(szValue,0,1024 * sizeof(wchar_t));
		GetCurrentDirectory_HD(1024,szFilePath);
		wcscat(szFilePath,L"\\Language\\");
		wcscat(szFilePath,FFD.cFileName);
		if(GetPrivateProfileStringW(szSection,szKey,L"",szValue,
			1024,szFilePath) != 0)
		{
			if(wcscmp(szValue,szLang) == 0)
			{
				bFound = true;
				if(GetPrivateProfileStringW(szSection,L"Language",L"",szCurrentLang,
					1024,szFilePath) != 0)
				{
					
				}

				memset(szFilePath,0,1024 * sizeof(wchar_t));
				GetCurrentDirectory_HD(1024,szFilePath);
				wcscat(szFilePath,L"\\Language\\");
				wcscat(szFilePath,L"LangSetting.ini");

				if(WritePrivateProfileStringW(szSection,L"CurrentLanguageID",szValue,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}
				if(WritePrivateProfileStringW(szSection,L"CurrentLanguage",szCurrentLang,szFilePath) == 0)
				{
					OutputDebugString(L"Write Language Error.");
				}

								
				delete [] szValue;
				delete [] szCurrentLang;

				break;
			}
		}
		bRet = FindNextFileW(hFindFile,&FFD);
		DWORD dwErr = 0;
		if(bRet == 0)
		{
			dwErr = GetLastError();
		}
		delete [] szValue;
		delete [] szCurrentLang;
	}
	FindClose(hFindFile);
	delete [] szFilePath;
	delete [] cIniFile;

	if(!bFound)
	{
		return 0;
	}
	return 1;
}


int UpdateLanguagePack()
{
	if(g_hUpdateLangHandle == NULL && g_hUpdateHandle == NULL && hNetworkThread == NULL)
	{
		g_hUpdateLangHandle = (HANDLE)_beginthreadex(NULL,0,UpdateLangPack,NULL,0,NULL);
		if(g_hUpdateLangHandle == INVALID_HANDLE_VALUE)
		{
			g_hUpdateLangHandle = NULL;
		}
	}
	
	return 1;
} 

unsigned __stdcall UpdateLangPack(void * pParam)
{
	DWORD dwErr = 0;
	BOOL bRet = FALSE;
	int iRet = GetFileFromServer("/en/Language/LanguageList.ini");
	if(iRet == 1)
	{
		wchar_t * szFilePath = new wchar_t[1024];
		memset(szFilePath,0,1024);
// 		GetCurrentDirectory_HD(1024,szFilePath);
		SHGetSpecialFolderPath(NULL, szFilePath, CSIDL_PERSONAL, FALSE);
		wcscat(szFilePath,L"\\HideDragon");
		CreateDirectory(szFilePath,NULL);

		wcscat(szFilePath,L"\\Downloads\\");
		wcscat(szFilePath,L"LanguageList.ini");

		wchar_t * szValue = new wchar_t[1024];
		memset(szValue,0,1024 * sizeof(wchar_t));
		
		wchar_t * cLangDir = new wchar_t[1024];
		memset(cLangDir,0,1024 * sizeof(wchar_t));
		GetCurrentDirectory_HD(1024,cLangDir);
		wcscat(cLangDir,L"\\Language\\");
		bRet = CreateDirectory(cLangDir,NULL);
		if(bRet == FALSE)
		{
			dwErr = GetLastError();
			if(dwErr == ERROR_ALREADY_EXISTS)
			{
				
			}
		}
		delete [] cLangDir;

		if(GetPrivateProfileString(L"Setting",L"FileNumber",L"",szValue,
			1024,szFilePath) != 0)
		{
			int iFileNum = _wtoi(szValue);
			for(int i = 1;i <= iFileNum;i ++)
			{
				wchar_t * szKey = new wchar_t[256],cNum[10],* szVersion = new wchar_t [256],szVersionValue[10];
				memset(szKey,0,256 * sizeof(wchar_t));
				memset(szVersion,0,256 * sizeof(wchar_t));
				memset(cNum,0,10 * sizeof(wchar_t));
				memset(szVersionValue,0,10 * sizeof(wchar_t));
				wcscpy(szKey,L"LanguageFile");
				wcscat(szKey,_itow(i,cNum,10));
				wcscpy(szVersion,L"Version");
				wcscat(szVersion,_itow(i,cNum,10));
				GetPrivateProfileString(L"Setting",szKey,L"",szValue,
					1024,szFilePath);
				GetPrivateProfileString(L"Setting",szVersion,L"",szVersionValue,
					1024,szFilePath);

				wchar_t * cLangFileName = new wchar_t[1024];
				memset(cLangFileName,0,1024 * sizeof(wchar_t));
				GetCurrentDirectory_HD(1024,cLangFileName);
				wcscat(cLangFileName,L"\\Language\\");
				wcscat(cLangFileName,szValue);
				
				HANDLE hLangFile = CreateFile(cLangFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
					NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if(hLangFile == INVALID_HANDLE_VALUE)
				{
					char * cServerFile = new char[512];
					memset(cServerFile,0,512);
					strcpy(cServerFile,"/en/Language/");
					char * szMValue = new char[1024];
					memset(szMValue,0,1024);
					WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,szValue,1024,szMValue,1024,NULL,FALSE);
					strcat(cServerFile,szMValue);//szValue);
					delete [] szMValue;
					
					iRet = GetFileFromServer(cServerFile);
					if(iRet == 1)
					{
						wchar_t * cLocalFile = new wchar_t[1024];
						memset(cLocalFile,0,1024 * sizeof(wchar_t));
// 						GetCurrentDirectory_HD(1024,cLocalFile);
						SHGetSpecialFolderPath(NULL, cLocalFile, CSIDL_PERSONAL, FALSE);
						wcscat(cLocalFile,L"\\HideDragon");
						CreateDirectory(cLocalFile,NULL);

						wcscat(cLocalFile,L"\\Downloads\\");
						wcscat(cLocalFile,szValue);
						CopyFile(cLocalFile,cLangFileName,FALSE);
						DeleteFile(cLocalFile);
						delete [] cLocalFile;
					}
					else
					{
						//Process Error;
					}
					delete [] cServerFile;
				}
				else
				{
					wchar_t szLocalVersion[20];
					memset(szLocalVersion,0,20 * sizeof(wchar_t));
					GetPrivateProfileString(L"Setting",L"Version",L"",szLocalVersion,1024,cLangFileName);
					char * mbValue = new char[1024];
					memset(mbValue,0,1024);
					WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,szLocalVersion,20,mbValue,1024,NULL,FALSE);
					double iLocalVersion = (float)atof(mbValue);
					delete [] mbValue;

					mbValue = new char[1024];
					memset(mbValue,0,1024);
					WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,szVersionValue,10,mbValue,1024,NULL,FALSE);
					double iServerVersion = (float)atof(mbValue);
					delete [] mbValue;
					//double iLocalVersion = _wtof(szLocalVersion);
					//double iServerVersion = _wtof(szVersionValue);
					if(iServerVersion > iLocalVersion)
					{
						wchar_t * cLangFile = new wchar_t[512];
						memset(cLangFile,0,512 * sizeof(wchar_t));
						wcscpy(cLangFile,L"/en/Language/");
						wcscat(cLangFile,szValue);
						char * szMValue = new char[1024];
						memset(szMValue,0,1024);
						WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,cLangFile,1024,szMValue,1024,NULL,FALSE);
						int iRet = GetFileFromServer(szMValue);//cLangFile);
						delete [] szMValue;
						if(iRet == 1)
						{
							wchar_t * cLocalFile = new wchar_t[1024];
							memset(cLocalFile,0,1024 * sizeof(wchar_t));
// 							GetCurrentDirectory_HD(1024,cLocalFile);
							SHGetSpecialFolderPath(NULL, cLocalFile, CSIDL_PERSONAL, FALSE);
							wcscat(cLocalFile,L"\\HideDragon");
							CreateDirectory(cLocalFile,NULL);

							wcscat(cLocalFile,L"\\Downloads\\");
							wcscat(cLocalFile,szValue);
							CopyFile(cLocalFile,cLangFileName,FALSE);
							DeleteFile(cLocalFile);
							delete [] cLocalFile;
						}
						else
						{
							//Process Error;
						}
						delete [] cLangFile;
					}
				}

				delete [] szKey;
				delete [] szVersion;
				delete [] cLangFileName;
			}
		}
		DeleteFile(szFilePath);
// 		GetCurrentDirectory_HD(1024,szFilePath);
		SHGetSpecialFolderPath(NULL, szFilePath, CSIDL_PERSONAL, FALSE);
		wcscat(szFilePath,L"\\HideDragon");
		CreateDirectory(szFilePath,NULL);

		wcscat(szFilePath,L"\\Downloads\\");

		RemoveDirectory(szFilePath);
		HDMessageBoxW(L"IDS_MESSAGEBOX_UPDATELANGSUCESS",L"Update Language Pack Successful.");
		MakeLanguageMenu();
		delete [] szFilePath;
		delete [] szValue;
	}
	else
	{
		//HDMessageBox("IDS_MESSAGEBOX_UPDATELANGFAILURE","更新语言包失败，请检查网络后重试。");
	}
	CloseHandle(g_hUpdateLangHandle);
	g_hUpdateLangHandle = NULL;

	return 1;
}





unsigned __stdcall AutoUpdateThreadProc(void * pParam)
{
	OutputDebugString(L"g_hUpdateHandle BEGIN.");
	wchar_t * cProgramFile = new wchar_t[1024];
	memset(cProgramFile,0,1024 * sizeof(wchar_t));
	wchar_t * cArcName = new wchar_t[1024];
	memset(cArcName,0,1024 * sizeof(wchar_t));
// 	GetCurrentDirectory_HD(1024,cArcName);
	SHGetSpecialFolderPath(NULL, cArcName, CSIDL_PERSONAL, FALSE);
	wcscat(cArcName,L"\\HideDragon");
	CreateDirectory(cArcName,NULL);
	wcscat(cArcName,L"\\Downloads\\");
	wcscat(cArcName,L"hidedragon.rar");
	wcscpy(cProgramFile,L"/en/HideDragon.rar");//  //en//HideDragon.rar //FIXME
	HANDLE hRarFile = CreateFile(cArcName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hRarFile != NULL && hRarFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hUpdateHandle);
		delete [] cProgramFile;
		delete [] cArcName;
		g_hUpdateHandle = NULL;	
		return 0;
	}
	char * szMValue = new char[1024];
	memset(szMValue,0,1024);
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,cProgramFile,1024,szMValue,1024,NULL,FALSE);
	int iRet = GetFileFromServer(szMValue);//cLangFile);
	delete [] szMValue;

	//int iRet = GetFileFromServer(cProgramFile);
	if(iRet != 1)
	{
		CloseHandle(g_hUpdateHandle);
		DeleteFile(cArcName);
		g_hUpdateHandle = NULL;
		delete [] cProgramFile;
		delete [] cArcName;
		return 0;
	}
	delete [] cProgramFile;

	/////////////////////////////////////////////////////////////////////////
	RARHeaderData rarHeader;
	RAROpenArchiveDataEx rarArchiveData;
	enum { EXTRACT, TEST, PRINT };
	char * RARBuff = (char*)VirtualAlloc(NULL,16*1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	HANDLE hArcData = NULL;
	int RHCode= 0,PFCode = 0;
	long rarMode = 0;


	
	memset((void*)&rarArchiveData,0,sizeof(rarArchiveData));
	rarArchiveData.ArcNameW = (wchar_t*)cArcName;
	rarArchiveData.CmtBuf = RARBuff;
	rarArchiveData.CmtBufSize = sizeof(16*1024);
	rarArchiveData.OpenMode = RAR_OM_EXTRACT;

	hArcData = RAROpenArchiveEx(&rarArchiveData);

	if (rarArchiveData.OpenResult != 0)
	{
		CloseHandle(g_hUpdateHandle);
		g_hUpdateHandle = NULL;
		delete [] cArcName;
		VirtualFree(RARBuff,NULL,MEM_RELEASE);
		DeleteFile(cArcName);
		return 0;
	}
	rarMode = EXTRACT;
	RARSetCallback(hArcData,rarCallback,(LONG)&rarMode);
	rarHeader.CmtBuf = NULL;
	while((RHCode = RARReadHeader(hArcData,&rarHeader))==0)
	{
		PFCode = RARProcessFileW(hArcData,RAR_EXTRACT,L"Downloads\\temp\\",NULL);
		if (PFCode != 0)
		{
			CloseHandle(g_hUpdateHandle);
			g_hUpdateHandle = NULL;
			delete [] cArcName;
			VirtualFree(RARBuff,NULL,MEM_RELEASE);
			DeleteFile(cArcName);
			return 0;
		}
	}
	if(RHCode == ERAR_BAD_DATA)
	{
		CloseHandle(g_hUpdateHandle);
		g_hUpdateHandle = NULL;
		delete [] cArcName;
		VirtualFree(RARBuff,NULL,MEM_RELEASE);
		DeleteFile(cArcName);
		return 0;
	}
	RARCloseArchive(hArcData);
	//DeleteFile(cArcName);
	delete [] cArcName;
	VirtualFree(RARBuff,NULL,MEM_RELEASE);
	CloseHandle(g_hUpdateHandle);
	g_hUpdateHandle = NULL;
	OutputDebugString(L"g_hUpdateHandle END.");
	return 1;
}


int CALLBACK rarCallback(UINT msg,LONG UserData,LONG P1,LONG P2)
{
	switch(msg)
	{
	case UCM_PROCESSDATA:
		return 0;
	}
	return 0;
}




int IsHaveUpdatedFile()
{
	wchar_t * cUpdatedFile = new wchar_t[1024],* cTemp = NULL;
	DWORD dwErr = 0;
	memset(cUpdatedFile,0,1024 * sizeof(wchar_t));
	GetModuleFileName(NULL,cUpdatedFile,1024);
	PathRemoveFileSpec(cUpdatedFile);
//	cTemp = wcschr(cUpdatedFile,'\\');
//	memset(cTemp,0,wcslen(cTemp) /* sizeof(wchar_t)*/);
	SHGetSpecialFolderPath(NULL, cUpdatedFile, CSIDL_PERSONAL, FALSE);
	wcscat(cUpdatedFile,L"\\HideDragon");
	CreateDirectory(cUpdatedFile,NULL);
	wcscat(cUpdatedFile,L"\\Downloads\\temp\\HideDragon.exe");
	HANDLE hUpdatedFile = CreateFile(cUpdatedFile,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hUpdatedFile!= INVALID_HANDLE_VALUE)
	{
		CloseHandle(hUpdatedFile);
		cTemp = wcsrchr(cUpdatedFile,L'\\');
		memset(cTemp,0,wcslen(cTemp));
		wcscat(cUpdatedFile,L"\\HDUpdate.exe");
		if(sOP.iAutoUpdate == ID_AUTOUPDATE_TIP)
		{
			ShellExecute(NULL,L"open",cUpdatedFile,L"-U",NULL,SW_SHOWNORMAL);
		}
		else
		{
			ShellExecute(NULL,L"open",cUpdatedFile,L"-N",NULL,SW_SHOWNORMAL);
			//dwErr = GetLastError();
		}
		delete [] cUpdatedFile;
		exit(0);
	}
	delete [] cUpdatedFile;
	return 0;
}




int IsHaveTempDirectory()
{
	WIN32_FIND_DATA UpdateFFD;
	DWORD dwErr = 0;
	int iDelCount = 0;
	wchar_t * cUpdateDir = new wchar_t[1024],* cTemp = NULL;
	memset(cUpdateDir,0,1024 * sizeof(wchar_t));
	GetModuleFileNameW(NULL,cUpdateDir,1024);
	cTemp = wcsrchr(cUpdateDir,'\\');
	memset(cTemp,0,wcslen(cTemp));

	wcscat(cUpdateDir,L"\\Downloads\\Temp\\");
	
	wchar_t * cUpdateFile = new wchar_t[1024];
	memset(cUpdateFile,0,1024 * sizeof(wchar_t));
	wcscpy(cUpdateFile,cUpdateDir);
	wcscat(cUpdateFile,L"*.*");
	memset(&UpdateFFD,0,sizeof(UpdateFFD));
	HANDLE hUpdateFile = FindFirstFile(cUpdateFile,&UpdateFFD);
	BOOL bRet = TRUE,bDelRet = FALSE;
	while(bRet == TRUE && hUpdateFile != INVALID_HANDLE_VALUE)
	{
		wcscpy(cUpdateFile,cUpdateDir);
		wcscat(cUpdateFile,UpdateFFD.cFileName);
retrydel:		bDelRet = DeleteFile(cUpdateFile);
		if(bDelRet == 0)
		{
			if(wcscmp(_wcsupr(UpdateFFD.cFileName),L"HDUPDATE.EXE") == 0)
			{
				dwErr = GetLastError();
				if(dwErr == 5)
				{
					iDelCount ++;
					if(iDelCount < 10)
					{
						goto retrydel;
					}
				}
			}
		}
		wcscpy(cUpdateFile,cUpdateDir);
		wcscat(cUpdateFile,L"*.*");
		bRet = FindNextFile(hUpdateFile,&UpdateFFD);
	}
	bRet = FindClose(hUpdateFile);
	
	memset(cUpdateFile,0,1024 * sizeof(wchar_t));
	GetModuleFileNameW(NULL,cUpdateFile,1024);
	cTemp = wcsrchr(cUpdateFile,'\\');
	memset(cTemp,0,wcslen(cTemp));
	wcscat(cUpdateFile,L"\\Downloads\\HideDragon.rar");
	wchar_t * cLngDir = new wchar_t[1024];
	memset(cLngDir,0,1024* sizeof(wchar_t));
	wcscpy(cLngDir,cUpdateDir);
	wcscat(cLngDir,L"Language");
	DeleteDir(cLngDir);
	delete [] cLngDir;
	DeleteFile(cUpdateFile);
	RemoveDirectory(cUpdateDir);
	

	delete [] cUpdateDir;
	delete [] cUpdateFile;

	return 1;
}


int SetWindowTransparent(HWND hWnd,int iLevel)
{
	DWORD dwErr = 0;
	LONG lStyle = 0;
	BYTE iAlpha = 0,iOldAlpha = 0;
	BOOL bRet = FALSE;
	DWORD dwFlag = 0;
//	COLORREF cr;
//	if(pGetLayeredWindowAttributes == NULL)
//	{
//		GetFunctionPointer();
//	}
//	bRet = pGetLayeredWindowAttributes(hWnd,&cr,&iOldAlpha,&dwFlag);
//	if(bRet == FALSE)
//	{
//		dwErr = GetLastError();
//	}

	switch(iLevel)
	{
	case ID_WNDALPHA_MOSTHIGH:
		iAlpha = 0;
		break;
	case ID_WNDALPHA_MIDDLEHIGH:
		iAlpha = 60;
		break;
	case ID_WNDALPHA_MIDDLE:
		iAlpha = 125;	
		break;
	case ID_WNDALPHA_MIDDLELOW:
		iAlpha = 160;
		break;
	case ID_WNDALPHA_LOW:
		iAlpha = 200;
		break;
	case ID_WNDALPHA_RESTORE:
		iAlpha = 255;
		break;
	}
//	if((bRet != FALSE) && iOldAlpha != 255)
//	{
//		lStyle = GetWindowLong(hWnd,GWL_EXSTYLE);
//		lStyle = lStyle ^ WS_EX_LAYERED;
//		SetWindowLong(hWnd,GWL_EXSTYLE,lStyle);
//		bRet = SetLayeredWindowAttributes(hWnd,0,iAlpha,LWA_ALPHA|LWA_COLORKEY);
//		if(iAlpha == 255)
//			return 1;
//	}
//	
//	if(!((bRet == FALSE) && iAlpha == 255))
//	{
//		lStyle = GetWindowLong(hWnd,GWL_EXSTYLE);
//		lStyle = lStyle ^ WS_EX_LAYERED;
//		SetWindowLong(hWnd,GWL_EXSTYLE,lStyle);
//
//		bRet = SetLayeredWindowAttributes(hWnd,0,iAlpha,LWA_ALPHA|LWA_COLORKEY);
//		if(bRet == FALSE)
//		{
//			dwErr = GetLastError();
//		}
//	}

	if(iAlpha == 255)
	{
		lStyle = GetWindowLong(hWnd,GWL_EXSTYLE);
		lStyle = lStyle ^ WS_EX_LAYERED;
		SetWindowLong(hWnd,GWL_EXSTYLE,lStyle);
		bRet = SetLayeredWindowAttributes(hWnd,0,iAlpha,LWA_ALPHA|LWA_COLORKEY);

		return 1;
	}
	lStyle = GetWindowLong(hWnd,GWL_EXSTYLE);
	lStyle = lStyle ^ WS_EX_LAYERED;
	SetWindowLong(hWnd,GWL_EXSTYLE,lStyle);
	bRet = SetLayeredWindowAttributes(hWnd,0,iAlpha,LWA_ALPHA|LWA_COLORKEY);
	
	if(bRet == FALSE)
	{
		lStyle = GetWindowLong(hWnd,GWL_EXSTYLE);
		lStyle = lStyle ^ WS_EX_LAYERED;
		SetWindowLong(hWnd,GWL_EXSTYLE,lStyle);

		bRet = SetLayeredWindowAttributes(hWnd,0,iAlpha,LWA_ALPHA|LWA_COLORKEY);
		if(bRet == FALSE)
		{
			dwErr = GetLastError();
		}
	}



	return 1;
}

int SetTipText(wchar_t * cID,wchar_t * pDefault)
{
	wchar_t * pText = new wchar_t[1024];
	SetWindowTextW(hUsageTip,LoadItemStringW(cID,pText,pDefault));
	delete [] pText;

	return 1;
}





int ClearTipTextArray()
{
	KillTimer(hMainDialog,IDT_SETTIPTEXT);
	int i = 0;
	for(i = 0;i < g_iTipCount;i ++)
	{
		delete [] g_pTipText[i][0];
		delete [] g_pTipText[i][1];
	}
	g_iTipCount = 0;
	SetTimer(hMainDialog,IDT_SETTIPTEXT,30*1000,SetTipTextProc);


	return 1;
}

int AppendTipText(wchar_t * pIDS,wchar_t * pDefault)
{
	KillTimer(hMainDialog,IDT_SETTIPTEXT);
	
	g_pTipText[g_iTipCount][0] = new wchar_t[256];
	memset(g_pTipText[g_iTipCount][0],0,256 * sizeof(wchar_t));
	wcscpy(g_pTipText[g_iTipCount][0],pIDS);

	
	g_pTipText[g_iTipCount][1] = new wchar_t[1024];
	memset(g_pTipText[g_iTipCount][1],0,1024 * sizeof(wchar_t));
	wcscpy(g_pTipText[g_iTipCount][1],pDefault);
	
	g_iTipCount ++;
	SetTimer(hMainDialog,IDT_SETTIPTEXT,30*1000,SetTipTextProc);

	
	return 1;
}


int GetFunctionPointer()
{
	hUser32 = LoadLibrary(L"user32.dll");
	if(hUser32 == NULL || hUser32 == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	pGetLayeredWindowAttributes = (GETLWA *)GetProcAddress(hUser32,"GetLayeredWindowAttributes");

	if(pGetLayeredWindowAttributes == NULL)
	{
		return 0;
	}
	
	return 1;
}


int BKCurrentWindow(int iSelMark)
{
	wchar_t * cTitle = new wchar_t[2048],cPID[20];
	memset(cTitle,0,2048 * sizeof(wchar_t));
	memset(cPID,0,20 * sizeof(wchar_t));

	ListView_GetItemText(hListView,iSelMark,0,cTitle,2040);
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	ConstructBKSW(cPID,cTitle);
	delete [] cTitle;
	HideSelected(&sBKSel,1,0);
	Switch2BKTab();
//	ExtendWindow();
	SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
	
	return 1;
}




int	Switch2BKTab()
{
	//SendMessage(g_Tab,TCM_SETCURSEL,2,0);
	if(sOP.bBeginnerModel == 1)
	{
		TabCtrl_SetCurFocus(g_Tab,2);
	}

	return 1;
}




int ConstructBKSW(wchar_t * cPID,wchar_t * cTitle, HWND hWnd /*= NULL*/)
{
	wchar_t cCurrentPID[20];
	int i = 0,j = 0;
	WndAndHandle *pNext = NULL,*pTemp = NULL,*pBKSel = NULL,* pTemp2 = NULL;

	sBKSel = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pTemp2 = sBKSel;
	for(i = 0;i<100;i++)
	{
		sBKSel[i].cWndTitle = (wchar_t *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sBKSel[i].cIconTip = (wchar_t*)new wchar_t [1024];
		memset(sBKSel[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(sBKSel[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(sBKSel[i].cProcessName,0,30 * sizeof(wchar_t));
		sBKSel[i].Next = NULL;
	}
	
	j = 0;
	for(i = 0;i < iBaSW;i ++)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0 && sW[i].bHide == 0 &&
			wcscmp(sW[i].cProcessName,L"explorer.exe") != 0 &&
			wcscmp(sW[i].cProcessName,L"iexplore.exe") != 0)
		{
			if(wcscmp(sW[i].cWndTitle,L"") == 0)
			{
				wcscpy(sBKSel[j].cWndTitle,sW[i].cProcessName);
			}
			else
			{
				wcscpy(sBKSel[j].cWndTitle,sW[i].cWndTitle);
			}
			wcscpy(sBKSel[j].cProcessName,sW[i].cProcessName);
			sBKSel[j].hWnd = sW[i].hWnd;
			sBKSel[j].lPID = sW[i].lPID;
			sBKSel[j].iWindowType = sW[i].iWindowType;
			sW[i].bToBk = 1;
			if(sW[i].iHasTrayIcon == 1)
			{
				sBKSel[j].trayicon = sW[i].trayicon;
				sBKSel[j].iCommandId = sW[i].iCommandId;
				wcscpy(sBKSel[j].cIconTip,sW[i].cIconTip);
			}
			sBKSel[j].Next = NULL;
			j ++;
			if(j > 1)
			{
				sBKSel[j-2].Next = sBKSel + (j-1);
			}
		}
		else if(wcscmp(sW[i].cProcessName,L"explorer.exe") == 0 && 
			wcscmp(sW[i].cWndTitle,cTitle) == 0)
		{
			wcscpy(sBKSel[j].cWndTitle,sW[i].cWndTitle);
			wcscpy(sBKSel[j].cProcessName,sW[i].cProcessName);
			sBKSel[j].hWnd = sW[i].hWnd;
			sBKSel[j].lPID = sW[i].lPID;
			sBKSel[j].iWindowType = sW[i].iWindowType;
			sW[i].bToBk = 1;
			sBKSel[j].Next = NULL;
			j ++;
			break;
		}
		else if(wcscmp(sW[i].cProcessName,L"iexplore.exe") == 0 && 
			(hWnd?(sW[i].hWnd == hWnd):(wcscmp(sW[i].cWndTitle,cTitle) == 0)))
		{
			wcscpy(sBKSel[j].cWndTitle,sW[i].cWndTitle);
			wcscpy(sBKSel[j].cProcessName,sW[i].cProcessName);
			sBKSel[j].hWnd = sW[i].hWnd;
			sBKSel[j].lPID = sW[i].lPID;
			sBKSel[j].iWindowType = sW[i].iWindowType;
			sW[i].bToBk = 1;
			sBKSel[j].Next = NULL;
			j ++;
			break;
		}
	}

	return 1;
}


int SetBkWndHotkey(int iSel)
{
	int i = 0;
	wchar_t cPID[20];
	
	INT_PTR iRet = 0;
	sBKWHotkey * pBH = NULL, * pBHTemp = NULL;
	pBH = new sBKWHotkey;
	pBH->cTitle = new wchar_t[2048];
	pBH->hWnd = NULL;
	memset(pBH->cTitle,0,2048 * sizeof(wchar_t));
	memset(pBH->cPID,0,20 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSel,0,pBH->cTitle,2040);
	ListView_GetItemText(hListView,iSel,1,pBH->cPID,20);
	for(i = 0;i < iBaSW;i ++)
	{
		memset(cPID,0,20 * sizeof(wchar_t));
		swprintf(cPID,L"%X",sW[i].lPID);
		if(wcscmp(cPID,pBH->cPID) == 0 && wcscmp(sW[i].cProcessName,L"iexplore.exe") != 0
			&& wcscmp(sW[i].cProcessName,L"explorer.exe") != 0)
		{
			memset(pBH->cProcessName,0,30 * sizeof(wchar_t));
			wcscpy(pBH->cProcessName,sW[i].cProcessName);
			break;
		}
		else if(wcscmp(cPID,pBH->cPID) == 0 && (wcscmp(sW[i].cProcessName,L"iexplore.exe") == 0
			|| wcscmp(sW[i].cProcessName,L"explorer.exe") == 0) && 
			wcscmp(pBH->cTitle,sW[i].cWndTitle) == 0)
		{
			memset(pBH->cProcessName,0,30 * sizeof(wchar_t));
			wcscpy(pBH->cProcessName,sW[i].cProcessName);
			pBH->hWnd = sW[i].hWnd;
			break;
		}
	}
	pBH->aHK = 0;
	pBH->uMF = 0;
	pBH->uVK = 0;
	pBH->wHK = 0;
	iRet = DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_BKWNDHOTKEY),
		hMainDialog,BkWndHotkeyProc,(LPARAM)pBH);
	if(pBH != NULL)
	{
		delete [] pBH->cTitle;
		delete pBH;
		pBH = NULL;
	}
	
	return 1;
}


INT_PTR CALLBACK BkWndHotkeyProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	BOOL bSuc = 0;
	ATOM aBKWnd = 0;
	wchar_t * cAtom = NULL;//[2048];
	sBKWHotkey * pBHParam = NULL, * pBHTemp = NULL, * pBHNext = NULL, * pBHBack = NULL;
	static sBKWHotkey * pCP = NULL;
	sBKWHotkey * pBH = NULL;
	WORD wHK = 0;
	BKHList ::iterator cNode;
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hBKDialog = hwndDlg;
		hHK_BkWnd = GetDlgItem(hwndDlg,IDC_HOTKEY_BKWINDOW);
		pBHParam = (sBKWHotkey * )lParam;
		pCP = pBHParam;
		for(cNode = g_BKHList.begin();cNode != g_BKHList.end(); cNode ++)
		{
			sBKWHotkey * pC =  *cNode;
			if(wcscmp(pC->cPID,pBHParam->cPID) == 0 && wcscmp(pBHParam->cProcessName,L"iexplore.exe") != 0 
					&& wcscmp(pBHParam->cProcessName,L"explorer.exe") != 0)
			{
				SendDlgItemMessage(hwndDlg,IDC_HOTKEY_BKWINDOW,HKM_SETHOTKEY,
					pC->wHK,0);
				break;
			}
			else if(wcscmp(pC->cPID,pBHParam->cPID) == 0 && (wcscmp(pBHParam->cProcessName,L"iexplore.exe") == 0 
				|| wcscmp(pBHParam->cProcessName,L"explorer.exe") == 0) && wcscmp(pC->cTitle,pBHParam->cTitle) == 0)
			{
				SendDlgItemMessage(hwndDlg,IDC_HOTKEY_BKWINDOW,HKM_SETHOTKEY,
					pC->wHK,0);
				break;
			}
		}
		
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOG_BKWNDHOTKEY);
		}

		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_OK:
			wHK = (WORD)SendMessage(hHK_BkWnd,HKM_GETHOTKEY,0,0);
			if(wHK == 0)
			{
				ShowWindow(GetDlgItem(hwndDlg,IDC_STATIC_BKNOTEMPTY),SW_SHOW);
				SetFocus(GetDlgItem(hwndDlg,IDC_HOTKEY_BKWINDOW));
				return FALSE;
			}

			cAtom = new wchar_t[2048];

			for(cNode = g_BKHList.begin();cNode != g_BKHList.end(); cNode ++)
			{
				sBKWHotkey * pC =  *cNode;
				if(wcscmp(pC->cPID,pCP->cPID) == 0 && wcscmp(pC->cProcessName,L"iexplore.exe") != 0 
					&& wcscmp(pC->cProcessName,L"explorer.exe") != 0)
				{
					memset(cAtom,0,2048 * sizeof(wchar_t));
					wcscpy(cAtom,L"REGHOTKEY");
					wcscat(cAtom,pC->cPID);
					wcscat(cAtom,pC->cTitle);
					pC->aHK = GlobalFindAtom(cAtom);
					UnregisterHotKey(hMainDialog,pC->aHK);

					g_BKHList.erase(cNode);
					delete [] pC->cTitle;
					delete pC;
					break;
				}
				else if(wcscmp(pC->cPID,pCP->cPID) == 0 && (wcscmp(pC->cProcessName,L"iexplore.exe") == 0 
					|| wcscmp(pC->cProcessName,L"explorer.exe") == 0) && wcscmp(pC->cTitle,pCP->cTitle) == 0)
				{
					memset(cAtom,0,2048 * sizeof(wchar_t));
					wcscpy(cAtom,L"REGHOTKEY");
					wcscat(cAtom,pC->cPID);
					wcscat(cAtom,pC->cTitle);
					pC->aHK = GlobalFindAtom(cAtom);
					UnregisterHotKey(hMainDialog,pC->aHK);

					g_BKHList.erase(cNode);
					delete [] pC->cTitle;
					delete pC;
					break;
				}
			}

			pBH = new sBKWHotkey;
			pBH->cTitle = new wchar_t[2048];
			memset(pBH->cTitle,0,2048 * sizeof(wchar_t));
			memset(pBH->cPID,0,20 * sizeof(wchar_t));
			pBH->aHK = 0;
			pBH->uMF = 0;	
			pBH->uVK = 0;
			pBH->wHK = 0;
			pBH->Next = NULL;

			wcscpy(pBH->cTitle,pCP->cTitle);
			wcscpy(pBH->cPID,pCP->cPID);
			wcscpy(pBH->cProcessName,pCP->cProcessName);
			pBH->hWnd = pCP->hWnd;
			memset(cAtom,0,2048 * sizeof(wchar_t));
			wcscpy(cAtom,L"REGHOTKEY");
			wcscat(cAtom,pBH->cPID);
			wcscat(cAtom,pBH->cTitle);
			pBH->aHK = GlobalFindAtom(cAtom);
			UnregisterHotKey(hMainDialog,pBH->aHK);

			pBH->wHK = wHK;
			pBH->aHK = GlobalAddAtom((LPCTSTR)cAtom);
			pBH->uMF = HIBYTE(pBH->wHK);
			pBH->uVK = LOBYTE(pBH->wHK);
			uModifier_BK = 0;
			if(pBH->uMF & HOTKEYF_ALT)
			{
				uModifier_BK |= MOD_ALT;
			}
			if(pBH->uMF & HOTKEYF_CONTROL)
			{
				uModifier_BK |= MOD_CONTROL;
			}
			if(pBH->uMF & HOTKEYF_SHIFT)
			{
				uModifier_BK |= MOD_SHIFT;
			}
			bSuc = RegisterHotKey(hMainDialog,pBH->aHK,uModifier_BK,pBH->uVK);
			pBH->uMF = uModifier_BK;
			delete [] cAtom;
			if (bSuc == 0)
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_REGBKWNDFAILURE",
					L"Register Custom Backgrounding Hotkey Failure,Please Change Retry.",hwndDlg);
				delete [] pBH->cTitle;
				delete pBH;

				return FALSE;
			}
			else
			{
				{
					DBClass db_BK;
					db_BK.InsertBKHKItem(pBH);
				}
				g_BKHList.push_back(pBH);
				EndDialog(hwndDlg,2);
			}
			return FALSE;
		case IDC_CANCEL:
			EndDialog(hwndDlg,1);
			return FALSE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}



int UnRegisterHK(int iSelMark,WndAndHandle * pW /* = NULL*/)
{
	BKHList ::iterator cNode;
	ATOM aHK = 0;
	wchar_t * cAtom = new wchar_t[2048];
	wchar_t * cTitle = (wchar_t*)VirtualAlloc(NULL,2048 * sizeof(wchar_t),
		MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(cTitle,0,2048 * sizeof(wchar_t));
	wchar_t cPID[20];
	memset(cPID,0,20);
	ListView_GetItemText(hListView,iSelMark,0,cTitle,2040);
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);

	if(pW != NULL)
	{
		wchar_t cSWPID[20];
		swprintf(cSWPID,L"%X",pW->lPID);
		wcscpy(cPID,cSWPID);
		wcscpy(cTitle,pW->cWndTitle);
	}
	for(cNode = g_BKHList.begin();cNode != g_BKHList.end(); cNode ++)
	{
		sBKWHotkey * pC =  *cNode;
		if(wcscmp(pC->cPID,cPID) == 0 && 
			 wcscmp(pC->cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(pC->cProcessName,L"explorer.exe") != 0)
		{
			g_BKHList.erase(cNode);
			memset(cAtom,0,2048 * sizeof(wchar_t));
			wcscpy(cAtom,L"REGHOTKEY");
			wcscat(cAtom,cPID);
			wcscat(cAtom,pC->cTitle);
			aHK = GlobalFindAtom(cAtom);
			UnregisterHotKey(hMainDialog,aHK);

			DBClass db_hk;
			db_hk.DelBKHKItem(pC);
			delete [] pC->cTitle;
			delete pC;
			break;
		}
		else if(wcscmp(pC->cPID,cPID) == 0 && 
			wcscmp(pC->cTitle,cTitle) == 0 && (wcscmp(pC->cProcessName,L"iexplore.exe") == 0 || 
			wcscmp(pC->cProcessName,L"explorer.exe") == 0))
		{
			g_BKHList.erase(cNode);
			memset(cAtom,0,50 * sizeof(wchar_t));
			wcscpy(cAtom,L"REGHOTKEY");
			wcscat(cAtom,cPID);
			wcscpy(cAtom,pC->cTitle);
			aHK = GlobalFindAtom(cAtom);
			UnregisterHotKey(hMainDialog,aHK);
			DBClass db_hk;
			db_hk.DelBKHKItem(pC);

			delete [] pC->cTitle;
			delete pC;
			break;
		}
	}

	delete [] cAtom;
	VirtualFree(cTitle,0,MEM_RELEASE);
	
	return 1;
}



int BKRegistered(sBKWHotkey * pWH)//wchar_t * cTitle, wchar_t * cPID)
{
	LV_ITEM bkItem;
	int i = 0,iNotFound = 0;
	WndAndHandle * pWAH = NULL;
	int iCount = ListView_GetItemCount(hList_BK);

	if(iCount > 0)
	{
		for(i = 0; i < iCount ;i ++)
		{
			bkItem.iItem = i;
			bkItem.mask = LVIF_PARAM;
			ListView_GetItem(hList_BK,&bkItem);
			pWAH = (WndAndHandle*)bkItem.lParam;
			ListView_GetItemText(hList_BK,i,2,cPID_BK,20);
			if(wcscmp(pWH->cPID,cPID_BK) == 0  && wcscmp(pWAH->cProcessName,L"iexplore.exe") != 0 &&
				wcscmp(pWAH->cProcessName,L"explorer.exe") != 0)
			{
				/*bkItem.iItem = i;
				bkItem.mask = LVIF_PARAM;
				ListView_GetItem(hList_BK,&bkItem);
				ShowSelected((WndAndHandle*)bkItem.lParam,1);*/
				ShowSelected(pWAH,1);
				ListView_DeleteItem(hList_BK,i);
				SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
				iNotFound = 0;
				break;
			}
			else if(wcscmp(pWH->cPID,cPID_BK) == 0  && (wcscmp(pWAH->cProcessName,L"iexplore.exe") == 0 ||
				wcscmp(pWAH->cProcessName,L"explorer.exe") == 0) &&
				/*wcscmp(pWAH->cWndTitle,pWH->cTitle) == 0*/pWAH->hWnd == pWH->hWnd)
			{
				ShowSelected(pWAH,1);
				ListView_DeleteItem(hList_BK,i);
				SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
				iNotFound = 0;
				break;				
			}
		}
		if(i >= iCount)
		{
			iNotFound = 1;
		}
		else
		{
			iNotFound = 0;
		}
	}
	else
	{
		iNotFound = 1;
	}

	if(iNotFound == 1)
	{
		ConstructBKSW(pWH->cPID,pWH->cTitle,pWH->hWnd);
		HideSelected(&sBKSel,1,0);
		//ExtendWindow();
		SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
	}

	
	return 1;
}

int ChangeWindowIcon(int iSelMark,wchar_t * cPath/* == NULL*/)
{
	ITList::iterator iNode;
	TWList::iterator tNode;

	
	DWORD dwErr = 0;
	HICON hNewIcon = NULL;
	OPENFILENAME OFN;
	HWND hDesWnd = NULL;
	int i = 0;
	wchar_t cPID[20],cCurrentPID[20],* cIconPath = NULL;
	wchar_t * pFileType = NULL, * pDialogTitle = NULL, * pT1 = NULL,
		* pT2 = NULL, * pT3 = NULL, * pT4 = NULL;
	memset(cPID,0,20 * sizeof(wchar_t));
	memset(cCurrentPID,0,20 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	for(i = 0;i < iBaSW;i ++)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
 		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0 && wcscmp(sW[i].cProcessName,L"explorer.exe") == 0)
		{
			if(cPath == NULL)
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_EXPLCANNOT",L"Explorer.exe Can NOT Change Icon.");
			}
			return 0;
		}
	}

	wchar_t * cWndTitle = new wchar_t[2048];
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2040);

	
	
	wchar_t * szFile = NULL;
	szFile = new wchar_t [1024];
	memset(szFile,0,1024 * sizeof(wchar_t));
	memset(&OFN,0,sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrDefExt = L".ICO";
	wchar_t * cCurDir_Temp = new wchar_t[1024];
	memset(cCurDir_Temp,0,1024*sizeof(wchar_t));
	GetCurrentDirectory_HD(1024,cCurDir_Temp);
	wcscat(cCurDir_Temp,L"\\Icon");
	OFN.lpstrInitialDir = cCurDir_Temp;
	
	OFN.hwndOwner = hMainDialog;
	OFN.hInstance = NULL;
	if(g_iUseLanguage == 1)
	{
		pFileType = new wchar_t[1024];
		pT1 = new wchar_t[1024];
		pT2 = new wchar_t[1024];
		pT3 = new wchar_t[1024];
		pT4 = new wchar_t[1024];

		memset(pFileType,0,1024 * sizeof(wchar_t));

		memset(pT1,0,1024 * sizeof(wchar_t));
		memset(pT2,0,1024 * sizeof(wchar_t));
		memset(pT3,0,1024 * sizeof(wchar_t));
		memset(pT4,0,1024 * sizeof(wchar_t));
		wcscpy(pT1,LoadItemStringW(L"IDS_CHANGEICON_FILETYPE1",pFileType,L"Icon Files (*.ICO)"));
		wcscpy(pT2 , LoadItemStringW(L"IDS_CHANGEICON_FILETYPE2",pFileType,L"*.ICO"));
		wcscpy(pT3 , LoadItemStringW(L"IDS_CHANGEICON_FILETYPE3",pFileType,L"AllFile (*.*)"));
		wcscpy(pT4 , LoadItemStringW(L"IDS_CHANGEICON_FILETYPE4",pFileType,L"*.*"));

		wchar_t * pHead = pFileType;
		memset(pFileType,0,1024 * sizeof(wchar_t));
		int p1len = wcslen(pT1);
		memcpy(pFileType ,pT1,p1len * 2);
		pFileType += wcslen(pT1) + 1;
		int p2len = wcslen(pT2);
		memcpy(pFileType,pT2,p2len * 2);
		pFileType += wcslen(pT2) +1 ;
		int p3len = wcslen(pT3);
		memcpy(pFileType,pT3,p3len * 2);
		pFileType += wcslen(pT3) + 1;
		int p4len = wcslen(pT4);
		memcpy(pFileType,pT4,p4len * 2);
		pFileType = pHead;


		OFN.lpstrFilter = pFileType;
	}
	else
	{
		OFN.lpstrFilter = L"Icon Files (*.ICO)\0*.ICO\0All Files (*.*)\0*.*\0\0";
	}
	OFN.lpstrCustomFilter = NULL;
	OFN.nMaxCustFilter = 0;
	OFN.nFilterIndex = 1;
	memset(szFile,0,1024 * sizeof(wchar_t));
	OFN.lpstrFile = szFile;
	OFN.nMaxFile = 1000;
	if(g_iUseLanguage == 1)
	{
		pDialogTitle = new wchar_t[1024];
		memset(pDialogTitle,0,1024 * sizeof(wchar_t));
		OFN.lpstrTitle = LoadItemStringW(L"IDS_CHANGEICON_PLEASESELICON",pDialogTitle,L"Please Selecte Icon To Changed");
	}
	else
	{
		OFN.lpstrTitle = L"Please Selecte Icon To Changed";
	}

	OFN.Flags = OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;

	iShowFileDialog = 1;
	





	BOOL bRet = TRUE;
	if(cPath == NULL)
	{
		bRet = GetOpenFileName(&OFN);
	}

	if(bRet)
	{
		if(sOP.bAutoChangeIconTitle == 1)
		{
			cIconPath = new wchar_t[2048];
			memset(cIconPath,0,2048 * sizeof(wchar_t));
			wcscpy(cIconPath,OFN.lpstrFile);
		}
		iShowFileDialog = 0;

		if(cPath == NULL)
		{
			hNewIcon = (HICON)LoadImage(NULL,OFN.lpstrFile,
				IMAGE_ICON,16,16,LR_LOADFROMFILE);
			dwErr = GetLastError();
		}
		else
		{
			hNewIcon = (HICON)LoadImage(NULL,cPath,
				IMAGE_ICON,16,16,LR_LOADFROMFILE);
			dwErr = GetLastError();
		}
		delete [] szFile;
		delete [] cCurDir_Temp;


		szFile = NULL;
		if(g_iUseLanguage && pFileType != NULL)
		{
			delete [] pFileType;
			delete [] pT1;
			delete [] pT2;
			delete [] pT3;
			delete [] pT4;
			pFileType = pT1 = pT2 = pT3 = pT4 = NULL;
		}
		if(g_iUseLanguage && pDialogTitle)
		{
			delete [] pDialogTitle;
			pDialogTitle = NULL;
		}
	}
	else
	{
		iShowFileDialog = 0;
		delete [] szFile;
		if(g_iUseLanguage && pFileType)
		{
			delete [] pFileType;
			delete [] pT1;
			delete [] pT2;
			delete [] pT3;
			delete [] pT4;
		}
		if(g_iUseLanguage && pDialogTitle)
		{
			delete [] pDialogTitle;
		}
		delete [] cWndTitle;
		delete [] cCurDir_Temp;
		return 0;
	}

	wchar_t * cCurDir = new wchar_t[1024];
	ZeroMemory(cCurDir,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cCurDir,1024);
	PathRemoveFileSpec(cCurDir);
	SetCurrentDirectory(cCurDir);
	delete [] cCurDir;

	sIconTitle * pIC = new sIconTitle;
	memset(pIC->cPID,0,20 * sizeof(wchar_t));
	memset(pIC->cProcessName,0,30 * sizeof(wchar_t));
	pIC->iWndCount = 0;
	int iHaveExist = 0;

///////////////////////////////////////////////////////////////////////////////

	sIconTitle * pTemp3 = NULL;



	for(iNode = g_ICList.begin(); iNode != g_ICList.end(); iNode ++)
	{
		sIconTitle * pI = *iNode;
		if(wcscmp(pI->cPID,cPID) == 0 && wcscmp(pI->cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(pI->cProcessName,L"explorer.exe") != 0)
		{
			for(tNode = pI->twlist.begin(); tNode != pI->twlist.end(); tNode++)
			{
				sTitleWnd * pT = *tNode;
				DestroyIcon(pT->hNewIcon);
				SendMessage(pT->hWnd,WM_SETICON,ICON_BIG,(LPARAM)hNewIcon);
				SendMessage(pT->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hNewIcon);
				pT->hNewIcon = hNewIcon;
			}
			pTemp3 = pI;
			iHaveExist = 1;
			break;
		}
		else if(wcscmp(pI->cPID,cPID) == 0 && (wcscmp(pI->cProcessName,L"iexplore.exe") == 0 ||
			wcscmp(pI->cProcessName,L"explorer.exe") == 0))
		{
			for(tNode = pI->twlist.begin(); tNode != pI->twlist.end(); tNode++)
			{
				sTitleWnd * pT = *tNode;
				if(wcscmp(pT->pOrgTitle,cWndTitle) == 0)
				{
					DestroyIcon(pT->hNewIcon);
					SendMessage(pT->hWnd,WM_SETICON,ICON_BIG,(LPARAM)hNewIcon);
					SendMessage(pT->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hNewIcon);
					pT->hNewIcon = hNewIcon;
					iHaveExist = 1;
					break;
				}
			}
			pTemp3 = pI;
			if(iHaveExist == 1)
			{
				break;
			}
		}
	}
	if(sOP.bAutoChangeIconTitle == 1 && pTemp3 != NULL && cPath == NULL)
	{

		IconTitleItem * pItem = new IconTitleItem;
		
		pItem->cIconPath = new wchar_t[1024 * 3];
		pItem->cTitle = new wchar_t[1024 * 3];
		memset(pItem->cProcessName,0,50 * sizeof(wchar_t));
		memset(pItem->cIconPath,0,1024 * 3 * sizeof(wchar_t));
		memset(pItem->cTitle,0,1024 * 3 * sizeof(wchar_t));

		wcscpy(pItem->cProcessName,pTemp3->cProcessName);
		wcscpy(pItem->cIconPath,cIconPath);

		SaveChangedIconInfo(pItem);

		delete [] pItem->cIconPath;
		delete [] pItem->cTitle;
		delete pItem;
		pItem = NULL;
		delete [] cIconPath;


		cIconPath = NULL;
	}

	if(iHaveExist == 1)
	{
		delete [] cWndTitle;
		delete pIC;
		return 2;
	}

/////////////////////////////////////////////////////////////////////////////////////



	for(i = 0;i < iBaSW;i ++)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
 		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0 && sW[i].iWindowType == 0 && 
			wcscmp(sW[i].cProcessName,L"iexplore.exe") != 0	&& 
			wcscmp(sW[i].cProcessName,L"explorer.exe") != 0)
		{
			hDesWnd = sW[i].hWnd;
			sTitleWnd * pIW = new sTitleWnd;
			memset(pIW,0,sizeof(sTitleWnd));
			pIW->pOrgTitle = new wchar_t[2048];
			memset(pIW->pOrgTitle,0,2048 * sizeof(wchar_t));
			wcscpy(pIW->pOrgTitle,cWndTitle);
			pIW->iWndType = sW[i].iWindowType;
			pIW->hWnd = sW[i].hWnd;
			pIW->hOrgIcon = (HICON)GetClassLongPtr(hDesWnd,GCLP_HICONSM);
			pIW->hNewIcon = hNewIcon;
			pIW->hOrgBigIcon = (HICON)GetClassLongPtr(hDesWnd,GCLP_HICON);
			pIW->hNewBigIcon = hNewIcon;
			
			pIC->twlist.push_back(pIW);
			SendMessage(pIW->hWnd,WM_SETICON,ICON_BIG,(LPARAM)hNewIcon);
			SendMessage(pIW->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hNewIcon);
			pIC->iWndCount ++;
			wcscpy(pIC->cPID,cPID);
			wcscpy(pIC->cProcessName,sW[i].cProcessName);
		}
		else if(wcscmp(cCurrentPID,cPID) == 0 && sW[i].iWindowType == 0 && 
			(wcscmp(sW[i].cProcessName,L"iexplore.exe") == 0	|| 
			wcscmp(sW[i].cProcessName,L"explorer.exe") == 0))
		{
			if(wcscmp(sW[i].cWndTitle,cWndTitle) == 0)
			{
				hDesWnd = sW[i].hWnd;
				sTitleWnd * pIW = new sTitleWnd;
				memset(pIW,0,sizeof(sTitleWnd));
				pIW->pOrgTitle = new wchar_t[2048];
				memset(pIW->pOrgTitle,0,2048 * sizeof(wchar_t));
				wcscpy(pIW->pOrgTitle,cWndTitle);
				pIW->iWndType = sW[i].iWindowType;
				pIW->hWnd = sW[i].hWnd;
				pIW->hOrgIcon = (HICON)GetClassLongPtr(hDesWnd,GCLP_HICONSM);
				pIW->hNewIcon = hNewIcon;
				pIW->hOrgBigIcon = (HICON)GetClassLongPtr(hDesWnd,GCLP_HICON);
				pIW->hNewBigIcon = hNewIcon;
				
				pIC->twlist.push_back(pIW);
				SendMessage(pIW->hWnd,WM_SETICON,ICON_BIG,(LPARAM)hNewIcon);
				SendMessage(pIW->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hNewIcon);
				pIC->iWndCount ++;
				wcscpy(pIC->cPID,cPID);
				wcscpy(pIC->cProcessName,sW[i].cProcessName);
				break;
			}
		}
	}
	g_ICList.push_back(pIC);
	delete [] cWndTitle;
	if(g_uCheckTI == NULL)
	{
		g_uCheckTI = SetTimer(hMainDialog,IDT_CHECKTI,1*1000
			,(TIMERPROC)CheckTitleIconTimer);
	}
	if(sOP.bAutoChangeIconTitle == 1 && cPath == NULL)
	{

		IconTitleItem * pItem = new IconTitleItem;
		
		pItem->cIconPath = new wchar_t[1024 * 3];
		pItem->cTitle = new wchar_t[1024 * 3];
		memset(pItem->cProcessName,0,50 * sizeof(wchar_t));
		memset(pItem->cIconPath,0,1024 * 3 * sizeof(wchar_t));
		memset(pItem->cTitle,0,1024 * 3 * sizeof(wchar_t));

		wcscpy(pItem->cProcessName,pIC->cProcessName);
		wcscpy(pItem->cIconPath,cIconPath);

		SaveChangedIconInfo(pItem);

		delete [] pItem->cIconPath;
		delete [] pItem->cTitle;
		delete pItem;
		delete [] cIconPath;


		cIconPath = NULL;
	}

	if(cIconPath != NULL)
	{
		delete [] cIconPath;
	}
	return 1;
}


int ChangeWndTitle(int iSelMark,wchar_t * cTitle /* == NULL*/)
{
	sTitleWnd * pTW = NULL;
	int iRet = 0,i = 0;
	wchar_t cCurrentPID[20];
	wchar_t * cWndTitle = new wchar_t[2048];
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	memset(cCurrentPID,0,20 * sizeof(wchar_t));
	sIconTitle * pIT = new sIconTitle,* pITCurrent = NULL;
	memset(pIT->cPID,0,20 * sizeof(wchar_t));
	memset(pIT->cProcessName,0,30 * sizeof(wchar_t));
	pIT->iWndCount = 0;
	ListView_GetItemText(hListView,iSelMark,1,pIT->cPID,20);
	ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2048);
	
	for(i = 0;i < iBaSW;i ++)
	{
		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(pIT->cPID,cCurrentPID) == 0 && 
			wcscmp(sW[i].cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(sW[i].cProcessName,L"explorer.exe") != 0 )
		{
			if(sW[i].iWindowType == 0)
			{
				pTW = NULL;
				pTW = new sTitleWnd;
				memset(pTW,0,sizeof(sTitleWnd));
				pTW->pOrgTitle = new wchar_t[2048];
				memset(pTW->pOrgTitle,0,2048);
				pTW->pNewTitle = new wchar_t[2048];
				memset(pTW->pNewTitle,0,2048);
				pTW->iWndType = sW[i].iWindowType;
				pTW->hWnd = sW[i].hWnd;
				GetWindowText(pTW->hWnd,pTW->pOrgTitle,2048);
				pIT->twlist.push_back(pTW);
				pIT->iWndCount ++;
			}
			
			wcscpy(pIT->cProcessName,sW[i].cProcessName);
			wcscpy(pIT->cPID,cCurrentPID);
		}
		else if(wcscmp(pIT->cPID,cCurrentPID) == 0 && 
			(wcscmp(sW[i].cProcessName,L"iexplore.exe") == 0 ||
			wcscmp(sW[i].cProcessName,L"explorer.exe") == 0)  &&
			wcscmp(sW[i].cWndTitle,cWndTitle) == 0)
		{
			if(sW[i].iWindowType == 0)
			{
				pTW = NULL;
				pTW = new sTitleWnd;
				memset(pTW,0,sizeof(sTitleWnd));
				pTW->pOrgTitle = new wchar_t[2048];
				memset(pTW->pOrgTitle,0,2048);
				pTW->pNewTitle = new wchar_t[2048];
				memset(pTW->pNewTitle,0,2048);
				pTW->iWndType = sW[i].iWindowType;
				pTW->hWnd = sW[i].hWnd;
				GetWindowText(pTW->hWnd,pTW->pOrgTitle,2048);
				pIT->twlist.push_back(pTW);
				pIT->iWndCount ++;
				wcscpy(pIT->cProcessName,sW[i].cProcessName);
				wcscpy(pIT->cPID,cCurrentPID);
				break;
			}
		}
	}
   
	delete [] cWndTitle;


	if(cTitle == NULL)
	{
		iRet = DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_CHANGEWNDTITLE),
			hMainDialog,ChangeWndTitleProc,(LPARAM)pIT);
	}
	else
	{
		ChangeTitle(cTitle,pIT);
	}
	

	return 1;
}


int ChangeTitle(wchar_t * cTitle,sIconTitle * pIT)
{
	ITList::iterator cITNode;
	TWList::iterator cNode,cTempNode;
	sIconTitle * pITTemp = pIT;
	
	
	
	
	wchar_t * cWndTitle = new wchar_t[2048];
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	wcscpy(cWndTitle,cTitle);
	int iHaveExist = 0;
	for(cITNode = g_ITList.begin();cITNode != g_ITList.end(); cITNode ++)
	{
		sIconTitle * pIT = * cITNode;
		if(wcscmp(pIT->cPID,pITTemp->cPID) == 0 &&
			wcscmp(pIT->cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(pIT->cProcessName,L"explorer.exe") != 0 )
		{
			for(cNode = pIT->twlist.begin();cNode != pIT->twlist.end();cNode ++)
			{
				sTitleWnd * pTW = * cNode;
				SetWindowText(pTW->hWnd,cWndTitle);
				wcscpy(pTW->pNewTitle,cWndTitle);
			}

			iHaveExist = 1;
			break;
		}
		else if(wcscmp(pIT->cPID,pITTemp->cPID) == 0 &&
			(wcscmp(pIT->cProcessName,L"iexplore.exe") == 0 ||
			wcscmp(pIT->cProcessName,L"explorer.exe") == 0))
		{
			cNode = pIT->twlist.begin();
			cTempNode = pITTemp->twlist.begin();
			sTitleWnd * pTW = * cNode;
			sTitleWnd * pTWTemp = * cTempNode;
			if(pTW->hWnd == pTWTemp->hWnd)
			{
				SetWindowText(pTW->hWnd,cWndTitle);
				wcscpy(pTW->pNewTitle,cWndTitle);
				iHaveExist = 1;
			}
		}
	}
	
	if(iHaveExist == 0)
	{
		for(cNode = pITTemp->twlist.begin();cNode != pITTemp->twlist.end(); cNode ++)
		{
			sTitleWnd * pTW = * cNode;
			SetWindowText(pTW->hWnd,cWndTitle);
			wcscpy(pTW->pNewTitle,cWndTitle);
		}
		g_ITList.push_back(pITTemp);
	}
	else
	{
		for(cNode = pITTemp->twlist.begin();cNode != pITTemp->twlist.end(); /*cNode ++*/)
		{
			sTitleWnd * pTW = * cNode;
			pITTemp->twlist.erase(cNode++);
			//cNode --;
			delete [] pTW->pOrgTitle;
			delete [] pTW->pNewTitle;
			delete pTW;
		}
		delete pITTemp;
	}




	if(g_uCheckTI == NULL)
	{
		g_uCheckTI = SetTimer(hMainDialog,IDT_CHECKTI,1*1000
			,(TIMERPROC)CheckTitleIconTimer);
	}

	delete [] cWndTitle;



	return 1;
}



INT_PTR CALLBACK ChangeWndTitleProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	int i = 0,iLength = 0,iHaveExist = 0;
	wchar_t * cWndTitle = NULL, * cStockTitle = NULL;
	static sIconTitle * pITTemp = NULL;
	static HWND hCombo_Title;
	static wchar_t cProcessName[50];
	ITList::iterator cITNode;
	TWList::iterator cNode,cTempNode;


	switch(uMsg)
	{
	case WM_INITDIALOG:
		hTitleDialog = hwndDlg;
		hCombo_Title = GetDlgItem(hwndDlg,IDC_COMBO_WNDTITLE);
		pITTemp = (sIconTitle *)lParam;
		SendMessage(hCombo_Title,CB_LIMITTEXT,(WPARAM)2048,0);
		cStockTitle = new wchar_t[1024];

		for(i = 0;i < 12;i ++)
		{
			memset(cStockTitle,0,1024 * sizeof(wchar_t));
			switch(i)
			{
			case 0:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_MACROFLASH",cStockTitle,
					L"Macromedia Flash MX Professional 2004 - [未命名-1]"));
				break;
			case 1:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_MYCOMPUTER",cStockTitle,
					L"My Computer"));
				break;
			case 2:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_NOTEPAD",cStockTitle,
					L"无标题 - 记事本"));
				break;
			case 3:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_EXCEL",cStockTitle,
					L"Microsoft Excel - Book1"));
				break;
			case 4:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_WORD",cStockTitle,
					L"文档 1 - Microsoft Word"));
				break;
			case 5:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_DW",cStockTitle,
					L"Macromedia Dreamweaver MX 2004 - [无标题文档 (Untitled-1)]"));
				break;
			case 6:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_OUTLOOK",cStockTitle,
					L"收件箱 - Microsoft Outlook"));
				break;
			case 7:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_PHOTOSHOP",cStockTitle,
					L"Adobe Photoshop"));
				break;
			case 8:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_CAL",cStockTitle,
					L"计算器"));
				break;
			case 9:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_ULTRAEDIT",cStockTitle,
					L"UltraEdit-32 - [编辑1]"));
				break;
			case 10:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_ADOBEREADER",cStockTitle,
					L"Adobe Reader"));
				break;
			case 11:
				wcscpy(cStockTitle,LoadItemStringW(L"IDS_TITLE_WINDOWHELP",cStockTitle,
					L"Windows Help"));
				break;

			}
			SendMessageW(hCombo_Title,CB_ADDSTRING,0,(LPARAM)cStockTitle);
		}
		
		delete [] cStockTitle;
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOG_CHANGEWNDTITLE);
		}

		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_OK:
			iLength = GetWindowTextLength(hCombo_Title);
			if(iLength <= 0)
			{
				ShowWindow(GetDlgItem(hwndDlg,IDC_STATIC_DONOTEMPTY),SW_SHOW);
				SetFocus(hCombo_Title);
				return TRUE;
			}
			cWndTitle = new wchar_t[2048];
			memset(cWndTitle,0,2048 * sizeof(wchar_t));
			GetDlgItemText(hwndDlg,IDC_COMBO_WNDTITLE,cWndTitle,2048);

			iHaveExist = 0;
			for(cITNode = g_ITList.begin();cITNode != g_ITList.end(); cITNode ++)
			{
				sIconTitle * pIT = * cITNode;
				if(wcscmp(pIT->cPID,pITTemp->cPID) == 0 &&
					wcscmp(pIT->cProcessName,L"iexplore.exe") != 0 && 
					wcscmp(pIT->cProcessName,L"explorer.exe") != 0 )
				{
					for(cNode = pIT->twlist.begin();cNode != pIT->twlist.end();cNode ++)
					{
						sTitleWnd * pTW = * cNode;
						SetWindowText(pTW->hWnd,cWndTitle);
						wcscpy(pTW->pNewTitle,cWndTitle);

						memset(cProcessName,0,50 * sizeof(wchar_t));
						wcscpy(cProcessName,pIT->cProcessName);
					}

					iHaveExist = 1;
					break;
				}
				else if(wcscmp(pIT->cPID,pITTemp->cPID) == 0 &&
					(wcscmp(pIT->cProcessName,L"iexplore.exe") == 0 ||
					wcscmp(pIT->cProcessName,L"explorer.exe") == 0))
				{
					cNode = pIT->twlist.begin();
					cTempNode = pITTemp->twlist.begin();
					sTitleWnd * pTW = * cNode;
					sTitleWnd * pTWTemp = * cTempNode;
					if(pTW->hWnd == pTWTemp->hWnd)
					{
						SetWindowText(pTW->hWnd,cWndTitle);
						wcscpy(pTW->pNewTitle,cWndTitle);
						iHaveExist = 1;
					}
				}
			}
			
			if(iHaveExist == 0)
			{
				for(cNode = pITTemp->twlist.begin();cNode != pITTemp->twlist.end(); cNode ++)
				{
					sTitleWnd * pTW = * cNode;
					SetWindowText(pTW->hWnd,cWndTitle);
					wcscpy(pTW->pNewTitle,cWndTitle);
					wcscpy(cProcessName,pITTemp->cProcessName);
				}
				g_ITList.push_back(pITTemp);
			}
			else
			{
				for(cNode = pITTemp->twlist.begin();cNode != pITTemp->twlist.end();/* cNode ++*/)
				{
					sTitleWnd * pTW = * cNode;
					pITTemp->twlist.erase(cNode++);
					//cNode --;
					delete [] pTW->pOrgTitle;
					delete [] pTW->pNewTitle;
					delete pTW;
				}
				delete pITTemp;
			}




			if(g_uCheckTI == NULL)
			{
				g_uCheckTI = SetTimer(hMainDialog,IDT_CHECKTI,1*1000
					,(TIMERPROC)CheckTitleIconTimer);
			}

			

			if(sOP.bAutoChangeIconTitle == 1 && 
				wcscmp(wcslwr(cProcessName),L"explorer.exe"))
			{
				IconTitleItem * pItem = new IconTitleItem;
				
				pItem->cIconPath = new wchar_t[1024 * 3];
				pItem->cTitle = new wchar_t[1024 * 3];
				memset(pItem->cProcessName,0,50 * sizeof(wchar_t));
				memset(pItem->cIconPath,0,1024 * 3 * sizeof(wchar_t));
				memset(pItem->cTitle,0,1024 * 3 * sizeof(wchar_t));
	
				wcscpy(pItem->cProcessName,cProcessName);
				wcscpy(pItem->cTitle,cWndTitle);
				wcscpy(pItem->cIconPath,L"");


				SaveChangedTitleInfo(pItem);
				delete [] pItem->cIconPath;
				delete [] pItem->cTitle;
				delete pItem;
			}

			delete [] cWndTitle;


			EndDialog(hwndDlg,1);
			return FALSE;
		case ID_CANCEL:
			for(cNode = pITTemp->twlist.begin();cNode != pITTemp->twlist.end();/* cNode ++*/)
			{
				sTitleWnd * pTW = *cNode;
				pITTemp->twlist.erase(cNode ++);
				delete [] pTW->pOrgTitle;
				delete [] pTW->pNewTitle;
				delete pTW;
			}
			delete pITTemp;
			EndDialog(hwndDlg,0);
			return FALSE;
		}
	}
	return FALSE;
}





VOID CALLBACK  CheckTitleIconTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	ITList::iterator cITNode;
	TWList::iterator cNode;
	
	//Check title///////////////////////////////////////////////////////////
	int iTitleCount = g_ITList.size();
	for(cITNode = g_ITList.begin();cITNode != g_ITList.end(); cITNode ++)
	{
		sIconTitle * pIT = * cITNode;

		for(cNode = pIT->twlist.begin();cNode != pIT->twlist.end();cNode ++)
		{
			sTitleWnd * pTW = * cNode;
			SetWindowText(pTW->hWnd,pTW->pNewTitle);
		}
	}
	ITList::iterator iNode;
	TWList::iterator tNode;
	
	//Check Icon///////////////////////////////////////////////////////////
	for(iNode = g_ICList.begin(); iNode != g_ICList.end(); iNode ++)
	{
		sIconTitle * pI = *iNode;
		for(tNode = pI->twlist.begin(); tNode != pI->twlist.end(); tNode++)
		{
			sTitleWnd * pT = *tNode;

			SendMessage(pT->hWnd,WM_SETICON,ICON_BIG,(LPARAM)pT->hNewIcon);
			SendMessage(pT->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)pT->hNewIcon);
		}
	}




}








int RestoreTitle(int iSelMark,int NotDelDB /* == 0 */)
{
	int i = 0;
	sIconTitle * pITTemp = NULL,* pITPrev = NULL;
	TWList::iterator cNode;
	ITList::iterator tNode;

	wchar_t cPID[20];
	wchar_t * cWndTitle = new wchar_t[2048];
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	memset(cPID,0,20 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2048);

	for(tNode = g_ITList.begin();tNode != g_ITList.end(); /*tNode ++*/)
	{
		sIconTitle * pITDel = * tNode;
		if(wcscmp(pITDel->cPID,cPID) == 0 && wcscmp(pITDel->cProcessName,L"iexplore.exe") != 0 &&
			wcscmp(pITDel->cProcessName,L"explorer.exe") != 0)
		{
			for(cNode = pITDel->twlist.begin(); cNode != pITDel->twlist.end(); /*cNode ++*/)
			{
				sTitleWnd * pTWDel = *cNode;
				pITDel->twlist.erase(cNode ++);
				//cNode --;
				SetWindowText(pTWDel->hWnd,pTWDel->pOrgTitle);

				delete [] pTWDel->pOrgTitle;
				delete [] pTWDel->pNewTitle;
				delete pTWDel;
			}
			if(NotDelDB == 0)
			{
				DeleteIconTitleFromDB(pITDel->cProcessName,1);
			}
			delete pITDel;
			g_ITList.erase(tNode ++);
			//tNode --;
		}
		else if(wcscmp(pITDel->cPID,cPID) == 0 && 
			(wcscmp(pITDel->cProcessName,L"iexplore.exe") == 0 ||
			wcscmp(pITDel->cProcessName,L"explorer.exe") == 0))
		{
			for(cNode = pITDel->twlist.begin(); cNode != pITDel->twlist.end(); /*cNode ++*/)
			{
				sTitleWnd * pTWDel = *cNode;
				if(wcscmp(pTWDel->pNewTitle,cWndTitle) == 0)
				{
					pITDel->twlist.erase(cNode ++);
					//cNode --;
					SetWindowText(pTWDel->hWnd,pTWDel->pOrgTitle);
					if(NotDelDB == 0)
					{
						DeleteIconTitleFromDB(pITDel->cProcessName,1);
					}

					delete [] pTWDel->pOrgTitle;
					delete [] pTWDel->pNewTitle;
					delete pTWDel;
					delete pITDel;
					g_ITList.erase(tNode ++);
					break;
				}
				else
				{
					break;
				}
			}
		}
	}






	delete [] cWndTitle;
	if(g_ITList.size() == 0 && g_ICList.size() == 0)
	{
		KillTimer(hMainDialog,IDT_CHECKTI);
		g_uCheckTI = NULL;
	}
	
	return 1;
}

int RestoreIcon(int iSelMark,DWORD lPID /* = 0*/,int NotDelDB /* == 0 */)
{
	int i = 0;
	sIconTitle * pIconTemp = NULL,* pIconPrev = NULL;
	wchar_t cPID[20], * cWndTitle = NULL;
	memset(cPID,0,20 * sizeof(wchar_t));
	cWndTitle = new wchar_t[2048];
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2048);
	if(lPID != 0)
	{
		swprintf(cPID,L"%X",lPID);
	}
	
	ITList::iterator iNode;
	TWList::iterator tNode;
		

	for(iNode = g_ICList.begin();iNode != g_ICList.end(); iNode ++)
	{
		sIconTitle * pICDel = * iNode;
		if(wcscmp(pICDel->cPID,cPID) == 0 && wcscmp(pICDel->cProcessName,L"iexplore.exe") != 0
			&& wcscmp(pICDel->cProcessName,L"explorer.exe") != 0)
		{
			int iIWSize = pICDel->twlist.size();
			for(tNode = pICDel->twlist.begin(); tNode != pICDel->twlist.end(); /*tNode ++*/)
			{
				sTitleWnd * pIWDel = *tNode;
				if(lPID == 0)
				{
					SendMessage(pIWDel->hWnd,WM_SETICON,ICON_BIG,(LPARAM)pIWDel->hOrgBigIcon);
					SendMessage(pIWDel->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)pIWDel->hOrgIcon);
				}
				pICDel->twlist.erase(tNode ++);
				//tNode --;
				delete [] pIWDel->pOrgTitle;
				DestroyIcon(pIWDel->hNewIcon);
				delete pIWDel;
			}
			if(NotDelDB == 0)
			{
				DeleteIconTitleFromDB(pICDel->cProcessName);
			}
			g_ICList.erase(iNode++);
			delete pICDel;
		}
		else if(wcscmp(pICDel->cPID,cPID) == 0 && (wcscmp(pICDel->cProcessName,L"iexplore.exe") == 0
			|| wcscmp(pICDel->cProcessName,L"explorer.exe") == 0))
		{
			int iIWSize = pICDel->twlist.size();
			
			for(tNode = pICDel->twlist.begin(); tNode != pICDel->twlist.end(); /*tNode ++*/)
			{
				sTitleWnd * pIWDel = *tNode;
				if(wcscmp(pIWDel->pOrgTitle,cWndTitle) == 0)
				{
					if(lPID == 0)
					{
						SendMessage(pIWDel->hWnd,WM_SETICON,ICON_BIG,(LPARAM)pIWDel->hOrgBigIcon);
						//SetClassLongPtr(pIWDel->hWnd,GCLP_HICON,(LONG_PTR)pIWDel->hOrgBigIcon);
						//SetClassLongPtr(pIWDel->hWnd,GCLP_HICONSM,(LONG_PTR)pIWDel->hOrgIcon);
						SendMessage(pIWDel->hWnd,WM_SETICON,ICON_SMALL,(LPARAM)pIWDel->hOrgIcon);
					}

					if(NotDelDB == 0)
					{
						DeleteIconTitleFromDB(pICDel->cProcessName);
					}
					pICDel->twlist.erase(tNode ++);
					//tNode --;
					DestroyIcon(pIWDel->hNewIcon);
					delete [] pIWDel->pOrgTitle;
					delete pIWDel;
					g_ICList.erase(iNode++);
					delete pICDel;
					break;
				}
			}
		}
	}
	if(g_ITList.size() == 0 && g_ICList.size() == 0)
	{
		KillTimer(hMainDialog,IDT_CHECKTI);
		g_uCheckTI = NULL;
	}

	delete [] cWndTitle;

	return 1;
}


int Save2AutoHideList(int iSelMark)
{
	 int i = 0;
	 wchar_t cPID[20],cCurrentPID[20], * cWndTitle = new wchar_t[2048];
	 memset(cPID,0,20 * sizeof(wchar_t));
	 memset(cWndTitle,0,2048 * sizeof(wchar_t));
	 ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2048);
	 ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	 

	 for(i = 0;i < iBaSW;i ++)
	 {
		 memset(cCurrentPID,0,20 * sizeof(wchar_t));
		 swprintf(cCurrentPID,L"%X",sW[i].lPID);
		 if(wcscmp(cCurrentPID,cPID) == 0  && 
			 wcscmp(sW[i].cProcessName,L"explorer.exe") != 0 )
		 {
			DBClass db;
			AutoHideItem * pItem = new AutoHideItem;
			pItem->cProcessName = new wchar_t[1024];
			memset(pItem->cProcessName,0,1024);

			wcscpy(pItem->cProcessName,sW[i].cProcessName);
			pItem->iType = 1;//window
			int iRet = db.AddAutoHideItem(pItem);
			
			if(iRet == 1)
			{
				Insert2AutoHideList(pItem);//For Listview
				AddDBPorcessList(pItem);//For list data 
			}
			

			delete [] pItem->cProcessName;
			delete pItem;
		 }
	 }

	 delete [] cWndTitle;

	return 1;
}



int SetWindowToTop(int iSelMark)
{
	int i = 0;
	wchar_t cPID[20],cCurrentPID[20], * cWndTitle = new wchar_t[2048];
	memset(cPID,0,20 * sizeof(wchar_t));
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2048);
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	
	
	for(i = 0;i < iBaSW;i ++)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0)
		{
			SetForegroundWindow(sW[i].hWnd);
			break;			
		}
	}
	

	return 1;
}





int Insert2AutoHideList(AutoHideItem * pItem)
{
	LVITEM bkItem;
	int i = 0,iIconIndex = 0;
	


	bkItem.mask = LVIF_TEXT;
// 	bkItem.lParam = NULL;//(LPARAM)(g_QuickHideAllTray + i);
	
	bkItem.iSubItem = 0;


	int iCount = ListView_GetItemCount(g_AutoHideList);
	wchar_t cCount[10];
	memset(cCount,0,10);
	swprintf(cCount,L"%d",iCount);
	bkItem.pszText=cCount;



	



	bkItem.iItem = ListView_GetItemCount(g_AutoHideList);
	ListView_InsertItem(g_AutoHideList,&bkItem);
	
	bkItem.mask = LVIF_TEXT;
	bkItem.iSubItem = 1;

	{
		bkItem.pszText= pItem->cProcessName;
	}



	ListView_SetItem(g_AutoHideList,&bkItem);
	
//	bkItem.mask = LVIF_TEXT; //准备去掉
//	bkItem.iSubItem = 2;
//
//	wchar_t * pText = new wchar_t[1024];
//	memset(pText,0,1024*sizeof(wchar_t));
//	if(pItem->iType == 1)
//	{
//		LoadItemStringW(L"IDS_AUTOHIDETYPE_WINDOW",pText,L"Window");
//		bkItem.pszText=	pText;
//	}
//	else
//	{
//		LoadItemStringW(L"IDS_AUTOHIDETYPE_TRAY",pText,L"Tray");
//		bkItem.pszText=	pText;
//	}
//	ListView_SetItem(g_AutoHideList,&bkItem);
//	delete [] pText;
	




	return 1;
}








int ShowPropertyDialog(int iSelMark)
{
	int i = 0;
	wchar_t cPID[20],cCurrentPID[20], * cWndTitle = new wchar_t[2048];
	memset(cPID,0,20 * sizeof(wchar_t));
	memset(cWndTitle,0,2048 * sizeof(wchar_t));
	ListView_GetItemText(hListView,iSelMark,0,cWndTitle,2048);
	ListView_GetItemText(hListView,iSelMark,1,cPID,20);
	sProperty * sP = new sProperty;
	//memset(sP,0,sizeof(sProperty));
	memset(sP->cPID,0,20 * sizeof(wchar_t));
	wcscpy(sP->cPID,cPID);
	memset(sP->cProcessName,0,30 * sizeof(wchar_t));
	sP->wHK = 0;
	
	sIconTitle * pITTemp = pITHeader;
	sTitleWnd * pTW = NULL;
	sP->iWndCount = 0;
	for(i = 0;i < iBaSW;i ++)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
		swprintf(cCurrentPID,L"%X",sW[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0 && wcscmp(sW[i].cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(sW[i].cProcessName,L"explorer.exe") != 0 )
		{
			pTW = new sTitleWnd;
			memset(pTW,0,sizeof(sTitleWnd));
			wcscpy(sP->cProcessName,sW[i].cProcessName);

			{
				sFileItem * pItem = new sFileItem;
				pItem->cFileName = sW[i].cProcessName;
				pItem->lSize = wcslen(sW[i].cProcessName) * 2;
				
				{
					DBClass arDB;

					if(arDB.IsInARFL(pItem))
					{
						sP->bAutoReserve = 1;
					}
					else
					{
						sP->bAutoReserve = 0;
					}
				}
				delete pItem;
			}

			if(sW[i].iHasTrayIcon == 1 && sW[i].trayicon.hIcon != NULL)
			{
				sP->hTrayIcon = sW[i].trayicon.hIcon;
			}
			else
			{
				sP->hTrayIcon = NULL;
			}
			pTW->hWnd = sW[i].hWnd;
			pTW->pOrgTitle = new wchar_t[2048];
			memset(pTW->pOrgTitle,0,2048 * sizeof(wchar_t));
			pTW->pNewTitle = new wchar_t[2048];
			memset(pTW->pNewTitle,0,2048 * sizeof(wchar_t));
			GetWindowText(sW[i].hWnd,pTW->pNewTitle,2048);
			GetWindowText(sW[i].hWnd,pTW->pOrgTitle,2048);
			pTW->iWndType = sW[i].iWindowType;
			
//			SendMessageTimeout(sW[i].hWnd,WM_GETICON,ICON_SMALL,NULL,SMTO_NORMAL,100,
//				(PDWORD_PTR)&pTW->hNewIcon); 
			pTW->hNewIcon = (HICON)GetClassLongPtr(sW[i].hWnd,GCLP_HICONSM);
			pTW->hOrgIcon = (HICON)GetClassLongPtr(sW[i].hWnd,GCLP_HICONSM);
			if(pTW->hNewIcon == NULL)
			{
				SendMessageTimeout(sW[i].hWnd,WM_GETICON,ICON_SMALL,NULL,SMTO_NORMAL,100,
				(PDWORD_PTR)&pTW->hNewIcon); 
				SendMessageTimeout(sW[i].hWnd,WM_GETICON,ICON_SMALL,NULL,SMTO_NORMAL,100,
				(PDWORD_PTR)&pTW->hOrgIcon); 
			}
			sP->twlist.push_back(pTW);
			sP->iWndCount ++;
		}
		else if(wcscmp(cCurrentPID,cPID) == 0 && (wcscmp(sW[i].cProcessName,L"iexplore.exe") == 0 || 
			wcscmp(sW[i].cProcessName,L"explorer.exe") == 0) && wcscmp(sW[i].cWndTitle,cWndTitle) == 0)
		{
			pTW = new sTitleWnd;
			memset(pTW,0,sizeof(sTitleWnd));
			wcscpy(sP->cProcessName,sW[i].cProcessName);

			
			{
				sFileItem * pItem = new sFileItem;
				pItem->cFileName = sW[i].cProcessName;
				pItem->lSize = wcslen(sW[i].cProcessName) * 2;
				
				{
					DBClass arDB;

					if(arDB.IsInARFL(pItem))
					{
						sP->bAutoReserve = 1;
					}
					else
					{
						sP->bAutoReserve = 0;
					}
				}
				delete pItem;
			}

			
			if(sW[i].iHasTrayIcon == 1 && sW[i].trayicon.hIcon != NULL)
			{
				sP->hTrayIcon = sW[i].trayicon.hIcon;
			}
			else
			{
				sP->hTrayIcon = NULL;
			}
			pTW->hWnd = sW[i].hWnd;
			pTW->pOrgTitle = new wchar_t[2048];
			memset(pTW->pOrgTitle,0,2048 * sizeof(wchar_t));
			pTW->pNewTitle = new wchar_t[2048];
			memset(pTW->pNewTitle,0,2048 * sizeof(wchar_t));
			GetWindowText(sW[i].hWnd,pTW->pNewTitle,2048);
			GetWindowText(sW[i].hWnd,pTW->pOrgTitle,2048);
			pTW->iWndType = sW[i].iWindowType;
			pTW->hNewIcon = (HICON)GetClassLongPtr(sW[i].hWnd,GCLP_HICONSM);
			pTW->hOrgIcon = (HICON)GetClassLongPtr(sW[i].hWnd,GCLP_HICONSM);
			if(pTW->hNewIcon == NULL)
			{
				SendMessageTimeout(sW[i].hWnd,WM_GETICON,ICON_SMALL,NULL,SMTO_NORMAL,100,
				(PDWORD_PTR)&pTW->hNewIcon); 
				SendMessageTimeout(sW[i].hWnd,WM_GETICON,ICON_SMALL,NULL,SMTO_NORMAL,100,
				(PDWORD_PTR)&pTW->hOrgIcon); 
			}
			sP->twlist.push_back(pTW);
			sP->iWndCount ++;
			break;
		}
	}
	
	BKHList::iterator hNode;
	for(hNode = g_BKHList.begin();hNode != g_BKHList.end();hNode ++)
	{
		sBKWHotkey * pH = * hNode;
		if(wcscmp(pH->cPID,cPID) == 0)
		{
			sP->wHK = pH->wHK;
		}
	}

	ITList::iterator iNode;
	for(iNode = g_ICList.begin();iNode != g_ICList.end();iNode ++)
	{
		sIconTitle * pI = *iNode;
		if(wcscmp(pI->cPID,cPID) == 0)
		{
			TWList::iterator tNode;
			for(tNode = pI->twlist.begin();tNode != pI->twlist.end();tNode ++)
			{
				sTitleWnd * pT = *tNode;
				TWList::iterator spNode;
				for(spNode = sP->twlist.begin();spNode != sP->twlist.end();spNode ++)
				{
					sTitleWnd * pSPT = *spNode;
					if(pT->hWnd == pSPT->hWnd)
					{
						pSPT->hOrgIcon = pT->hOrgIcon;
						pSPT->hNewIcon = pT->hNewIcon;				
						break;
					}
				}
			}
		}
	}

	for(iNode = g_ITList.begin();iNode != g_ITList.end();iNode ++)
	{
		sIconTitle * pI = *iNode;
		if(wcscmp(pI->cPID,cPID) == 0)
		{
			TWList::iterator tNode;
			for(tNode = pI->twlist.begin();tNode != pI->twlist.end();tNode ++)
			{
				sTitleWnd * pT = *tNode;
				TWList::iterator spNode;
				for(spNode = sP->twlist.begin();spNode != sP->twlist.end();spNode ++)
				{
					sTitleWnd * pSPT = *spNode;
					if(pT->hWnd == pSPT->hWnd)
					{
						wcscpy(pSPT->pOrgTitle,pT->pOrgTitle);
						wcscpy(pSPT->pNewTitle,pT->pNewTitle);

						break;
					}
				}
			}
		}
	}

	sFileItem * pItem = new sFileItem;
	pItem->lSize = wcslen(sP->cProcessName);
	pItem->cFileName = sP->cProcessName;
	if(IsInARFL(pItem))
	{
		sP->bAutoReserve = 1;
	}
	pItem->cFileName = NULL;
	delete pItem;
	delete [] cWndTitle;
	CreatePropertyDialog(sP);
	

//	int iSize = sP->twlist.size();
//	for(i = 0;i < iSize;i ++)
//	{
//		sTitleWnd * pToDel = sP->twlist[i];
//		delete [] pToDel->pOrgTitle;
//		delete [] pToDel->pNewTitle;
//		
//	}
	



	TWList::iterator cNode;

	TWList::iterator cNode_temp;
	for(cNode = sP->twlist.begin();cNode != sP->twlist.end();/* ++cNode 在下边递增了，不用在这再递增了*/ )
	{
		sTitleWnd * pTW1 = *cNode;

		sP->twlist.erase(cNode++);


		delete [] pTW1->pOrgTitle;
		delete [] pTW1->pNewTitle;
		delete pTW1;
	}

	delete sP;

		
		
	
	return 1;
}


INT_PTR CALLBACK PropertyProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
//	wchar_t cWndNum[10],cHandle[20];
	static sProperty * pPropertyValue = NULL;
	static HWND hIcon_Tray = NULL,hIcon_Org = NULL,hIcon_New = NULL,
		hList_TitleHandle = NULL;
	int i = 0;
//	LVITEM lvItem;
	switch(uMsg)
	{
	case WM_INITDIALOG:
//		hPropertyDialog = hwndDlg;
//		pPropertyValue = (sProperty*)lParam;
//		SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PROCESSID),pPropertyValue->cPID);
//		SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PROCESSNAME),pPropertyValue->cProcessName);
//		SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_WNDNUM),_itow(pPropertyValue->iWndCount,cWndNum,10));
//		hIcon_Tray = GetDlgItem(hwndDlg,IDC_ICON_TRAY);
//		hIcon_Org = GetDlgItem(hwndDlg,IDC_ICON_ORG);
//		hIcon_New = GetDlgItem(hwndDlg,IDC_ICON_NEW);
//		hList_TitleHandle = GetDlgItem(hwndDlg,IDC_LIST_TITLEANDHANDLE);
//		FillTitleHandleColumn(hList_TitleHandle);
//		memset(&lvItem,0,sizeof(LVITEM));
//		lvItem.mask = LVIF_TEXT;
//		
//
//		
//		for(i = 0;i < pPropertyValue->iWndCount;i ++)
//		{
//			lvItem.iItem = i;
//			lvItem.iSubItem = 0;
//			lvItem.pszText = pPropertyValue->cTitle[i];
//			ListView_InsertItem(hList_TitleHandle,&lvItem);
//			memset(cHandle,0,20 * sizeof(wchar_t));
//			swprintf(cHandle,L"%8X",pPropertyValue->hWnd[i]);
//			lvItem.pszText = cHandle;
//			lvItem.iSubItem = 1;
//			ListView_SetItem(hList_TitleHandle,&lvItem);
//		}	
//		if(pPropertyValue->hTrayIcon != NULL)
//		{
//			SendMessage(hIcon_Tray,STM_SETICON,(WPARAM)pPropertyValue->hTrayIcon,NULL);
//			SendMessage(hIcon_Org,STM_SETICON,(WPARAM)pPropertyValue->hOrgIcon,NULL);
//			SendMessage(hIcon_New,STM_SETICON,(WPARAM)pPropertyValue->hNewIcon,NULL);
//		}
//		if(pPropertyValue->wHK != 0)
//		{
//			SendDlgItemMessage(hwndDlg,IDC_HOTKEY_BK,HKM_SETHOTKEY,
//				pPropertyValue->wHK,0);
//		}
//		if(pPropertyValue->bAutoReserve == 1)
//		{
//			SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_AUTORESERVE),L"是");
//		}
//		else
//		{
//			SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_AUTORESERVE),L"否");
//		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwndDlg,1);
			return FALSE;

		case IDCANCEL:
			EndDialog(hwndDlg,0);
			return FALSE;
		default:
			return FALSE;
		}
	}
	return FALSE;
}


int DelBKHotKeyNode()
{
	int i = 0;
	wchar_t cPID[20],* cAtom = new wchar_t[2048];
	memset(cAtom,0,2048 * sizeof(wchar_t));
	memset(cPID,0,20 * sizeof(wchar_t));
	ATOM aHK = 0;
		
	BKHList::iterator hNode;
	
	int iICSize = g_BKHList.size();
	for(hNode = g_BKHList.begin();hNode != g_BKHList.end(); /*hNode ++*/)
	{
		sBKWHotkey * pBHDel = * hNode;
		for(i = 0;i < iBaSW;i ++)
		{
			memset(cPID,0,20 * sizeof(wchar_t));
			swprintf(cPID,L"%X",sW[i].lPID);
			if(wcscmp(pBHDel->cPID,cPID) == 0 && wcscmp(pBHDel->cProcessName,L"iexplore.exe") != 0
				&& wcscmp(pBHDel->cProcessName,L"explorer.exe") != 0)
			{
				break;
			}
			else if(wcscmp(pBHDel->cPID,cPID) == 0 && 
				(wcscmp(pBHDel->cProcessName,L"iexplore.exe") == 0
				|| wcscmp(pBHDel->cProcessName,L"explorer.exe") == 0))
			{
				if(wcscmp(pBHDel->cTitle,sW[i].cWndTitle) == 0)
				{
					break;
				}
				else
				{
					if(IsInITList(pBHDel->cTitle) == 1)
					{
						break;
					}
				}
			}
		}
		if(i < iBaSW)
		{
			continue;
		}
		int iInBK = IsInBKList(pBHDel->cPID);
		if(iInBK == 1)
		{
			delete [] cAtom;
			return 1;
		}
		
		g_BKHList.erase(hNode ++);
		//hNode --;
		memset(cAtom,0,2048 * sizeof(wchar_t));
		wcscpy(cAtom,L"REGHOTKEY");
		wcscat(cAtom,pBHDel->cPID);
		wcscat(cAtom,pBHDel->cTitle);
		aHK = GlobalFindAtom(cAtom);
		DWORD dwErr = 0;
		BOOL bRet = UnregisterHotKey(hMainDialog,aHK);

		if(bRet == FALSE)
		{
			dwErr = GetLastError();
		}
		delete [] pBHDel->cTitle;
		delete pBHDel;
		delete [] cAtom;
		iICSize = g_BKHList.size();
		return 1;
	}
	delete [] cAtom;

	
	
	return 0;
}







int IsInRegBKHK(WndAndHandle * pItem)
{
	wchar_t cPID[20];
	memset(cPID,0,20 * sizeof(wchar_t));

		
	BKHList::iterator hNode;
	

	for(hNode = g_BKHList.begin();hNode != g_BKHList.end(); hNode ++)
	{
		sBKWHotkey * pBHDel = * hNode;
		if(wcscmp(pBHDel->cProcessName,pItem->cProcessName) == 0)
		{
			return 1;
		}
	}
	
	return 0;
}





int IsInBKList(wchar_t * cPID)
{
	int i = 0;
	wchar_t cCurrentPID[20];
	LVITEM bkItem;
	memset(&bkItem,0,sizeof(LVITEM));
	WndAndHandle* pItem_WH = NULL;
	int iItemCount = ListView_GetItemCount(hList_BK);
	for(i = 0;i < iItemCount;i ++)
	{
		bkItem.iItem = i;
		bkItem.mask = LVIF_PARAM;
		ListView_GetItem(hList_BK,&bkItem);
		pItem_WH = (WndAndHandle * )bkItem.lParam;
		memset(cCurrentPID,0,20*sizeof(wchar_t));
		swprintf(cCurrentPID,L"%X",pItem_WH->lPID);
		if(wcscmp(cPID,cCurrentPID) == 0)
		{
			return 1;
		}
	}

	return 0;
}



int DelTitleNode()
{
	int i = 0;
	wchar_t cPID[20];
	memset(cPID,0,20 * sizeof(wchar_t));
		
	ITList::iterator iNode;
	TWList::iterator tNode;
		
	int iICSize = g_ITList.size();
	for(iNode = g_ITList.begin();iNode != g_ITList.end(); iNode ++)
	{
		sIconTitle * pITDel = * iNode;
		for(i = 0;i < iBaSW;i ++)
		{
			memset(cPID,0,20 * sizeof(wchar_t));
			swprintf(cPID,L"%X",sW[i].lPID);
			if(wcscmp(pITDel->cPID,cPID) == 0 && wcscmp(pITDel->cProcessName,L"iexplore.exe") != 0 && 
				wcscmp(pITDel->cProcessName,L"explorer.exe") != 0)
			{
				return 1;
			}
			else if(wcscmp(pITDel->cPID,cPID) == 0 && wcscmp(pITDel->cProcessName,L"iexplore.exe") == 0 ||
				wcscmp(pITDel->cProcessName,L"explorer.exe") == 0)
			{
				for(tNode = pITDel->twlist.begin(); tNode != pITDel->twlist.end(); tNode ++)
				{
					sTitleWnd * pTWDel = *tNode;
					if((wcscmp(pITDel->cProcessName,L"iexplore.exe") == 0 ||
						wcscmp(pITDel->cProcessName,L"explorer.exe") == 0))
					{
						if(wcscmp(pTWDel->pOrgTitle,sW[i].cWndTitle) == 0)
						{
							return 1;
						}
						else
						{
							if(IsInITList(pTWDel->pOrgTitle) == 1)
							{
								return 1;
							}
						}
					}
				}
			}
		}

		int iInBK = IsInBKList(pITDel->cPID);
		if(iInBK == 1)
		{
			return 1;
		}

		int iTWSize = pITDel->twlist.size();
		for(tNode = pITDel->twlist.begin(); tNode != pITDel->twlist.end(); /*tNode ++*/)
		{
			sTitleWnd * pTWDel = *tNode;
			pITDel->twlist.erase(tNode++);
			//tNode --;
			delete [] pTWDel->pOrgTitle;
			delete [] pTWDel->pNewTitle;
			delete pTWDel;
		}
		g_ITList.erase(iNode ++);
		delete pITDel;
		iICSize = g_ITList.size();
		return 1;
	}
	
	return 1;
}



int DelIconNode()
{
	int i = 0;
	sIconTitle * pIconTemp = NULL,* pIconPrev = NULL;
	wchar_t cPID[20];
	memset(cPID,0,20 * sizeof(wchar_t));
		
	ITList::iterator iNode;
	TWList::iterator tNode;
		
	int iICSize = g_ICList.size();
	for(iNode = g_ICList.begin();iNode != g_ICList.end(); iNode ++)
	{
		sIconTitle * pICDel = * iNode;
		for(i = 0;i < iBaSW;i ++)
		{
			memset(cPID,0,20 * sizeof(wchar_t));
			swprintf(cPID,L"%X",sW[i].lPID);
			if(wcscmp(pICDel->cPID,cPID) == 0 && wcscmp(pICDel->cProcessName,L"iexplore.exe") != 0 && 
				wcscmp(pICDel->cProcessName,L"explorer.exe") != 0)
			{
				return 1;
			}
			else if(wcscmp(pICDel->cPID,cPID) == 0 && (wcscmp(pICDel->cProcessName,L"iexplore.exe") == 0 || 
				wcscmp(pICDel->cProcessName,L"explorer.exe") == 0))
			{
				for(tNode = pICDel->twlist.begin(); tNode != pICDel->twlist.end(); tNode ++)
				{
					sTitleWnd * pTWDel = *tNode;
					if((wcscmp(pICDel->cProcessName,L"iexplore.exe") == 0 || 
						wcscmp(pICDel->cProcessName,L"explorer.exe") == 0))
					{
						if(wcscmp(pTWDel->pOrgTitle,sW[i].cWndTitle) == 0)
						{
							return 1;
						}
						else
						{
							if(IsInITList(pTWDel->pOrgTitle) == 1)
							{
								return 1;
							}
						}
					}
				}
			}
		}
		int iTWSize = pICDel->twlist.size();
		for(tNode = pICDel->twlist.begin(); tNode != pICDel->twlist.end();/* tNode ++*/)
		{
			sTitleWnd * pIWDel = *tNode;
			DestroyIcon(pIWDel->hNewIcon);
			pICDel->twlist.erase(tNode++);
			//tNode --;
			delete [] pIWDel->pOrgTitle;
			delete pIWDel;
		}
		g_ICList.erase(iNode ++);
		delete pICDel;
		iICSize = g_ICList.size();
		return 1;
	}

	return 0;
}

int IsInITList(wchar_t* pTitle)
{
	ITList::iterator tNode;
	TWList::iterator wNode;
	
	for(tNode = g_ITList.begin();tNode != g_ITList.end();tNode ++)
	{
		sIconTitle * pIT = * tNode;
		for(wNode = pIT->twlist.begin();wNode != pIT->twlist.end();wNode ++)
		{
			sTitleWnd * pTW = *wNode;
			if(wcscmp(pTW->pOrgTitle,pTitle) == 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

int FillTitleHandleColumn(HWND hList)
{
	wchar_t pWndTitle[100],pWndHandle[100];
	LVCOLUMN LC;
	LC.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	LC.fmt  = LVCFMT_LEFT;
	LC.cx   = 300;
	if(g_iUseLanguage == 0)
	{
		LC.pszText =L"Window Title";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pWndTitle,0,100 * sizeof(wchar_t));
		wcscpy(pWndTitle,LoadItemStringW(L"IDS_PROPERTYLIST_WNDTITLE",pHeaderText,
			L"Window Title"));
		LC.pszText = pWndTitle;
		delete [] pHeaderText;
	}
	ListView_InsertColumn(hList,0,&LC);

	LC.cx   = 100;
	if(g_iUseLanguage == 0)
	{
		LC.pszText = L"Window Handle";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pWndHandle,0,100 * sizeof(wchar_t));
		wcscpy(pWndHandle,LoadItemStringW(L"IDS_PROPERTYLIST_WNDHANDLE",pHeaderText,
			 L"Window Handle"));
		LC.pszText = pWndHandle;
		delete [] pHeaderText;
	}
	ListView_InsertColumn(hList,1,&LC);
	DWORD lvStyle = 0;
	lvStyle = ListView_GetExtendedListViewStyle(hList);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList,0,lvStyle);
	
	return 1;
}


int CreatePropertyDialog(sProperty * pProperty)
{
	PROPSHEETPAGE psp[2];
	PROPSHEETHEADER psh ;
	
	wchar_t * pGeneralTitle = new wchar_t[256], * pOtherTitle = new wchar_t[256],
		* pPropertyTitle = new wchar_t[256];
	
	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInst;
	psp[0].pszTemplate = MAKEINTRESOURCE(IDD_PROPERTY_ADDITIONAL);
	psp[0].pszIcon = NULL;
	psp[0].pfnDlgProc =(int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))AddtionalPageProc;
	if(g_iUseLanguage == 0)
	{
		psp[0].pszTitle =L"General";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pGeneralTitle,0,256 * sizeof(wchar_t));
		wcscpy(pGeneralTitle,LoadItemStringW(L"IDS_GENERAL_TITLE",pText,L"General"));
		psp[0].pszTitle = pGeneralTitle;
		delete [] pText;
	}
	psp[0].lParam = (LPARAM)pProperty;

	psp[1].dwSize = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInst;
	psp[1].pszTemplate = MAKEINTRESOURCE(IDD_PROPERTY_GENERAL);
	psp[1].pszIcon = NULL;
	psp[1].pfnDlgProc =(int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))GeneralPageProc;
	if(g_iUseLanguage == 0)
	{
		psp[1].pszTitle =L"Other";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pOtherTitle,0,256 * sizeof(wchar_t));
		wcscpy(pOtherTitle,LoadItemStringW(L"IDS_PROPERTYOTHER_TITLE",pText,L"Other"));
		psp[1].pszTitle = pOtherTitle;
		delete [] pText;
	}
	psp[1].lParam = (LPARAM)pProperty;

	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE|PSH_NOAPPLYNOW|PSH_NOCONTEXTHELP;
	psh.hwndParent = hMainDialog;
	psh.hInstance = hInst;
	psh.pszIcon = NULL;
	if(g_iUseLanguage == 0)
	{
		psh.pszCaption = L"Property";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pPropertyTitle,0,256 * sizeof(wchar_t));
		wcscpy(pPropertyTitle,LoadItemStringW(L"IDS_PROPERTY_TITLE",pText,L"Property"));
		psh.pszCaption = pPropertyTitle;
		delete [] pText;
	}



	psh.nPages = 2;
	psh.ppsp = (_PROPSHEETPAGEW *)&psp;

	int iRet = PropertySheet(&psh);

	delete [] pGeneralTitle;
	delete [] pOtherTitle;
	delete [] pPropertyTitle;
	
	return iRet;
}




HBRUSH wbr_gen = NULL;

INT_PTR CALLBACK GeneralPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	wchar_t cWndNum[10],cHandle[20];
	sProperty * pPropertyValue = NULL;
	PROPSHEETPAGE  * pPSP = NULL;
	static HWND hList_TitleHandle;
	int i = 0,iItem = 0;
	LVITEM lvItem;
	TWList::iterator spNode;
	NMLISTVIEW nmListView;
	HWND hEdit_WndNum,hEdit_OrgTitle,hEdit_NewTitle;



	wchar_t * pNew = NULL;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hPropertyDialog = hwndDlg;
		pPSP = (PROPSHEETPAGE *)lParam;
		pPropertyValue = (sProperty*)(pPSP->lParam);
		hList_TitleHandle = GetDlgItem(hwndDlg,IDC_LIST_PAGE_TITLEANDHANDLE);
		FillTitleHandleColumn(hList_TitleHandle);
		memset(&lvItem,0,sizeof(LVITEM));
		lvItem.mask = LVIF_TEXT|LVIF_PARAM;
		SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_WNDNUM),_itow(pPropertyValue->iWndCount,cWndNum,10));

		
		for(spNode = pPropertyValue->twlist.begin();spNode != pPropertyValue->twlist.end();spNode ++)
		{
			sTitleWnd * pTW = *spNode;
			lvItem.mask = LVIF_TEXT|LVIF_PARAM;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			if(pTW->pNewTitle != NULL)
			{
				lvItem.pszText = pTW->pNewTitle;
			}
			lvItem.lParam = (LPARAM)pTW;
			ListView_InsertItem(hList_TitleHandle,&lvItem);
			memset(cHandle,0,20 * sizeof(wchar_t));
			swprintf(cHandle,L"%08X",pTW->hWnd);
			lvItem.pszText = cHandle;
			lvItem.lParam = NULL;
			lvItem.iSubItem = 1;
			lvItem.mask = LVIF_TEXT;
			BOOL bRet = ListView_SetItem(hList_TitleHandle,&lvItem);
		}

		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_PROPERTY_GENERAL);
		}
		wbr_gen = CreateSolidBrush(RGB(255,255,255));
		return FALSE;
	case WM_DESTROY:
		DeleteObject(wbr_gen);
		break;
	case WM_NOTIFY:
		switch((((LPNMHDR)lParam)->code ) )
		{
		case LVN_ITEMCHANGED:
			nmListView = *((LPNMLISTVIEW)lParam);
			iItem = ((LPNMLISTVIEW)lParam) ->iItem;
			if(nmListView.uNewState & LVIS_SELECTED)
			{
				memset(&lvItem,0,sizeof(LVITEM));
				lvItem.mask = LVIF_PARAM;
				lvItem.iItem = nmListView.iItem;
				BOOL bRet = ListView_GetItem(hList_TitleHandle,&lvItem);
				SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_ORGTITLE),
					((sTitleWnd*)(lvItem.lParam))->pOrgTitle);
				SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_NEWTITLE),
					((sTitleWnd*)(lvItem.lParam))->pNewTitle);

			}
			break;
		}
		break;		
	case WM_CTLCOLORSTATIC:
		hEdit_WndNum = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_WNDNUM);
		hEdit_OrgTitle = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_ORGTITLE);
		hEdit_NewTitle = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_NEWTITLE);

		if ((HWND)lParam == hEdit_WndNum||
			(HWND)lParam == hEdit_OrgTitle||
			(HWND)lParam == hEdit_NewTitle)
		{
			return (LRESULT)wbr_gen;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			DeleteObject(wbr_gen);
			EndDialog(hwndDlg,0);
			return FALSE;
		case IDCANCEL:
			DeleteObject(wbr_gen);
			EndDialog(hwndDlg,1);
			return FALSE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}

HBRUSH wbr = NULL;

INT_PTR CALLBACK AddtionalPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	sProperty * pPropertyValue = NULL;
	PROPSHEETPAGE  * pPSP = NULL;
	HWND hIcon_Tray = NULL,hIcon_Org = NULL,hIcon_New = NULL;
	TWList::iterator iNode;
	HWND hEdit_AutoReserve,hEdit_Transparent,hEdit_ProcessID,hEdit_ProcessName;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		hPropertyDialog = hwndDlg;
		pPSP = (PROPSHEETPAGE *)lParam;
		pPropertyValue = (sProperty*)(pPSP->lParam);
		hIcon_Tray = GetDlgItem(hwndDlg,IDC_ICON_PAGE_TRAY);
		SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_PROCESSID),pPropertyValue->cPID);
		SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_PROCESSNAME),pPropertyValue->cProcessName);

		hIcon_Org = GetDlgItem(hwndDlg,IDC_ICON_PAGE_ORG);
		hIcon_New = GetDlgItem(hwndDlg,IDC_ICON_PAGE_NEW);
		for(iNode = pPropertyValue->twlist.begin();iNode != pPropertyValue->twlist.end(); iNode ++)
		{
			sTitleWnd * pTW = *iNode;
			SendMessage(hIcon_Org,STM_SETICON,(WPARAM)pTW->hOrgIcon,NULL);
			SendMessage(hIcon_New,STM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)pTW->hNewIcon);
		}

		if(pPropertyValue->hTrayIcon != NULL)
		{
			ShowWindow(GetDlgItem(hwndDlg,IDC_ICON_PAGE_TRAY),SW_SHOW);
			ShowWindow(GetDlgItem(hwndDlg,IDC_STATIC_NOTRAYICON),SW_HIDE);
			SendMessage(hIcon_Tray,STM_SETICON,(WPARAM)pPropertyValue->hTrayIcon,NULL);
		}
		else
		{
			ShowWindow(GetDlgItem(hwndDlg,IDC_ICON_PAGE_TRAY),SW_HIDE);
			ShowWindow(GetDlgItem(hwndDlg,IDC_STATIC_NOTRAYICON),SW_SHOW);
		}
		if(pPropertyValue->wHK != 0)
		{
			SendDlgItemMessage(hwndDlg,IDC_HOTKEY_PAGE_BK,HKM_SETHOTKEY,
				pPropertyValue->wHK,0);
		}
		if(pPropertyValue->bAutoReserve == 1)
		{
			if(g_iUseLanguage == 0)
			{
				SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_AUTORESERVE),L"Yes");
			}
			else
			{
				wchar_t * pYes = new wchar_t[1024];
				memset(pYes,0,1024 * sizeof(wchar_t));
				SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_AUTORESERVE),
					LoadItemStringW(L"IDS_AUTORESERVE_YES",pYes,L"Yes"));
				delete [] pYes;
			}
		}
		else
		{
			if(g_iUseLanguage == 0)
			{
				SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_AUTORESERVE),L"No");
			}
			else
			{
				wchar_t * pNo = new wchar_t[1024];
				memset(pNo,0,1024 * sizeof(wchar_t));
				SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT_PAGE_AUTORESERVE),
					LoadItemStringW(L"IDS_AUTORESERVE_NO",pNo,L"No"));
				delete [] pNo;
			}
		}


		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_PROPERTY_ADDITIONAL);
		}
		wbr = CreateSolidBrush(RGB(255,255,255));

		return FALSE;
	case WM_DESTROY:
		DeleteObject(wbr);

		break;
	case WM_CTLCOLORSTATIC:
		hEdit_AutoReserve = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_AUTORESERVE);
		hEdit_Transparent = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_TRANSPARENT);
		hEdit_ProcessID = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_PROCESSID);
		hEdit_ProcessName = GetDlgItem(hwndDlg,IDC_EDIT_PAGE_PROCESSNAME);

		if ((HWND)lParam == hEdit_ProcessID ||
			(HWND)lParam == hEdit_ProcessName||
			(HWND)lParam == hEdit_AutoReserve ||
			(HWND)lParam == hEdit_Transparent)
		{
			return (LRESULT)wbr;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			DeleteObject(wbr);
			EndDialog(hwndDlg,1);
			return FALSE;
		case IDCANCEL:
			DeleteObject(wbr);
			EndDialog(hwndDlg,0);
			return FALSE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}



int SaveWindowInfo(WndAndHandle * pSW,int iBaSW)
{
	if(iBaSW <= 0)
	{
		return 0;
	}

	wchar_t * cFileContent = (wchar_t *)VirtualAlloc(NULL,iBaSW * 2048 * 2* sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,
		PAGE_READWRITE);
	memset(cFileContent,0,iBaSW * 2048*2 * sizeof(wchar_t));
	wchar_t * cTemp = cFileContent;
	int i = 0;
	for(i = 0;i < iBaSW;i ++)
	{
		if(pSW[i].bHide == 1)
		{
			*(int *)cTemp = pSW[i].bExist;
			cTemp += sizeof(int);
			*(int *)cTemp = pSW[i].bHide;
			cTemp += sizeof(int);
			*(int *)cTemp = pSW[i].bReserved;
			cTemp += sizeof(int);
			wcscpy(cTemp,pSW[i].cIconTip);
			cTemp += 1024;
			wcscpy(cTemp,pSW[i].cProcessName);
			cTemp += 30;
			wcscpy(cTemp,pSW[i].cWndTitle);
			cTemp += 2048;
			*(HWND *)cTemp = pSW[i].hWnd;
			cTemp += sizeof(HWND);
			*(int *)cTemp = pSW[i].iHasTrayIcon;
			cTemp += sizeof(int);
			*(int *)cTemp = pSW[i].iIconIndex;
			cTemp += sizeof(int);
			*(int *)cTemp = pSW[i].iLink;
			cTemp += sizeof(int);
			*(int *)cTemp = pSW[i].iWindowType;
			cTemp += sizeof(int);
			*(DWORD *)cTemp = pSW[i].lPID;
			cTemp += sizeof(DWORD);
			*(DWORD *)cTemp = pSW[i].trayicon.Reserved[0];
			cTemp += sizeof(DWORD);
			*(DWORD *)cTemp = pSW[i].trayicon.Reserved[1];
			cTemp += sizeof(DWORD);
			*(HICON *)cTemp = pSW[i].trayicon.hIcon;
			cTemp += sizeof(HICON);
			*(HWND *)cTemp = pSW[i].trayicon.hwnd;
			cTemp += sizeof(HWND);
			*(UINT *)cTemp = pSW[i].trayicon.uCallbackMessage;
			cTemp += sizeof(UINT);
			*(UINT *)cTemp = pSW[i].trayicon.uID;
			cTemp += sizeof(UINT);			
		}
	}
	DWORD lWritten = 0,FileSizeHigh = 0;
	wchar_t * cWndInfoFileName = (wchar_t*) new wchar_t[1024];
	memset(cWndInfoFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileNameW(hInst,cWndInfoFileName,1024);
	PathRemoveFileSpec(cWndInfoFileName);
	wcscat(cWndInfoFileName,L"\\WndInfo.wif");
	if(IsWindowEnabled(hListView) != 0)
	{
		DeleteFile(cWndInfoFileName);
		delete [] cWndInfoFileName;
		VirtualFree(cFileContent,0,MEM_RELEASE);
		return 0;
	}
	if(wcslen(cFileContent) > 0)
	{
		HANDLE hWndInfoFile = CreateFileW(cWndInfoFileName,GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
		DWORD FileSizeLow = GetFileSize(hWndInfoFile,&FileSizeHigh);
		
		WriteFile(hWndInfoFile,cFileContent,iBaSW * 2048 * 2,&lWritten,NULL);
		CloseHandle(hWndInfoFile);
	}
	delete [] cWndInfoFileName;
	VirtualFree(cFileContent,0,MEM_RELEASE);
	
	return 1;
}






int ReadWndInfo(WndAndHandle * pSW,int * pBaSW)
{
	int i = 0;	
	DWORD dwFileSize = 0;
	wchar_t * cWndInfoFileName = (wchar_t*) new wchar_t[1024];
	memset(cWndInfoFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cWndInfoFileName,1024);
	PathRemoveFileSpec(cWndInfoFileName);
	wcscat(cWndInfoFileName,L"\\WndInfo.wif");
	HANDLE hWndInfoFile = CreateFile(cWndInfoFileName,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if(hWndInfoFile == NULL || hWndInfoFile == INVALID_HANDLE_VALUE)
	{
		delete [] cWndInfoFileName;
		return 0;
	}
	dwFileSize = GetFileSize(hWndInfoFile,NULL);
	wchar_t * cFileContent = (wchar_t*)VirtualAlloc(NULL,dwFileSize,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(cFileContent,0,dwFileSize * sizeof(wchar_t));
	wchar_t * cTemp = cFileContent;
	* pBaSW = dwFileSize/(2048 * 2);
	DWORD dwReaded = 0;
	ReadFile(hWndInfoFile,cFileContent,dwFileSize,&dwReaded,NULL);
	CloseHandle(hWndInfoFile);
	delete [] cWndInfoFileName;

	for(i = 0;i < *pBaSW;i ++)
	{
		pSW[i].bExist = *(int *)cTemp;
		cTemp += sizeof(int);
		pSW[i].bHide = *(int *)cTemp;
		cTemp += sizeof(int);
		pSW[i].bReserved = *(int *)cTemp;
		cTemp += sizeof(int);
		memcpy(pSW[i].cIconTip,cTemp,1024);
		cTemp += 1024;
		memcpy(pSW[i].cProcessName,cTemp,30);
		cTemp += 30;
		memcpy(pSW[i].cWndTitle,cTemp,2048);
		cTemp += 2048;
		pSW[i].hWnd = *(HWND *)cTemp;
		cTemp += sizeof(HWND);
		pSW[i].iHasTrayIcon = *(int *)cTemp;
		cTemp += sizeof(int);
		pSW[i].iIconIndex = *(int *)cTemp;
		cTemp += sizeof(int);
		pSW[i].iLink = *(int *)cTemp;
		cTemp += sizeof(int);
		pSW[i].iWindowType = *(int *)cTemp;
		cTemp += sizeof(int);
		pSW[i].lPID = *(DWORD *)cTemp;
		cTemp += sizeof(DWORD);
		pSW[i].trayicon.Reserved[0] = *(DWORD *)cTemp;
		cTemp += sizeof(DWORD);
		pSW[i].trayicon.Reserved[1] = *(DWORD *)cTemp;
		cTemp += sizeof(DWORD);
		pSW[i].trayicon.hIcon = *(HICON*)cTemp;
		cTemp += sizeof(HICON);
		pSW[i].trayicon.hwnd = *(HWND *)cTemp;
		cTemp += sizeof(HWND);
		pSW[i].trayicon.uCallbackMessage = *(UINT *)cTemp;
		cTemp += sizeof(UINT);
		pSW[i].trayicon.uID = *(UINT *)cTemp;
		cTemp += sizeof(UINT);
	}
	
	DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_DIALOG_RESUMETIP),
		hMainDialog,ResumeTipProc,(LPARAM)pSW);
	
	*pBaSW = 0;
	
	VirtualFree(cFileContent,0,MEM_RELEASE);

	return 1;
}



INT_PTR CALLBACK ResumeTipProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	int i = 0;
	HWND hListHided;
	static WndAndHandle * pHided = NULL;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		pHided = (WndAndHandle *)lParam;
		hListHided = GetDlgItem(hwndDlg,IDC_LIST_HIDEDWND);
		FillColumn(hListHided);
		FillRow(hListHided,pHided);
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
		case IDC_BUTTON_SHOWALL:
			i = 0;
			while(pHided[i].lPID != 0)
			{
				ShowWindow(pHided[i].hWnd,SW_SHOW);
				i ++;
			}
			EndDialog(hwndDlg,1);
			return FALSE;
		case IDC_BUTTON_KEEPHIDE:
			TakeToBk(pHided);
			EndDialog(hwndDlg,0);
			return FALSE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}


int FillColumn(HWND hList)
{
	LVCOLUMN LC;
	LC.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	LC.fmt  = LVCFMT_LEFT;
	LC.cx   = 300;
	LC.pszText = L"Window Title";
	ListView_InsertColumn(hList,0,&LC);

	LC.cx   = 80;
	LC.pszText = L"Process Name";
	ListView_InsertColumn(hList,1,&LC);
	
	return 1;
}

int FillRow(HWND hList,WndAndHandle * pHided)
{
	LVITEM lvItem;
	memset(&lvItem,0,sizeof(LVITEM));
	lvItem.mask = LVIF_TEXT;
	int i = 0;

	while(pHided[i].lPID != 0)
	{
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = pHided[i].cWndTitle;
		ListView_InsertItem(hList,&lvItem);
		lvItem.pszText = pHided[i].cProcessName;
		lvItem.iSubItem = 1;
		ListView_SetItem(hList,&lvItem);

		i ++;
	}

	return 1;
}


int TakeToBk(WndAndHandle * pHided)
{
	int iSize = 0,i = 0;
	while(pHided[i].lPID != 0)
	{
		i ++;
	}
	iSize = i;
	pTempBK = (WndAndHandle *)new WndAndHandle[100];//[iSize];
	memset(pTempBK,0,sizeof(WndAndHandle) * 100);//iSize);
	for(i = 0;i < iSize;i ++)
	{
		pTempBK[i].cWndTitle = new wchar_t[2048];
		memset(pTempBK[i].cWndTitle,0,2048 * sizeof(wchar_t));
		wcscpy(pTempBK[i].cWndTitle,pHided[i].cWndTitle);
//		pTempBK[i].cIconTip  = NULL;
		pTempBK[i].cIconTip  = new wchar_t[1024];
		memset(pTempBK[i].cIconTip,0,1024 * sizeof(wchar_t));
		wcscpy(pTempBK[i].cIconTip,pHided[i].cIconTip);
		pTempBK[i].bExist = pHided[i].bExist;
		pTempBK[i].bHide = pHided[i].bHide;
		pTempBK[i].bReserved = pHided[i].bReserved;
		wcscpy(pTempBK[i].cProcessName,pHided[i].cProcessName);
		pTempBK[i].hWnd = pHided[i].hWnd;
		pTempBK[i].iHasTrayIcon = pHided[i].iHasTrayIcon;
		pTempBK[i].iCommandId = pHided[i].iCommandId;
		pTempBK[i].iIconIndex = pHided[i].iIconIndex;
		pTempBK[i].iLink = pHided[i].iLink;
		pTempBK[i].iWindowType = pHided[i].iWindowType;
		pTempBK[i].lPID = pHided[i].lPID;
		pTempBK[i].trayicon = pHided[i].trayicon;
	}
	iTakeToBk = 1;
	
	return 1;
}



int BKHided()
{
	int i = 0;
	wchar_t cPID[20];
	WndAndHandle * pTemp = NULL;
	while(pTempBK[i].lPID != 0)
	{
		memset(cPID,0,20 * sizeof(wchar_t));
		swprintf(cPID,L"%X",pTempBK[i].lPID);
		pTemp = pTempBK + i;
		HideSelected(&pTemp,1,0);
		//ExtendWindow();
		SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
//		delete [] pTempBK[i].cWndTitle;
//		delete [] pTempBK[i].cIconTip;
		i ++;
	}
//	delete [] pTempBK;

	return 1;
}


int SetWindowListViewColumn(HWND hListView)
{
	LVCOLUMNW lvColumnW;
	DWORD lvStyle = 0;
	wchar_t pWindowTitleW[100],pProcessIDW[100],pTrayIconW[100];

	
	lvColumnW.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumnW.fmt  = LVCFMT_LEFT;
	lvColumnW.cx   = 400;


	if(g_iUseLanguage == 0)
	{
		lvColumnW.pszText = L"Window Title";
	}
	else
	{
		wchar_t * pColumnText = new wchar_t[1024];
		memset(pWindowTitleW,0,100 * sizeof(wchar_t));
		wcscpy(pWindowTitleW,LoadItemStringW(L"IDS_LISTVIEW_WINDOWTITLE",pColumnText,L"Window Title"));
		lvColumnW.pszText = pWindowTitleW;
		delete [] pColumnText;
	}
	ListView_SetColumn(hListView,0,&lvColumnW);
	lvColumnW.cx  = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumnW.pszText = L"Process ID";
	}
	else
	{
		wchar_t * pColumnText = new wchar_t[1024];
		memset(pProcessIDW,0,100 * sizeof(wchar_t));
		wcscpy(pProcessIDW,LoadItemStringW(L"IDS_LISTVIEW_PROCESSID",pColumnText,L"Process ID"));
		lvColumnW.pszText = pProcessIDW;
		delete [] pColumnText;
	}
	ListView_SetColumn(hListView,1,&lvColumnW);
	lvColumnW.cx  = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumnW.pszText = L"Tray Icon";
	}
	else
	{
		wchar_t * pColumnText = new wchar_t[1024];
		memset(pTrayIconW,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIconW,LoadItemStringW(L"IDS_LISTVIEW_TRAYICON",pColumnText,L"Tray Icon"));
		lvColumnW.pszText = pTrayIconW;
		delete [] pColumnText;
	}
	ListView_SetColumn(hListView,2,&lvColumnW);
	lvStyle = ListView_GetExtendedListViewStyle(hListView);
	lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hListView,0,lvStyle);
	

	return 1;
}


int SetBKListColumn(HWND hBKList)
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512],pProcessID[100];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Tray Icon";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pTrayIcon,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_LISTVIEWBK_TRAYICON",pHeaderText,L"Tray Icon"));
		lvColumn.pszText = pTrayIcon;
		delete [] pHeaderText;
	}
	ListView_SetColumn(hBKList,0,&lvColumn);
	lvColumn.cx    = 360;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Window Title or Tray Icon's Tooltip";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pRelatedTitle,0,512 * sizeof(wchar_t));
		wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_LISTVIEWBK_RELATEDTITLE",pHeaderText,
			L"Window Title or Tray Icon's Tooltip"));
		lvColumn.pszText = pRelatedTitle;
		delete [] pHeaderText;		
	}
	ListView_SetColumn(hBKList,1,&lvColumn);
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Process ID";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pProcessID,0,100 * sizeof(wchar_t));
		wcscpy(pProcessID,LoadItemStringW(L"IDS_LISTVIEWBK_PROCESSID",pHeaderText,
			L"Process ID"));
		lvColumn.pszText = pProcessID;
		delete [] pHeaderText;		
	}
	ListView_SetColumn(hBKList,2,&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_BK);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_BK,0,lvStyle);

	return 1;
}


int SetTrayListColumn(HWND hBKList)
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512],pProcessID[100];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Tray Icon";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pTrayIcon,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_LISTVIEWBK_TRAYICON",pHeaderText,L"Tray Icon"));
		lvColumn.pszText = pTrayIcon;
		delete [] pHeaderText;
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	ListView_SetColumn(g_TrayList,0,&lvColumn);
	lvColumn.cx    = 360;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Window Title or Tray Icon's Tooltip";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pRelatedTitle,0,512 * sizeof(wchar_t));
		wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_LISTVIEWBK_RELATEDTITLE",pHeaderText,
			L"Window Title or Tray Icon's Tooltip"));
		lvColumn.pszText = pRelatedTitle;
		delete [] pHeaderText;		
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
	ListView_SetColumn(g_TrayList,1,&lvColumn);
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Process ID";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pProcessID,0,100 * sizeof(wchar_t));
		wcscpy(pProcessID,LoadItemStringW(L"IDS_LISTVIEWBK_PROCESSID",pHeaderText,
			L"Process ID"));
		lvColumn.pszText = pProcessID;
		delete [] pHeaderText;		
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
	ListView_SetColumn(g_TrayList,2,&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(g_TrayList);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(g_TrayList,0,lvStyle);


	return 1;
}




int	SetIPListColumn(HWND g_IPList)
{

	UseProxyDlg dlg;
	dlg.SetListColumn();


	return 1;
}




int SetProcessListColumn(HWND hProcessList)
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 160;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Image Name";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pTrayIcon,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_PROCESSLISTVIEW_IMAGENAME",pHeaderText,
			L"Image Name"));
		lvColumn.pszText = pTrayIcon;
		delete [] pHeaderText;
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	ListView_SetColumn(hProcessList,0,&lvColumn);
	lvColumn.cx    = 360;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"PID";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pRelatedTitle,0,512 * sizeof(wchar_t));
		wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_PROCESSLISTVIEW_PID",pHeaderText,
			L"PID"));
		lvColumn.pszText = pRelatedTitle;
		delete [] pHeaderText;		
	}
//	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
	ListView_SetColumn(hProcessList,1,&lvColumn);





	lvStyle = ListView_GetExtendedListViewStyle(hProcessList);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hProcessList,0,lvStyle);


	return 1;
}


int SetAHListColumn(HWND hAHList)
{	
		LVCOLUMNW lvColumn;
		DWORD lvStyle = 0;
		wchar_t pTrayIcon[100],pRelatedTitle[512]/*,pProcessID[100]*/;
		lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
		lvColumn.fmt  = LVCFMT_LEFT;
		lvColumn.cx   = 80;
		if(g_iUseLanguage == 0)
		{
			lvColumn.pszText = L"ID";
		}
		else
		{
			wchar_t * pHeaderText = new wchar_t[1024];
			memset(pTrayIcon,0,100 * sizeof(wchar_t));
			wcscpy(pTrayIcon,LoadItemStringW(L"IDS_LISTVIEWAH_ID",pHeaderText,L"ID"));
			lvColumn.pszText = pTrayIcon;
			delete [] pHeaderText;
		}
		//SendMessageW(g_AutoHideList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
		ListView_SetColumn(g_AutoHideList,0,&lvColumn);
		lvColumn.cx    = 360;
		if(g_iUseLanguage == 0)
		{
			lvColumn.pszText = L"Process Name";
		}
		else
		{
			wchar_t * pHeaderText = new wchar_t[1024];
			memset(pRelatedTitle,0,512 * sizeof(wchar_t));
			wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_LISTVIEWAH_PROCESSNAME",pHeaderText,
				L"Process Name"));
			lvColumn.pszText = pRelatedTitle;
			delete [] pHeaderText;		
		}
		//SendMessageW(g_AutoHideList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
		ListView_SetColumn(g_AutoHideList,1,&lvColumn);
//		lvColumn.cx   = 80;//准备去掉
//		if(g_iUseLanguage == 0)
//		{
//			lvColumn.pszText = L"Type";
//		}
//		else
//		{
//			wchar_t * pHeaderText = new wchar_t[1024];
//			memset(pProcessID,0,100 * sizeof(wchar_t));
//			wcscpy(pProcessID,LoadItemStringW(L"IDS_LISTVIEWAH_TYPE",pHeaderText,
//				L"Type"));
//			lvColumn.pszText = pProcessID;
//			delete [] pHeaderText;
//		}
//		//SendMessageW(g_AutoHideList,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
		ListView_SetColumn(g_AutoHideList,2,&lvColumn);
		lvStyle = ListView_GetExtendedListViewStyle(g_AutoHideList);
		lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyleEx(g_AutoHideList,0,lvStyle);
		
		
		
		return 1;
}





int ResetPriTree()
{
	CClearDlg cDlg;
	cDlg.InitMainDlg(g_TabWnd[5]);


	return 1;
}




int TipIfExit()
{
	int iRet = 0;
	iRet = HDMessageBoxW(L"IDS_MESSAGEBOX_IFEXIT",L"Exit?",
		hMainDialog,MB_YESNO|MB_ICONINFORMATION);
	if(iRet == IDYES)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	return 0;
}


int Compatible25()
{
	CopyFile(L"temp\\HDUpdate.exe",L"HDUpdate.exe",FALSE);
	DeleteFile(L"temp\\HDUpdate.exe");
	RemoveDirectory(L"temp");

	return 1;
}



int DeleteDir(wchar_t * pDir)
{
	SHFILEOPSTRUCT SFO;
	memset(&SFO,0,sizeof(SFO));
	SFO.hwnd = NULL;
	SFO.wFunc = FO_DELETE;
	SFO.pFrom = pDir;
	SFO.pTo = NULL;
	SFO.fFlags = FOF_NOERRORUI|FOF_SILENT|FOF_NOCONFIRMATION;
	SFO.fAnyOperationsAborted = FALSE;
	SFO.hNameMappings = NULL;
	SFO.lpszProgressTitle = NULL;


	SHFileOperation(&SFO);

	
	return 1;
}

int RestoreAllIconTitle()
{
	int iItemCount = ListView_GetItemCount(hListView);
	int i = 0;
	for(i = 0;i < iItemCount;i ++)
	{
		RestoreTitle(i,1);
		RestoreIcon(i,0,1);
	}

	return 1;
}


int GetCurrentDirectory_HD(int iSize,wchar_t * cDir)
{
	memset(cDir,0,iSize * sizeof(wchar_t));
	GetModuleFileName(hInst,cDir,iSize);
	PathRemoveFileSpec(cDir);
	

	return 1;
}









int RegisterBKHotkey(WndAndHandle * pWH)
{
	sBKWHotkey * pBH = NULL;
	pBH = new sBKWHotkey;
	memset(pBH,0,sizeof(sBKWHotkey));

	
	pBH->cTitle = new wchar_t[2048];
	memset(pBH->cTitle,0,2048 * sizeof(wchar_t));
	memset(pBH->cPID,0,20 * sizeof(wchar_t));

	
	pBH->aHK = 0;
	pBH->uMF = 0;	
	pBH->uVK = 0;
	pBH->wHK = 0;
	pBH->Next = NULL;

	DBClass db_HK;
	wcscpy(pBH->cProcessName,pWH->cProcessName);
	db_HK.ReadHK(pBH);


	wchar_t cPID[20];
	memset(cPID,0,20);
	swprintf(cPID,L"%X",pWH->lPID);
	wcscpy(pBH->cPID,cPID);
	wcscpy(pBH->cTitle,pWH->cWndTitle);


	wchar_t * cAtom = new wchar_t[2048];
	memset(cAtom,0,2048 * sizeof(wchar_t));
	wcscpy(cAtom,L"REGHOTKEY");
	wcscat(cAtom,pBH->cPID);
	wcscat(cAtom,pBH->cTitle);
	pBH->aHK = GlobalFindAtom(cAtom);
//	UnregisterHotKey(hMainDialog,pBH->aHK);

	pBH->aHK = GlobalAddAtom((LPCTSTR)cAtom);


	BOOL bSuc = RegisterHotKey(hMainDialog,pBH->aHK,pBH->uMF,pBH->uVK);

	DWORD dwErr = GetLastError();

	delete [] cAtom;
	if (bSuc == 0)
	{	
		delete [] pBH->cTitle;
		delete pBH;

		return 0;
	}
	else
	{
		g_BKHList.push_back(pBH);
		
		return 1;
	}	


	return 1;
}




int GetProcessTrayIcon(DWORD lPID,TBBUTTON * pTB,TRAYDATA * pTrayData)
{
	HWND hTrayWnd = FindTrayToolbarWindow();

	DWORD dwPID = 0;
	GetWindowThreadProcessId(hTrayWnd,&dwPID);
	BOOL ret=FALSE;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS
		|PROCESS_VM_OPERATION
		|PROCESS_VM_READ
		|PROCESS_VM_WRITE,
		FALSE,
		dwPID);
	if(hProcess == NULL)
	{
		return 0;
	}
	
	PVOID ptb=VirtualAllocEx(hProcess,NULL,sizeof(TBBUTTON),MEM_COMMIT,PAGE_READWRITE);

	memset(pTB,0,sizeof(TBBUTTON));
	HICON hIcon = NULL;
	
	ULONG lButtons=(ULONG)SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);
	for(ULONG i=0;i<lButtons;++i)
	{
		SendMessage(hTrayWnd,TB_GETBUTTON,WPARAM(i),LPARAM(ptb));
        ReadProcessMemory(hProcess,ptb,pTB,sizeof(TBBUTTON),NULL);

		ReadProcessMemory(hProcess,(PVOID)(pTB->dwData),pTrayData,sizeof(TRAYDATA),NULL);
		
		HWND hWnd=pTrayData->hwnd;
		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hWnd,&dwProcessID);
		if(dwProcessID == lPID)
		{
			SendMessage(hTrayWnd,g_HD_Msg_GetIcon,WPARAM(pTB->iBitmap),LPARAM(&hIcon));
			pTrayData->hIcon = hIcon;
			pTrayData->Reserved[0] = 0;
			pTrayData->Reserved[1] = 0;
			if(hIcon == NULL)
			{
				hIcon = ReadTrayIcon_dll();
				pTrayData->hIcon = hIcon;
				pTrayData->Reserved[0] = 22;
				pTrayData->Reserved[1] = 33;
			}
			break;
		}
    }
	
	VirtualFreeEx(hProcess,ptb,0,MEM_RELEASE);

    CloseHandle(hProcess);
	hProcess = NULL;

	

	return 1;
}

int DelInvalidTrayIcon()
{
	





	return 1;
}




HWND FindTaskbarWnd()
{
	HWND hTray    = ::FindWindow(L"Shell_TrayWnd", NULL);

	HWND hReBar   = FindWindowEx( hTray    , 0, L"ReBarWindow32"   , NULL );
	HWND hTask    = FindWindowEx( hReBar   , 0, L"MSTaskSwWClass"  , NULL );
	HWND hToolbar = FindWindowEx( hTask    , 0, L"ToolbarWindow32" , NULL );


	if(IsWindow(hToolbar))
	{
		return hToolbar;
	}
	else
	{
		return NULL;
	}
}



int IsInSW(HWND hWnd)
{
	int i = 0;

	for(i = 0;i < iBaSW;i ++)
	{
		if(sW[i].hWnd == hWnd && 
			sW[i].bExist != 0)
		{
			return 1;
		}
	}



	return 0;
}





int IsInBKSW(HWND hWnd)
{
	int i = 0;

	for(i = 0;i < iW_Sel;i ++)
	{
		if(sBKSel[i].hWnd == hWnd)
		{
			return 1;
		}
	}



	return 0;
}








int AppendSW(HWND hWnd)
{
	GetWindowThreadProcessId(hWnd,&sW[iBaSW].lPID);
	sW[iBaSW].hWnd = hWnd;
	sW[iBaSW].iWindowType = 1;
	sW[iBaSW].bExist = 1;
	sW[iBaSW].bReserved = 0;
	GetWindowText(hWnd,sW[iBaSW].cWndTitle,2048);
	sW[iBaSW].bHide = 0;
	GetProcessName(sW[iBaSW].lPID,sW[iBaSW].cProcessName);
	sW[iBaSW].bExist = 1;
	sW[iBaSW].bToBk = 0;

	TRAYDATA * pTrayinfo = NULL;	
	IsHaveTrayIcon(sW + iBaSW ,pTrayinfo);

	iBaSW ++;

//	sW[iBaSW].iHasTrayIcon = sW[iBaSW+1].iHasTrayIcon;
//	sW[iBaSW].iIconIndex = sW[iBaSW+1].iIconIndex;
//	sW[iBaSW].trayicon = sW[iBaSW+1].trayicon;
//	sW[iBaSW].iCommandId = 0;
//	memset(sW[iBaSW].cIconTip,0,1024 * sizeof(wchar_t));
//	wcscpy(sW[iBaSW].cIconTip,sW[iBaSW+1].cIconTip);	




	return 1;
}



int GetProcessName(DWORD lPID,wchar_t * pName)
{
	PROCESSENTRY32 pe32;


	HANDLE hProcessSnap = NULL;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First((void*)hProcessSnap,&pe32);
	while(Process32Next((void*)hProcessSnap,&pe32))
	{
		if(pe32.th32ProcessID == lPID)
		{
			if(wcscmp(wcslwr(pe32.szExeFile),L"iexplore.exe")==0)
			{
				wcscpy(pName,pe32.szExeFile);
			}
			else if(wcscmp(wcslwr(pe32.szExeFile),L"explorer.exe") == 0)
			{
				wcscpy(pName,pe32.szExeFile);
			}
			else
			{
				wcscpy(pName,pe32.szExeFile);
			}
		}
	}
	CloseHandle((HANDLE)hProcessSnap);




	return 1;
}





// DoCreateTabControl - creates a tab control, sized to fit the 
//     specified parent window's client area, and adds some tabs. 
// Returns the handle to the tab control.
// hwndParent - parent window (the application's main window).
HFONT hFont_New2 = NULL;
LOGFONT lf2;
HWND WINAPI DoCreateTabControl(HWND hwndParent)
{ 
    RECT rcClient; 
    HWND hwndTab; 

	hwndParent = hMainDialog;//GetDlgItem(hMainDialog,IDC_STATIC_FOREGROUND);
 
    // Get the dimensions of the parent window's client area, and 
    // create a tab control child window of that size. 
    GetClientRect(hMainDialog, &rcClient); 
    InitCommonControls(); 
    hwndTab = CreateWindow( 
        WC_TABCONTROL,L"TAB2", 
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
        20, 30, 580, 120, 
        hwndParent, NULL,hInst , NULL 
        ); 
    if (hwndTab == NULL)
        return NULL; 

	lf2.lfHeight = 15;
	lf2.lfWidth = 0;
	lf2.lfEscapement = 0;
	lf2.lfOrientation = 0;
	lf2.lfWeight = 0;
	lf2.lfItalic = FALSE;
	lf2.lfUnderline = FALSE;
	lf2.lfStrikeOut = FALSE;
	lf2.lfCharSet = DEFAULT_CHARSET;
	lf2.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf2.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf2.lfQuality = DEFAULT_QUALITY;
	lf2.lfPitchAndFamily = DEFAULT_PITCH;
	wcscpy(lf2.lfFaceName,L"MS Sans Serif");
	hFont_New2 = CreateFontIndirect(&lf2);
	SendMessage(hwndTab,WM_SETFONT,(WPARAM)hFont_New2,0);
	RECT mainRect;
	GetWindowRect(hMainDialog,&mainRect);

	SetWindowPos(hwndTab,NULL,10,30,
		mainRect.right-mainRect.left-20,mainRect.bottom-mainRect.top-155,SWP_NOZORDER);



	g_Tab = hwndTab;

	CreateImageListForTab();
	

	

// 	TabCtrl_SetItemSize(hwndTab,50,20);
// 	TabCtrl_SetPadding(hwndTab,5,0);

    // Add tabs for each day of the week.
    TCITEMW tie; 
	memset(&tie,0,sizeof(tie));

	if(g_UseIcon == 1)
	{
		tie.mask = TCIF_TEXT | TCIF_IMAGE; 
	}
	else
	{
		tie.mask = TCIF_TEXT; 
	}
    tie.iImage = 0; 
	wchar_t * pText = new wchar_t[1024];
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_WNDLIST",pText,L"Window List");

 
	TabCtrl_InsertItem(hwndTab, 0, &tie);
    tie.iImage = 1; 

	tie.pszText = LoadItemStringW(L"IDC_TABNAME_TRAYICON",pText,L"Tray Icon List");
 
	TabCtrl_InsertItem(hwndTab, 1, &tie);

	tie.iImage = 2; 
   tie.pszText = LoadItemStringW(L"IDC_TABNAME_BKLIST",pText,L"Backgrounded Program List");
 
	TabCtrl_InsertItem(hwndTab, 2, &tie);

#ifndef _NOADS
	tie.iImage = 3; 

    tie.pszText = LoadItemStringW(L"IDC_TABNAME_WEBPAGE",pText,L"Web Page");
 
	TabCtrl_InsertItem(hwndTab, 3, &tie);
#endif

#ifdef _HIDEPROCESS
	
    tie.iImage = 4; 
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_PROCESSLIST",pText,L"Process List Page");
 
	TabCtrl_InsertItem(hwndTab, 4, &tie);
#endif
	


    tie.iImage = 3; 
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_CLEANPRIVACY",pText,L"Clean Privacy");
 
	TabCtrl_InsertItem(hwndTab, 3, &tie);
	
 //   tie.iImage = 5; 
 //  tie.pszText = LoadItemStringW(L"IDC_TABNAME_HIDEIP",pText,L"Hide IP");

	//TabCtrl_InsertItem(hwndTab, 6, &tie);



    tie.iImage = 4; 
	tie.pszText = LoadItemStringW(L"IDC_TABNAME_AUTOHIDE",pText,L"Auto Hide");

	TabCtrl_InsertItem(hwndTab, 4, &tie);


	delete [] pText;

	g_Tab = hwndTab;

    return hwndTab; 
}




BOOL CreateImageListForTab()
{
	g_ImageList_Tab = ImageList_Create(16,16,ILC_COLOR24,7,7);
	


	HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_ALLWND));
	int iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);
	

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_TRAYICON));
	iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);

	

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_BKLIST));
	iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);


	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_ADSUNION));
	iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);

	
	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_CLEAN));
	iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);




	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_HIDEIP));
	iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);


	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAB_AUTOHIDE));
	iRet = ImageList_Add (g_ImageList_Tab,(HBITMAP)hBitmap,NULL);
	DeleteObject(hBitmap);





	TabCtrl_SetImageList(g_Tab,g_ImageList_Tab);

	

	


	return TRUE;
}


BOOL DestroyImageListForTab()
{
	ImageList_Destroy(g_ImageList_Tab);
	
	
	
	
	
	return TRUE;
}






int SetTabLanguage()
{
    TCITEMW tie; 
	memset(&tie,0,sizeof(tie));

    tie.mask = TCIF_TEXT | TCIF_IMAGE; 
    tie.iImage = -1; 
	wchar_t * pText = new wchar_t[1024];
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_WNDLIST",pText,L"Window List");

 
	TabCtrl_SetItem(g_Tab, 0, &tie);

	tie.pszText = LoadItemStringW(L"IDC_TABNAME_TRAYICON",pText,L"Tray Icon List");
 
	TabCtrl_SetItem(g_Tab, 1, &tie);

    tie.pszText = LoadItemStringW(L"IDC_TABNAME_BKLIST",pText,L"Backgrounded Program List");
 
	TabCtrl_SetItem(g_Tab, 2, &tie);

#ifndef _NOADS
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_WEBPAGE",pText,L"Web Content");
 
	TabCtrl_SetItem(g_Tab, 3, &tie);

#endif


#ifdef _HIDEPROCESS
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_PROCESSLIST",pText,L"Process List");
 
	TabCtrl_SetItem(g_Tab, 4, &tie);
#endif
 
#ifndef _NOADS
#ifdef _HIDEPROCESS
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_CLEANPRIVACY",pText,L"Clean Privacy");

	TabCtrl_SetItem(g_Tab, 5, &tie);

   tie.pszText = LoadItemStringW(L"IDC_TABNAME_HideIP",pText,L"Hide IP");

	TabCtrl_SetItem(g_Tab, 6, &tie);

	tie.pszText = LoadItemStringW(L"IDC_TABNAME_AUTOHIDE",pText,L"AUTOHIDE");
	
	TabCtrl_SetItem(g_Tab, 7, &tie);

#else
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_CLEANPRIVACY",pText,L"Clean Privacy");
	TabCtrl_SetItem(g_Tab, 3, &tie);

	tie.pszText = LoadItemStringW(L"IDC_TABNAME_AUTOHIDE",pText,L"AUTOHIDE");
	TabCtrl_SetItem(g_Tab, 4, &tie);

	//tie.pszText = LoadItemStringW(L"IDC_TABNAME_AUTOHIDE",pText,L"AUTOHIDE");
	//
	//TabCtrl_SetItem(g_Tab, 6, &tie);


#endif
#endif



#ifdef _NOADS
#ifdef _HIDEPROCESS
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_CLEANPRIVACY",pText,L"Clean Privacy");
	
	TabCtrl_SetItem(g_Tab, 4, &tie);
	
	tie.pszText = LoadItemStringW(L"IDC_TABNAME_HideIP",pText,L"Hide IP");
	
	TabCtrl_SetItem(g_Tab, 5, &tie);
#else
    tie.pszText = LoadItemStringW(L"IDC_TABNAME_CLEANPRIVACY",pText,L"Clean Privacy");
	
	TabCtrl_SetItem(g_Tab, 3, &tie);
	tie.pszText = LoadItemStringW(L"IDC_TABNAME_AUTOHIDE",pText,L"AUTOHIDE");
	TabCtrl_SetItem(g_Tab, 4, &tie);
#endif



#endif


	delete [] pText;


	
	return 1;
}


// DoCreateDisplayWindow - creates a child window (a static 
//     control) to occupy the tab control's display area. 
// Returns the handle to the static control. 
// hwndParent - parent window (the application's main window). 
 
HWND WINAPI DoCreateDisplayWindow(HWND hwndParent) 
{
    HWND hwndStatic = CreateWindow(L"STATIC", L"THIS IS A TEST.", 
        WS_CHILD | WS_VISIBLE | WS_BORDER, 
        10, 30, 109, 30, 
        g_Tab, NULL, hInst, NULL);
 
    return hwndStatic;
}




int OnTabSelChange(int iPage)
{
	switch(iPage)
	{
	case 0:
		HideOtherPage(iPage);
		ShowAllWndPage();
		ChangeMenuState(0);
		break;
	case 1:
		HideOtherPage(iPage);
		ShowTrayPage();
		FillTrayList();
		ChangeMenuState(1);
		break;
	case 2:
		HideOtherPage(iPage);
		ShowBKPage();
		ChangeMenuState(2);
		break;
	case 3:
		HideOtherPage(iPage);
#ifndef _NOADS


		ShowWebPage();
#else
		ShowCleanPage();
#endif

		ChangeMenuState(3);
		break;
	case 4:
		HideOtherPage(4);//iPage);

#ifndef _NOADS
		ShowCleanPage();
#else

		ShowAutoHidePage();
#endif
		ChangeMenuState(5);
		break;
	case 5:
		HideOtherPage(6);//);
		ShowHideIPPage();
		ChangeMenuState(6);
		break;
	case 6:
		HideOtherPage(7);//);
		ShowAutoHidePage();
		ChangeMenuState(7);
		break;
	default:
		break;
	}


	return 1;
}






int ShowAllWndPage()
{
	HWND hAllWndPage = NULL;
	if(IsWindow(g_TabWnd[0]) == FALSE)
	{
		hAllWndPage = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_ALLWND),
			g_Tab,AllWndPageProc);
		g_TabWnd[0] = hAllWndPage;
	}

	


	ShowWindow(g_TabWnd[0],SW_SHOW);

	
	
	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);

//	SetWindowPos(g_TabWnd[0],NULL,10,30,
//		tabRect.right-tabRect.left-20,tabRect.bottom-tabRect.top-45,SWP_NOZORDER);
	
	SetWindowPos(g_TabWnd[0],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	

	return 1;
}




int ShowWebPage()
{
	HWND hWebPage = NULL;
	if(IsWindow(g_TabWnd[3]) == FALSE)
	{
		hWebPage = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_WEBPAGE),
			g_Tab,WebPageProc);
		g_TabWnd[3] = hWebPage;
	}



	HWND hStatic = GetDlgItem(g_TabWnd[3],IDC_STATIC_WEBPOS);

	wchar_t * cURL = new wchar_t[1024];
	memset(cURL,0,1024*sizeof(wchar_t));

	LoadItemStringW(L"IDS_UNIONPAGE_DEFAULT",cURL,L"http://www.store3000.com/hidedragon");


	DisplayHTMLPage(hStatic,cURL/*L"http://www.store3000.com/hidedragon"*/);
	g_oleemb = 1;

	delete [] cURL;

	ShowWindow(g_TabWnd[3],SW_SHOW);

	
	
	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);

	
	SetWindowPos(g_TabWnd[3],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	

	return 1;
}



int ShowTrayPage()
{
	HWND hTrayIconPage = NULL;
	if(IsWindow(g_TabWnd[1]) == FALSE)
	{
		hTrayIconPage = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_TRAYICON),
			g_Tab,TrayPageProc);
		g_TabWnd[1] = hTrayIconPage;
	}



	ShowWindow(g_TabWnd[1],SW_SHOW);

	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);
	
//	SetWindowPos(g_TabWnd[1],NULL,10,30,
//		tabRect.right-tabRect.left-20,tabRect.bottom-tabRect.top-45,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[1],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

	return 1;
}

int ShowBKPage()
{
	HWND hBKPage = NULL;
	if(IsWindow(g_TabWnd[2]) == FALSE)
	{
		hBKPage = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_BKLIST),
			g_Tab,BKPageProc);
		g_TabWnd[2] = hBKPage;
	}



	ShowWindow(g_TabWnd[2],SW_SHOW);

	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);
	
//	SetWindowPos(g_TabWnd[2],NULL,10,30,
//		tabRect.right-tabRect.left-20,tabRect.bottom-tabRect.top-45,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[2],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

	return 1;
}




int ShowProcessPage()
{
	HWND hProcessPage = NULL;
	if(IsWindow(g_TabWnd[4]) == FALSE)
	{
		hProcessPage = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_PROCESSLIST),
			g_Tab,ProcessPageProc);
		g_TabWnd[4] = hProcessPage;
	}



	ShowWindow(g_TabWnd[4],SW_SHOW);

	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);
	
//	SetWindowPos(g_TabWnd[2],NULL,10,30,
//		tabRect.right-tabRect.left-20,tabRect.bottom-tabRect.top-45,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[4],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

	return 1;
}


int ShowCleanPage()
{
	if(IsWindow(g_TabWnd[5]) == FALSE)
	{	
		CClearDlg dlg;
		dlg.Create(g_Tab,hInst);
	}



	ShowWindow(g_TabWnd[5],SW_SHOW);

	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);
	
	SetWindowPos(g_TabWnd[5],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

	return 1;
}


int ShowHideIPPage()
{
	HWND hHideIPPage = NULL;
	if(IsWindow(g_TabWnd[6]) == FALSE)
	{	
		UseProxyDlg dlg;
		dlg.create(g_Tab,hInst);
	}




	ShowWindow(g_TabWnd[6],SW_SHOW);

	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);
	
	SetWindowPos(g_TabWnd[6],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

	return 1;
}




int ShowAutoHidePage()
{
	HWND hAutoHidePage = NULL;
	if(IsWindow(g_TabWnd[7]) == FALSE)
	{	
		 		hAutoHidePage = CreateDialog(hInst,MAKEINTRESOURCE(IDD_TAB_AUTOHIDE),
		 			g_Tab,AutoHideProc);
		 		g_TabWnd[7] = hAutoHidePage;
	}
	
	
	
	
	ShowWindow(g_TabWnd[7],SW_SHOW);
	
	RECT tabRect;
	GetWindowRect(g_Tab,&tabRect);
	

	SetWindowPos(g_TabWnd[7],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	

	return 1;
}



int HideOtherPage(int iPage)
{

	for(int i = 0;i < g_iTabCount;i ++)
	{
		if(i != iPage)
		{
			ShowWindow(g_TabWnd[i],SW_HIDE);
		}
	}


	return 1;
}



HBRUSH g_brushAllPage,g_brushTrayPage,g_brushBKPage;
HDC tabDC = NULL;
COLORREF cr = NULL;



HWND hList_AllWnd, hToolTip1= NULL,hBtn_SwitchHide = NULL;

extern HWND hToolTip;
CHyperLink m_lnk_wndlist;

HWND hToolTip_allwnd;


CPictureEx m_flash;



INT_PTR CALLBACK AllWndPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hBtn_Refresh = NULL,hListView = NULL,hChk_SelHide = NULL;
	HWND hAdsLink = NULL;
	TOOLINFO ti;
	LPTOOLTIPTEXT lpttt;
	int idCtrl;
	wchar_t * pTipText2 = NULL;
	
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		g_brushAllPage = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_TAB_ALLWND);
		}
		if(sOP.bReserveSel == 0)
		{
			CheckDlgButton(hwndDlg,IDC_TAB_CHECK_SELHIDE,BST_CHECKED);
		}
		else 
		{
			CheckDlgButton(hwndDlg,IDC_TAB_CHECK_SELHIDE,BST_UNCHECKED);
		}
		hList_AllWnd = ::GetDlgItem(hwndDlg,IDC_TAB_LIST_ALLWND);
		hToolTip_allwnd = CreateWindowExW(NULL,TOOLTIPS_CLASSW,NULL,
			WS_POPUP|TTS_ALWAYSTIP|TTS_BALLOON|TTS_NOPREFIX,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwndDlg,NULL,hInst,NULL);
		ti.cbSize     = sizeof (TOOLINFO);
		ti.hinst      = hInst;
		ti.uFlags     = TTF_IDISHWND;
		ti.hwnd       = hwndDlg;
		ti.uId        = (UINT_PTR)hList_AllWnd;
		ti.lpszText   = LPSTR_TEXTCALLBACK;
		SendMessage(hToolTip_allwnd,TTM_ADDTOOL,0,(LPARAM)&ti);
		hBtn_SwitchHide = ::GetDlgItem(hwndDlg,IDC_TAB_CHECK_SELHIDE);
		ti.cbSize     = sizeof (TOOLINFO);
		ti.hinst      = hInst;
		ti.uFlags     = TTF_IDISHWND;
		ti.hwnd       = hwndDlg;
		ti.uId        = (UINT_PTR)hBtn_SwitchHide;
		ti.lpszText   = LPSTR_TEXTCALLBACK;

		SendMessage(hToolTip_allwnd,TTM_ADDTOOL,0,(LPARAM)&ti);

		{

			HWND ads_all = GetDlgItem(hwndDlg,IDC_STATIC_ADS1);

			ShowWindow(ads_all,FALSE);

	#ifndef _NOADS
			m_flash.Attach(ads_all);
			
			

			wchar_t cFileName[1024] = L"";
			GetCurrentDirectory_HD(1024,cFileName);
			wcscat(cFileName,L"\\gif\\gif_all_def.gif");
			m_flash.Load(cFileName);
			m_flash.Draw();
#endif
			PostMessage(hwndDlg,WM_SIZE,NULL,NULL);
		}

		hAdsLink = GetDlgItem(hwndDlg,IDC_WNDLIST_ADS);
		
#ifndef _NOADS
		m_lnk_wndlist.Attach(hAdsLink);
// 		m_lnk_wndlist.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.store3000.com/hidedragon", TRUE);

		{
			wchar_t * cDefaultText = new wchar_t[1024];
			memset(cDefaultText,0,1024*sizeof(wchar_t));
			LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
			
			wchar_t * cDefaultLink = new wchar_t[1024];
			memset(cDefaultLink,0,1024*sizeof(wchar_t));
			
			LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.store3000.com/hidedragon");
			
			
			m_lnk_wndlist.SetLink(cDefaultText,cDefaultLink, TRUE);
			
			
			delete [] cDefaultText;
			cDefaultText = NULL;
			delete [] cDefaultLink;
			cDefaultLink = NULL;

		}







#endif
		
#ifdef _NOADS
		ShowWindow(hAdsLink,SW_HIDE);
#endif

		if(g_UseIcon == 1)
		{
			HWND hHideThisWnd = GetDlgItem(hwndDlg,IDC_WNDLIST_HIDE);
			HWND hRefresh = GetDlgItem(hwndDlg,IDC_TAB_BTN_REFRESH);



			HICON hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_ALLWND_HIDETHISWINDOW),IMAGE_ICON,16,16,NULL);



			SendMessage(hHideThisWnd,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);

			DestroyIcon(hI3);

			hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_ALLWND_REFRESH),IMAGE_ICON,16,16,NULL);
			
			
			
			SendMessage(hRefresh,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);
			
			
			RECT rtM;
			rtM.left = 0;
			rtM.right = 20;
			rtM.top = 0;
			rtM.bottom = 00;
			Button_SetTextMargin(hHideThisWnd,&rtM);

			Button_SetTextMargin(hRefresh,&rtM);


		}

		return FALSE;
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
		return (LRESULT)g_brushAllPage;
	case WM_CTLCOLORDLG:
		return (LRESULT)g_brushAllPage;
	case WM_SETCURSOR:
		{
#ifndef _NOADS

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_STATIC_ADS1);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);

			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);

			if (PtInRect(&rtGif,pMouse))
			{
				SetCursor(hHand);
			}
			else
			{
				SetCursor(hNorm);
			}
			


#endif

			return TRUE;
			break;
		}

	case WM_SIZING:
	case WM_SIZE:
		{
			hListView = GetDlgItem(hwndDlg,IDC_TAB_LIST_ALLWND);
			hBtn_Refresh = GetDlgItem(hwndDlg,IDC_TAB_BTN_REFRESH);
			hChk_SelHide = GetDlgItem(hwndDlg,IDC_TAB_CHECK_SELHIDE);
			HWND hHideNow = GetDlgItem(hwndDlg,IDC_WNDLIST_HIDE);
			hAdsLink = GetDlgItem(hwndDlg,IDC_WNDLIST_ADS);

			RECT pageRect;
			GetWindowRect(hwndDlg,&pageRect);

#ifndef _NOADS

			SetWindowPos(hListView,NULL,10,50,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-149,SWP_NOZORDER);
#else

			SetWindowPos(hListView,NULL,10,50,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-79,SWP_NOZORDER);

#endif

			LONG lWidth = pageRect.right-pageRect.left;


			SetWindowPos(hHideNow,NULL,10,10,0,0,SWP_NOZORDER|SWP_NOSIZE);



			SendMessageW(hListView,LVM_SETCOLUMNWIDTH,0,
				(LPARAM)MAKELPARAM((int)((lWidth/3)*2), 0));
			SendMessageW(hListView,LVM_SETCOLUMNWIDTH,1,
				(LPARAM)MAKELPARAM((int)80, 0));
			SendMessageW(hListView,LVM_SETCOLUMNWIDTH,2,
				(LPARAM)MAKELPARAM((int)80, 0));


			RECT adlinkRect;
			GetWindowRect(hAdsLink,&adlinkRect);
			RECT btnRect;
			GetWindowRect(hHideNow,&btnRect);
			LONG btnLen = btnRect.right - btnRect.left;
			LONG adlinkLen = adlinkRect.right - adlinkRect.left;
			SetWindowPos(hAdsLink,NULL,btnLen + 30,15,pageRect.right-pageRect.left - (btnLen + 50),
				20,SWP_NOZORDER);







			RECT refreshRect;
			GetWindowRect(hBtn_Refresh,&refreshRect);
			LONG refreshLen = refreshRect.right - refreshRect.left;


#ifndef _NOADS

			SetWindowPos(hBtn_Refresh,NULL,pageRect.right-pageRect.left-refreshLen-9,
				pageRect.bottom-pageRect.top-95,0,0,SWP_NOZORDER|SWP_NOSIZE);
			



			SetWindowPos(hChk_SelHide,NULL,10,
				pageRect.bottom-pageRect.top-93,0,0,SWP_NOZORDER|SWP_NOSIZE);




#else
			SetWindowPos(hBtn_Refresh,NULL,pageRect.right-pageRect.left-refreshLen-9,
				pageRect.bottom-pageRect.top-25,0,0,SWP_NOZORDER|SWP_NOSIZE);



			SetWindowPos(hChk_SelHide,NULL,10,
				pageRect.bottom-pageRect.top-23,0,0,SWP_NOZORDER|SWP_NOSIZE);


#endif


			HWND ads_all = GetDlgItem(hwndDlg,IDC_STATIC_ADS1);
			SetWindowPos(ads_all,NULL,10,pageRect.bottom-pageRect.top-63,
				pageRect.right-pageRect.left - 20,60/*pageRect.bottom-pageRect.top-179*/,SWP_NOZORDER);



		}
		break;
	case WM_LBUTTONDOWN:
		{
#ifndef _NOADS

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_STATIC_ADS1);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
		
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",urlArray[0]->cUrl,NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			else
			{
				SetCursor(hNorm);
			}

#endif
			
			return TRUE;
			break;
		}

		break;
	case WM_CLOSE:
		DeleteObject((HGDIOBJ)g_brushAllPage);
		break;
	case WM_DESTROY:
		DeleteObject((HGDIOBJ)g_brushAllPage);
		DestroyWindow(g_TabWnd[0]);
		break;
	case WM_NOTIFY:
		{
			switch((((LPNMHDR)lParam)->code))
			{
			case  TTN_GETDISPINFO:
				idCtrl = ::GetDlgCtrlID((HWND)((LPNMHDR)lParam)->idFrom);
				lpttt  = (LPTOOLTIPTEXTW)lParam;
				switch(idCtrl)
				{
				case IDC_TAB_LIST_ALLWND:
					if(sOP.bShowTip == 1)
					{
						if(g_iUseLanguage == 1)
						{
							pTipText2 = new wchar_t[1024];
							wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_LISTVIEW",pTipText2));
							delete [] pTipText2;
						}
						else
						{
							lpttt ->lpszText = L"Click mouse right button to access more functions.";
						}
					}

					break;
				case IDC_TAB_CHECK_SELHIDE:
					if(sOP.bShowTip == 1)
					{
						if(g_iUseLanguage == 1)
						{
							pTipText2 = new wchar_t[1024];
							wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_SWITCH",pTipText2,
								L"Switch What Window Will Hide."));
							delete [] pTipText2;
						}
						else
						{
							lpttt ->lpszText = L"Switch What Window Will Hide.";
						}
					}
					break;
				default:
					return FALSE;
				}
				break;
			case NM_DBLCLK:
			case NM_CLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem < 0 && pIA->hdr.idFrom == IDC_TAB_LIST_ALLWND)
					{
						SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,0);
						//SendMessage(hwndDlg,WM_COMMAND,IDC_TAB_BTN_REFRESH,0);
					}
					else if(pIA->iItem >= 0 && pIA->hdr.idFrom == IDC_TAB_LIST_ALLWND )
					{
						if(sOP.bBeginnerModel == 0)
						{
							//break;
						}
						SendMessage(hMainDialog,WM_NOTIFY,wParam,lParam);
					}
				}
				
				break;
			case NM_RCLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem < 0 && pIA->hdr.idFrom == IDC_TAB_LIST_ALLWND)
					{
						//SendMessage(hMainDialog,WM_COMMAND,IDC_TAB_BTN_REFRESH,0);
						SendMessage(hMainDialog,WM_NOTIFY,wParam,lParam);
					}
					else if(pIA->iItem >= 0 && pIA->hdr.idFrom == IDC_TAB_LIST_ALLWND)
					{
						SendMessage(hMainDialog,WM_NOTIFY,wParam,lParam);
					}
				}
				break;

				return FALSE;
//			case NM_DBLCLK:
//				SendMessage(hMainDialog,WM_NOTIFY,wParam,lParam);
//				break;
			case LVN_ITEMCHANGED:
				SendMessage(hMainDialog,WM_NOTIFY,wParam,lParam);
				break;

			default:
				return FALSE;
			}
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_WNDLIST_HIDE:
			{
				int iSelMark = 0;
				hListView = GetDlgItem(hwndDlg,IDC_TAB_LIST_ALLWND);
				iSelMark = ListView_GetSelectionMark(hListView);
				if(iSelMark < 0)
				{
					HDMessageBoxW(L"IDS_MESSAGEBOX_PLEASESELECTHIDEITEM",L"PLEASE SELECT TO HIDE IWNDOW");
				}
				BKCurrentWindow(iSelMark);



				m_flash.Load(L"gif_tray.gif");
				m_flash.Draw();
				HWND hwnd_gif = GetDlgItem(hwndDlg,IDC_STATIC_ADS1);
				PostMessage(hwndDlg,WM_SIZE,NULL,NULL);
				InvalidateRect(hwnd_gif,NULL,TRUE);
				break;
			}
			break;
		case IDC_TAB_CHECK_SELHIDE:
			if(IsDlgButtonChecked(hwndDlg,IDC_TAB_CHECK_SELHIDE) == BST_CHECKED)
			{
				SendMessage(hMainDialog,WM_COMMAND,IDM_HIDESEL,NULL);
			}
			else
			{
				SendMessage(hMainDialog,WM_COMMAND,IDM_RESERVESEL,NULL);
			}
			break;
		case IDC_TAB_BTN_REFRESH:
			SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,0);
			break;
		case IDCANCEL:

			break;

		default:
			return FALSE;
		}
	}

	return FALSE;
}


HBRUSH g_brushWebPage;



CPictureEx gif_web;

INT_PTR CALLBACK WebPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hStatic = NULL;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		g_brushWebPage = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
		{
			HWND hStatic = GetDlgItem(hwndDlg,IDC_STATIC_WEBPOS);

			EmbedBrowserObject(hStatic);
		}
		
		{		
// 			HWND ads_web = GetDlgItem(hwndDlg,IDC_WEB_ADS);
// 			
// 			gif_web.Attach(ads_web);
// 			gif_web.Load(L"gif_web.gif");
// 			gif_web.Draw();
		}
		return FALSE;
	case WM_SETCURSOR:
		{
			break;

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_WEB_ADS);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				SetCursor(hHand);
			}
			else
			{
				SetCursor(hNorm);
			}
			
			return TRUE;
			break;
		}
	case WM_LBUTTONDOWN:
		{
			break;

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_WEB_ADS);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					urlArray[3]->cUrl,NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			else
			{
				SetCursor(hNorm);
			}
			
			return TRUE;
			break;
		}
		
		break;
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
		return (LRESULT)g_brushWebPage;
	case WM_CTLCOLORDLG:
		return (LRESULT)g_brushWebPage;

	case WM_DESTROY:
		DeleteObject(g_brushWebPage);
		DestroyWindow(g_TabWnd[3]);
		break;
	case WM_SIZING:
	case WM_SIZE:
		{
			hStatic = GetDlgItem(hwndDlg,IDC_STATIC_WEBPOS);
			/*hRefreshTray = GetDlgItem(hwndDlg,IDC_TAB_BTN_REFRESHLIST);*/

			RECT pageRect;
			GetWindowRect(hwndDlg,&pageRect);
			SetWindowPos(hStatic,NULL,10,0,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-9,SWP_NOZORDER);
			HWND hWeb;
			if(g_oleemb)
			SetBrowserRect(hStatic,&hWeb);
			hWeb=::FindWindowEx(hStatic,0,L"Shell Embedding",NULL);
			SetWindowPos(hWeb,NULL,0,0,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-9,SWP_NOZORDER);
			hWeb=::FindWindowEx(hWeb,0,L"Shell DocObject View",NULL);
			SetWindowPos(hWeb,NULL,0,0,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-9,SWP_NOZORDER);
			hWeb=::FindWindowEx(hWeb,0,L"Internet Explorer_Server",NULL);


			SetWindowPos(hWeb,NULL,0,0,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-9,SWP_NOZORDER);



// 			HWND ads_web = GetDlgItem(hwndDlg,IDC_WEB_ADS);
// 			SetWindowPos(ads_web,NULL,5,pageRect.bottom-pageRect.top-60,
// 				pageRect.right-pageRect.left - 20,60,SWP_NOZORDER);
		}

		break;

	case WM_NOTIFY:
		{

		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDCANCEL:
				break;
			default:
				return FALSE;
		}
	}


	return FALSE;
}






extern HWND hButton_ShowBK;
CHyperLink m_lnk_tray;
HWND hStatic_ADLink;



HWND hToolTip_tray;
TOOLINFO ti_tray;
HWND hList_Tray;

CPictureEx gif_tray;


INT_PTR CALLBACK TrayPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hTrayList,hRefreshTray;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		g_brushTrayPage = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
		hTrayListMenu = LoadMenu(hInst,MAKEINTRESOURCE(IDR_MENU_TABTRAYLIST));
		hTrayListMenu_Sub = GetSubMenu(hTrayListMenu,0);
		g_TrayList = GetDlgItem(hwndDlg,IDC_TAB_TRAYLIST);
		AddTrayListHeader();
		AllocResourceForTrayList();
		FillTrayList();
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_TAB_TRAYICON);
		}

		hStatic_ADLink = GetDlgItem(hwndDlg,IDC_TABPAGE_ADLINK);

#ifndef _NOADS
		m_lnk_tray.Attach(hStatic_ADLink);
// 		m_lnk_tray.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.store3000.com/hidedragon", TRUE);
		{
			wchar_t * cDefaultText = new wchar_t[1024];
			memset(cDefaultText,0,1024*sizeof(wchar_t));
			LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
			
			wchar_t * cDefaultLink = new wchar_t[1024];
			memset(cDefaultLink,0,1024*sizeof(wchar_t));
			
			LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.store3000.com/hidedragon");
			
			
			m_lnk_tray.SetLink(cDefaultText,cDefaultLink, TRUE);
			
			
			delete [] cDefaultText;
			cDefaultText = NULL;
			delete [] cDefaultLink;
			cDefaultLink = NULL;
		}

		{		
			HWND ads_tray = GetDlgItem(hwndDlg,IDC_TRAY_ADS);
			
			ShowWindow(ads_tray,FALSE);

#ifndef _NOADS

			gif_tray.Attach(ads_tray);

			wchar_t cFileName[1024] = L"";
			GetCurrentDirectory_HD(1024,cFileName);
			wcscat(cFileName,L"\\gif\\gif_tray_def.gif");
			gif_tray.Load(cFileName);
			gif_tray.Draw();
#endif
			PostMessage(hwndDlg,WM_SIZE,NULL,NULL);


		}




#endif

#ifdef _NOADS
		ShowWindow(hStatic_ADLink,SW_HIDE);
#endif


		if(g_UseIcon == 1)
		{
			HWND hHideThisIcon = GetDlgItem(hwndDlg,IDC_TRAYPAGE_HIDETRAY);
			HWND hRefresh = GetDlgItem(hwndDlg,IDC_TAB_BTN_REFRESHLIST);
			
			
			
			HICON hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_TRAY_HIDEICON),IMAGE_ICON,16,16,NULL);
			
			
			
			SendMessage(hHideThisIcon,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);
			
			hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_ALLWND_REFRESH),IMAGE_ICON,16,16,NULL);
			
	
			SendMessage(hRefresh,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);
			
			
			RECT rtM;
			rtM.left = 0;
			rtM.right = 20;
			rtM.top = 0;
			rtM.bottom = 00;
			Button_SetTextMargin(hHideThisIcon,&rtM);
			
			Button_SetTextMargin(hRefresh,&rtM);
		}



		{
			hToolTip_tray = CreateWindowExW(NULL,TOOLTIPS_CLASSW,NULL,
				WS_POPUP|TTS_ALWAYSTIP|TTS_BALLOON|TTS_NOPREFIX,
				CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwndDlg,NULL,hInst,NULL);
			hList_Tray = GetDlgItem(hwndDlg,IDC_TAB_TRAYLIST);
			ti_tray.cbSize     = sizeof (TOOLINFO);
			ti_tray.hinst      = hInst;
			ti_tray.uFlags     = TTF_IDISHWND;
			ti_tray.hwnd       = hwndDlg;
			ti_tray.uId        = (UINT_PTR)hList_Tray;
			ti_tray.lpszText   = LPSTR_TEXTCALLBACK;
			SendMessage(hToolTip_tray,TTM_ADDTOOL,0,(LPARAM)&ti_tray);
		}

		return FALSE;
	case WM_SETCURSOR:
		{
			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_TRAY_ADS);
			ShowWindow(hwndGif,FALSE);

#ifndef _NOADS
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				SetCursor(hHand);
			}
			else
			{
				SetCursor(hNorm);
			}
#endif		
			return TRUE;
			break;
		}
	case WM_LBUTTONDOWN:
		{
#ifndef _NOADS

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_TRAY_ADS);
			ShowWindow(hwndGif,FALSE);

			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",urlArray[1]->cUrl
					,NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			else
			{
				SetCursor(hNorm);
			}
#endif		

			
			return TRUE;
			break;
		}
		
		break;
	case WM_DESTROY:
		DestroyMenu(hTrayListMenu);
		DestroyWindow(g_TabWnd[1]);
		break;
//	case WM_CTLCOLORSTATIC:
//		SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
//		return (LRESULT)g_brushTrayPage;
//	case WM_CTLCOLORDLG:
//		return (LRESULT)g_brushTrayPage;
	case WM_SIZING:
	case WM_SIZE:
		{
			hTrayList = GetDlgItem(hwndDlg,IDC_TAB_TRAYLIST);
			hRefreshTray = GetDlgItem(hwndDlg,IDC_TAB_BTN_REFRESHLIST);

			HWND hBtn_BKIcon = GetDlgItem(hwndDlg,IDC_TRAYPAGE_HIDETRAY);
			hStatic_ADLink = GetDlgItem(hwndDlg,IDC_TABPAGE_ADLINK);



			RECT pageRect;
			GetWindowRect(hwndDlg,&pageRect);

#ifndef _NOADS

			SetWindowPos(hTrayList,NULL,10,120,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-150,SWP_NOZORDER);
#else

			SetWindowPos(hTrayList,NULL,10,50,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-80,SWP_NOZORDER);

#endif


#ifndef _NOADS

			SetWindowPos(hBtn_BKIcon,NULL,10,80,
			0,0,SWP_NOZORDER|SWP_NOSIZE);
#else
			SetWindowPos(hBtn_BKIcon,NULL,10,10,
				0,0,SWP_NOZORDER|SWP_NOSIZE);


#endif


			HWND ads_tray = GetDlgItem(hwndDlg,IDC_TRAY_ADS);
			ShowWindow(ads_tray,FALSE);

			SetWindowPos(ads_tray,NULL,10,5,
				pageRect.right-pageRect.left - 20,60,SWP_NOZORDER);
			

			RECT adlinkRect;
			GetWindowRect(hStatic_ADLink,&adlinkRect);
			RECT btnRect;
			GetWindowRect(hBtn_BKIcon,&btnRect);
			LONG btnLen = btnRect.right - btnRect.left;
			LONG adlinkLen = adlinkRect.right - adlinkRect.left;
			SetWindowPos(hStatic_ADLink,NULL,btnLen + 30,85,pageRect.right-pageRect.left - (btnLen + 50),
				20,SWP_NOZORDER);





			RECT refreshRect;
			GetWindowRect(hRefreshTray,&refreshRect);
			LONG refreshLen = refreshRect.right - refreshRect.left;
			SetWindowPos(hRefreshTray,NULL,pageRect.right-pageRect.left-refreshLen-9,
				pageRect.bottom-pageRect.top-25,0,0,SWP_NOZORDER|SWP_NOSIZE);
		}

		break;

	case WM_NOTIFY:
		{
			UINT idCtrl = 0;
			LPTOOLTIPTEXTW lpttt;
			wchar_t * pTipText2 = NULL;
			if(&lParam == NULL)
			{
				return FALSE;
			}
			switch((((LPNMHDR)lParam)->code))
			{
			case  TTN_GETDISPINFO:
				idCtrl = ::GetDlgCtrlID((HWND)((LPNMHDR)lParam)->idFrom);
				lpttt  = (LPTOOLTIPTEXTW)lParam;
				switch(idCtrl)
				{
				case IDC_TAB_TRAYLIST:
					if(sOP.bShowTip == 1)
					{
						if(g_iUseLanguage == 1)
						{
							pTipText2 = new wchar_t[1024];
							wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_TRAYLIST",pTipText2));
							delete [] pTipText2;
						}
						else
						{
							lpttt ->lpszText = L"Click mouse right button to access more functions.";
						}
					}
				}
				break;
				case NM_CLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem != -1 && pIA->hdr.idFrom == IDC_TAB_TRAYLIST)
					{
						if(sOP.bBeginnerModel == 0)
						{
							break;
						}
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						TrackPopupMenu(hTrayListMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);

						break;
					}
					else if(pIA->iItem < 0 && pIA->hdr.idFrom == IDC_TAB_TRAYLIST)
					{
						FillTrayList();
					}
				}

		 		break;
			case NM_RCLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem != -1 && pIA->hdr.idFrom == IDC_TAB_TRAYLIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						EnableMenuItem(hTrayListMenu_Sub,IDM_TAB_TRAY_LIST,MF_BYCOMMAND|
							MF_ENABLED);
						EnableMenuItem(hTrayListMenu_Sub,IDM_TAB_TRAY_ADDAUTOHIDE,MF_BYCOMMAND|
							MF_ENABLED);

						TrackPopupMenu(hTrayListMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);

						break;
					}
					else if(pIA->iItem < 0 && pIA->hdr.idFrom == IDC_TAB_TRAYLIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						EnableMenuItem(hTrayListMenu_Sub,IDM_TAB_TRAY_ADDAUTOHIDE,MF_BYCOMMAND|
							MF_GRAYED);				
						EnableMenuItem(hTrayListMenu_Sub,IDM_TAB_TRAY_LIST,MF_BYCOMMAND|
							MF_GRAYED);

						TrackPopupMenu(hTrayListMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
						break;
					}
				}
				break;
			default:
				break;
			}

		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDCANCEL:

				break;

			case IDC_TAB_BTN_REFRESHLIST:
				FillTrayList();


				break;
			case IDC_TRAYPAGE_HIDETRAY:
			case IDM_TAB_TRAY_LIST:
				BKTrayIcon();
				break;
			case IDM_TAB_TRAY_ADDAUTOHIDE:
				AddTrayAutoHide();

				break;


			default:
				return FALSE;
		}
	}


	return FALSE;
}



HIMAGELIST hImage_TrayList;


int	AddTrayListHeader()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512],pProcessID[100];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Tray Icon";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pTrayIcon,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_LISTVIEWBK_TRAYICON",pHeaderText,L"Tray Icon"));
		lvColumn.pszText = pTrayIcon;
		delete [] pHeaderText;
	}
	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	lvColumn.cx    = 360;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Window Title or Tray Icon's Tooltip";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pRelatedTitle,0,512 * sizeof(wchar_t));
		wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_LISTVIEWBK_RELATEDTITLE",pHeaderText,
			L"Window Title or Tray Icon's Tooltip"));
		lvColumn.pszText = pRelatedTitle;
		delete [] pHeaderText;		
	}
	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Process ID";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pProcessID,0,100 * sizeof(wchar_t));
		wcscpy(pProcessID,LoadItemStringW(L"IDS_LISTVIEWBK_PROCESSID",pHeaderText,
			L"Process ID"));
		lvColumn.pszText = pProcessID;
		delete [] pHeaderText;		
	}
	SendMessageW(g_TrayList,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(g_TrayList);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(g_TrayList,0,lvStyle);

	hImage_TrayList = ImageList_Create(16,16,ILC_COLOR24,20,10);
	ImageList_AddIcon(hImage_TrayList,hIcon_NoIcon);
	ListView_SetImageList(g_TrayList,hImage_TrayList,LVSIL_SMALL);
//	_CrtDumpMemoryLeaks();	



	return 1;
}








int	FillTrayList()
{
	if(IsWindow(g_TrayList) == FALSE)
	{
		return 0;
	}
	ListView_DeleteAllItems(g_TrayList);



	ImageList_RemoveAll(hImage_TrayList);

	EnumAllVisibleTray();


	LVITEM bkItem;
	int i = 0,iIconIndex = 0;
	wchar_t cPID[20];
	
	while(g_QuickHideAllTray[i].lPID != 0 && i < 100)
	{
		bkItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM;
		bkItem.lParam = (LPARAM)(g_QuickHideAllTray + i);

		bkItem.iSubItem = 0;
		if(g_QuickHideAllTray[i].trayicon.hIcon == NULL)
		{
			iIconIndex = 0;
		}
		else
		{
			iIconIndex = ImageList_AddIcon(hImage_TrayList,g_QuickHideAllTray[i].trayicon.hIcon);
		}
		bkItem.iImage = iIconIndex;
		if(g_QuickHideAllTray[i].trayicon.hIcon != NULL)
		{
			bkItem.pszText = L"";
		}
		else
		{
			bkItem.pszText = L"";
		}
		bkItem.iItem = ListView_GetItemCount(g_TrayList);
		ListView_InsertItem(g_TrayList,&bkItem);

		bkItem.mask = LVIF_TEXT;
		bkItem.iSubItem = 1;
//		if(IsWindow(pWH[i]->hWnd))
//		{
//			bkItem.pszText=pWH[i]->cWndTitle;
//		}
//		else
		{
			bkItem.pszText= g_QuickHideAllTray[i].cIconTip;
		}
		ListView_SetItem(g_TrayList,&bkItem);

		bkItem.iSubItem = 2;
		swprintf(cPID,L"%X",g_QuickHideAllTray[i].lPID);
		bkItem.pszText=cPID;
		ListView_SetItem(g_TrayList,&bkItem);
		
		i ++;

	}


	return 1;
}




int	FillAutoHideList()
{
	if(IsWindow(g_AutoHideList) == FALSE)
	{
		return 0;
	}
	ListView_DeleteAllItems(g_AutoHideList);
	
	int iExist = 0;
	AutoHideItem * pItem = new AutoHideItem;
	pItem->cProcessName = new wchar_t[1024];
	memset(pItem->cProcessName,0,1024);
	
	DBClass db;
	db.ReadAHItem(pItem,&iExist,Insert2AutoHideList);


	delete [] pItem->cProcessName;
	delete [] pItem;
	

	
	return 1;
}





int AddTrayAutoHide()
{
	int iSelIndex = ListView_GetSelectionMark(g_TrayList);
	if(iSelIndex < 0)
	{
		HDMessageBoxW(L"IDS_TRAYLIST_PLEASESELITEM",L"PLEASESELITEM");
		return FALSE;
	}
	
	wchar_t cPID[20],cCurrentPID[20];
	memset(cPID,0,20 * sizeof(wchar_t));


	ListView_GetItemText(g_TrayList,iSelIndex,2,cPID,20);
	



	int i = 0;
	while(g_QuickHideAllTray[i].lPID != 0 && i < 100)
	{
		memset(cCurrentPID,0,20 * sizeof(wchar_t));
		swprintf(cCurrentPID,L"%X",g_QuickHideAllTray[i].lPID);
		if(wcscmp(cCurrentPID,cPID) == 0 &&
			wcscmp(wcslwr(g_QuickHideAllTray[i].cProcessName),L"explorer.exe"))
		{
			DBClass db;
			AutoHideItem * pItem = new AutoHideItem;
			pItem->cProcessName = new wchar_t[1024];
			memset(pItem->cProcessName,0,1024);
			GetProcessName(g_QuickHideAllTray[i].lPID,pItem->cProcessName);
			pItem->iType = 2;//tray
			int iRet = db.AddAutoHideItem(pItem);
			if(iRet == 1)
			{
				Insert2AutoHideList(pItem);//For listview
				AddDBPorcessList(pItem);//For list data 
// 				if(g_AutoHideHandle ==  NULL)
// 				{
// 					CreateAutoHideThread();
// 				}
// 				if(IsDlgButtonChecked(g_TabWnd[7],IDC_AUTOHIDE_ENABLEAH) == FALSE)
// 				{
// 					CheckDlgButton(g_TabWnd[7],IDC_AUTOHIDE_ENABLEAH,BST_CHECKED);
// 				}
			}
			

			
			delete [] pItem->cProcessName;
			delete pItem;
		}
		i ++;
	 }
	

	return 1;
}





int BKTrayIcon()
{
	WndAndHandle * pTemp = NULL;

	LVITEM bkItem;
	memset(&bkItem,0,sizeof(bkItem));

	int iSelIndex = ListView_GetSelectionMark(g_TrayList);
	if(iSelIndex < 0)
	{
		HDMessageBoxW(L"IDS_TRAYLIST_PLEASESELITEM",L"PLEASESELITEM");
		return FALSE;
	}
	bkItem.iItem = iSelIndex;
	bkItem.mask = LVIF_PARAM;
	ListView_GetItem(g_TrayList,&bkItem);
	pTemp = (WndAndHandle*)bkItem.lParam;
	pTemp->cWndTitle = NULL;
	HWND hWnd = (HWND)1;
	pTemp->hWnd = hWnd;
	HideSelected(&pTemp,1,0);
	ListView_DeleteItem(g_TrayList,iSelIndex);



	return 1;
}



int CleanHistory()
{
	SendMessageW(g_TabWnd[5],WM_COMMAND,IDC_CLEAN_BK,0);



	return 1;
}




int	AddProcessListColumn()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 160;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Image Name";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pTrayIcon,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_PROCESSLISTVIEW_IMAGENAME",
			pHeaderText,L"Image Name"));
		lvColumn.pszText = pTrayIcon;
		delete [] pHeaderText;
	}
	SendMessageW(g_ProcessList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	lvColumn.cx    = 360;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"PID";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pRelatedTitle,0,512 * sizeof(wchar_t));
		wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_PROCESSLISTVIEW_PID",pHeaderText,
			L"PID"));
		lvColumn.pszText = pRelatedTitle;
		delete [] pHeaderText;		
	}
	SendMessageW(g_ProcessList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);

	
	
	
	
	
	lvStyle = ListView_GetExtendedListViewStyle(g_ProcessList);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(g_ProcessList,0,lvStyle);

//	hImage_TrayList = ImageList_Create(16,16,ILC_COLOR24,20,10);
//	ImageList_AddIcon(g_ProcessList,hIcon_NoIcon);
//	ListView_SetImageList(g_ProcessList,hImage_TrayList,LVSIL_SMALL);
//	_CrtDumpMemoryLeaks();	

	



	return 1;
}


WndAndHandle * g_ProcessParam = NULL;


int AllocMemForProcessList()
{
	int i = 0;
	g_ProcessParam = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,
		MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(g_ProcessParam,0,sizeof(WndAndHandle)*100);
	for(i = 0;i<100;i++)
	{
		g_ProcessParam[i].cWndTitle = NULL;//(wchar_t *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		g_ProcessParam[i].cIconTip = NULL;//(wchar_t*)new wchar_t [1024];
		//memset(g_ProcessParam[i].cIconTip,0,1024 * sizeof(wchar_t));
//		memset(g_QuickHideAllTray[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(g_ProcessParam[i].cProcessName,0,30 * sizeof(wchar_t));
		g_ProcessParam[i].Next = NULL;
	}




	return 1;
}



int	FillProcessList()
{
	PROCESSENTRY32 pe32;
	LVITEM bkItem;
	wchar_t cPID[10] = L"";
	wchar_t pName[256] = L"";
	int i = 0;





	ListView_DeleteAllItems(g_ProcessList);


	

	
	HANDLE hProcessSnap = NULL;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First((void*)hProcessSnap,&pe32);

	while(Process32Next((void*)hProcessSnap,&pe32))
	{
		memset(&bkItem,0,sizeof(LVITEM));
		//wcscpy(pName,pe32.szExeFile);

		wcscpy(g_ProcessParam[i].cProcessName,pe32.szExeFile);
		g_ProcessParam[i].lPID = pe32.th32ProcessID;
		
		bkItem.mask = LVIF_TEXT|LVIF_PARAM;
		bkItem.lParam = LPARAM(g_ProcessParam + i);
	
		bkItem.iSubItem = 0;
		bkItem.iImage = 0;
		memset(pName,0,256*sizeof(wchar_t));
		bkItem.pszText = pName;
		wcscpy(bkItem.pszText,pe32.szExeFile);
		bkItem.iItem = ListView_GetItemCount(g_ProcessList);
		ListView_InsertItem(g_ProcessList,&bkItem);

		bkItem.mask = LVIF_TEXT;


		bkItem.iSubItem = 1;
		memset(cPID,0,10*sizeof(wchar_t));
		swprintf(cPID,L"%X",pe32.th32ProcessID);
		bkItem.pszText=cPID;
		ListView_SetItem(g_ProcessList,&bkItem);

		
		i ++;
	}
	CloseHandle((HANDLE)hProcessSnap);
	
	


	return 1;
}











HBRUSH g_brushProcessPage;
CHyperLink m_lnk_process;
HWND hProcess_Adlink;



INT_PTR CALLBACK ProcessPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hProcessList,hBtn_Refresh;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		g_brushProcessPage = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_TAB_PROCESSLIST);
		}
		hProcessList = GetDlgItem(hwndDlg,IDC_TAB_PROCESSLIST);
		g_ProcessList = hProcessList;

		AddProcessListColumn();
		
		AllocMemForProcessList();
		FillProcessList();

		
		hProcessListMenu = LoadMenu(hInst,
			MAKEINTRESOURCE(IDR_MENU_PROCESSLIST));
		hProcessMenu_Sub = GetSubMenu(hProcessListMenu,0);
		
		hProcess_Adlink = GetDlgItem(hwndDlg,IDC_PROCESSPAGE_ADLINK);
		m_lnk_process.Attach(hProcess_Adlink);
// 		m_lnk_process.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.store3000.com/hidedragon", TRUE);
		{
			wchar_t * cDefaultText = new wchar_t[1024];
			memset(cDefaultText,0,1024*sizeof(wchar_t));
			LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
			
			wchar_t * cDefaultLink = new wchar_t[1024];
			memset(cDefaultLink,0,1024*sizeof(wchar_t));
			
			LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.store3000.com/hidedragon");
			
			
			m_lnk_process.SetLink(cDefaultText,cDefaultLink, TRUE);
			
			
			delete [] cDefaultText;
			cDefaultText = NULL;
			delete [] cDefaultLink;
			cDefaultLink = NULL;
		}

		return FALSE;
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
		return (LRESULT)g_brushProcessPage;
	case WM_CTLCOLORDLG:
		return (LRESULT)g_brushProcessPage;
	case WM_SIZING:
	case WM_SIZE:
		{
			hProcessList = GetDlgItem(hwndDlg,IDC_TAB_PROCESSLIST);
			hBtn_Refresh = GetDlgItem(hwndDlg,IDC_PROCESSLIST_REFRESH);
//			hCK_HidePop = GetDlgItem(hwndDlg,IDC_TAB_CHECK_AUTOHIDE);


			HWND hBtn_HidePro = GetDlgItem(hwndDlg,IDC_PROCESSPAGE_HIDE);
			hProcess_Adlink = GetDlgItem(hwndDlg,IDC_PROCESSPAGE_ADLINK);
			RECT pageRect;
			GetWindowRect(hwndDlg,&pageRect);
			SetWindowPos(hProcessList,NULL,10,50,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-79,SWP_NOZORDER);

			RECT showRect;
			GetWindowRect(hBtn_Refresh,&showRect);
			LONG showLen = showRect.right - showRect.left;
			SetWindowPos(hBtn_Refresh,NULL,pageRect.right-pageRect.left-showLen-9,
				pageRect.bottom-pageRect.top-25,0,0,SWP_NOZORDER|SWP_NOSIZE);

			
			SetWindowPos(hBtn_HidePro,NULL,10,10,
			0,0,SWP_NOZORDER|SWP_NOSIZE);

			
			RECT adlinkRect;
			GetWindowRect(hProcess_Adlink,&adlinkRect);
			LONG adlinkLen = adlinkRect.right - adlinkRect.left;
			SetWindowPos(hProcess_Adlink,NULL,pageRect.right-pageRect.left-adlinkLen-9,30,
			0,0,SWP_NOZORDER|SWP_NOSIZE);
			
			
			
			
			
			
			//			
//			SetWindowPos(hCK_HidePop,NULL,10,
//				pageRect.bottom-pageRect.top-23,0,0,SWP_NOZORDER|SWP_NOSIZE);

		}
		break;
	case WM_NOTIFY:
		{
			if(&lParam == NULL)
			{
				return FALSE;
			}
			switch((((LPNMHDR)lParam)->code))
			{
			case NM_CLICK:
				if(sOP.bBeginnerModel == 0)
				{
					break;
				}
			case NM_RCLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem >= 0 && pIA->hdr.idFrom == IDC_TAB_PROCESSLIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						TrackPopupMenu(hProcessMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);



						break;
					}
				}
				break;

			default:
				break;
			}
		}
		break;
	case WM_DESTROY:
		if(g_ProcessParam != NULL)
		{
			VirtualFree(g_ProcessParam,0,MEM_RELEASE);
			g_ProcessParam = NULL;
		}
		DestroyMenu(hProcessListMenu);
		DeleteObject(g_brushProcessPage);
		DestroyWindow(g_TabWnd[4]);

		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_PROCESSPAGE_HIDE:
			{

			}

		case IDM_PROCESSLIST_HIDE:
			{
				LVITEM ProItem;
				WndAndHandle* pItemData = NULL;
				memset(&ProItem,0,sizeof(ProItem));


				int iSelIndex = ListView_GetSelectionMark(g_ProcessList);
				if(iSelIndex < 0)
				{
					HDMessageBoxW(L"IDS_HIDEPROCESS_PLEASESELITEM",L"PLEASESELITEM");
					return FALSE;
				}
				ProItem.iItem = iSelIndex;
				ProItem.mask = LVIF_PARAM;
				ListView_GetItem(g_ProcessList,&ProItem);
				pItemData = (WndAndHandle*)ProItem.lParam;

				
				
				HideSelected(&pItemData,1,0);
	
	
	
	
	
				//HideProcess(pItemData->lPID);
				
				ListView_DeleteItem(g_ProcessList,iSelIndex);
	
	
			}
			break;
		case IDM_PROCESSLIST_END:
			{
				LVITEM ProItem;
				WndAndHandle* pItemData = NULL;
				memset(&ProItem,0,sizeof(ProItem));


				int iSelIndex = ListView_GetSelectionMark(g_ProcessList);
				if(iSelIndex < 0)
				{
					HDMessageBoxW(L"IDS_HIDEPROCESS_PLEASESELITEM",L"PLEASESELITEM");
					return FALSE;
				}
				ProItem.iItem = iSelIndex;
				ProItem.mask = LVIF_PARAM;
				ListView_GetItem(g_ProcessList,&ProItem);
				pItemData = (WndAndHandle*)ProItem.lParam;

				
				
				
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,NULL,pItemData->lPID);
				TerminateProcess(hProcess,0);
	
	
	
	
	
				//HideProcess(pItemData->lPID);
				
				ListView_DeleteItem(g_ProcessList,iSelIndex);
	
	
			}


			break;
		case IDC_PROCESSLIST_REFRESH:
			FillProcessList();

			break;
			default:
				return FALSE;
		}
	}


	return FALSE;
}






INT_PTR CALLBACK HideIPPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_TAB_CLEANPRIV);
		}
		{
		}

		return FALSE;
	case WM_SIZING:
	case WM_SIZE:
		{
//			hProcessList = GetDlgItem(hwndDlg,IDC_TAB_PROCESSLIST);
//			hBtn_Refresh = GetDlgItem(hwndDlg,IDC_PROCESSLIST_REFRESH);
////			hCK_HidePop = GetDlgItem(hwndDlg,IDC_TAB_CHECK_AUTOHIDE);



//			RECT pageRect;
//			GetWindowRect(hwndDlg,&pageRect);
//			SetWindowPos(hProcessList,NULL,10,0,
//				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-29,SWP_NOZORDER);
//
//			RECT showRect;
//			GetWindowRect(hBtn_Refresh,&showRect);
//			LONG showLen = showRect.right - showRect.left;
//			SetWindowPos(hBtn_Refresh,NULL,pageRect.right-pageRect.left-showLen-9,
//				pageRect.bottom-pageRect.top-25,0,0,SWP_NOZORDER|SWP_NOSIZE);
//			
//			SetWindowPos(hCK_HidePop,NULL,10,
//				pageRect.bottom-pageRect.top-23,0,0,SWP_NOZORDER|SWP_NOSIZE);

		}
		break;
	case WM_NOTIFY:
		{
			if(&lParam == NULL)
			{
				return FALSE;
			}
		}
		break;
	case WM_DESTROY:
		break;
	case WM_COMMAND:
//		switch(LOWORD(wParam))
//		{
//			
//
//			return FALSE;
//		}
		break;
	}


	return FALSE;
}




int	AddAutoHideListHeader()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512]/*,pProcessID[100]*/;
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"ID";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pTrayIcon,0,100 * sizeof(wchar_t));
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_LISTVIEWAH_ID",pHeaderText,L"ID"));
		lvColumn.pszText = pTrayIcon;
		delete [] pHeaderText;
	}
	SendMessageW(g_AutoHideList,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	lvColumn.cx    = 360;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Process Name";
	}
	else
	{
		wchar_t * pHeaderText = new wchar_t[1024];
		memset(pRelatedTitle,0,512 * sizeof(wchar_t));
		wcscpy(pRelatedTitle,LoadItemStringW(L"IDS_LISTVIEWAH_PROCESSNAME",pHeaderText,
			L"Process Name"));
		lvColumn.pszText = pRelatedTitle;
		delete [] pHeaderText;		
	}
	SendMessageW(g_AutoHideList,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
//	lvColumn.cx   = 80;
//	if(g_iUseLanguage == 0)
//	{
//		lvColumn.pszText = L"Type";
//	}
//	else
//	{
//		wchar_t * pHeaderText = new wchar_t[1024];
//		memset(pProcessID,0,100 * sizeof(wchar_t));
//		wcscpy(pProcessID,LoadItemStringW(L"IDS_LISTVIEWAH_TYPE",pHeaderText,
//			L"Type"));
//		lvColumn.pszText = pProcessID;
//		delete [] pHeaderText;
//	}
//	SendMessageW(g_AutoHideList,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(g_AutoHideList);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(g_AutoHideList,0,lvStyle);
	


	
	
	
	return 1;
}


CHyperLink m_lnk_autohide;
HWND hAutohide_Adlink;


CPictureEx gif_ah;



INT_PTR CALLBACK AutoHideProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hAutoHideList,hAutoHide_Add,hAutoHide_Ads,hAutoHide_Tip,hAutoHide_Del,hChk_EnableAH;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_TAB_AUTOHIDE);

			g_AutoHideList = GetDlgItem(hwndDlg,IDC_AUTOHIDE_LIST);
			AddAutoHideListHeader();

			FillAutoHideList();


			hAutoHideListMenu = LoadMenu(hInst,MAKEINTRESOURCE(IDR_MENU_AUTOHIDE));
			hAutoHideListMenu_Sub = GetSubMenu(hAutoHideListMenu,0);

			if(sOP.bAutoHideProgram == 1)
			{
				CheckDlgButton(hwndDlg,IDC_AUTOHIDE_ENABLEAH,BST_CHECKED);
			}

			hAutoHide_Tip = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADLINK);

#ifdef _NOADS
			hAutoHide_Ads = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADLINK);
			ShowWindow(hAutoHide_Ads,SW_HIDE);

#endif

#ifndef _NOADS
			m_lnk_autohide.Attach(hAutoHide_Tip);
// 			m_lnk_autohide.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 				L"http://www.store3000.com/hidedragon", TRUE);

			{
				wchar_t * cDefaultText = new wchar_t[1024];
				memset(cDefaultText,0,1024*sizeof(wchar_t));
				LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
				
				wchar_t * cDefaultLink = new wchar_t[1024];
				memset(cDefaultLink,0,1024*sizeof(wchar_t));
				
				LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.store3000.com/hidedragon");
				
				
				m_lnk_autohide.SetLink(cDefaultText,cDefaultLink, TRUE);
				
				
				delete [] cDefaultText;
				cDefaultText = NULL;
				delete [] cDefaultLink;
				cDefaultLink = NULL;
			}
			{		
				HWND ads_ah = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADS);
#ifdef _NOADS				
				ShowWindow(hAutoHide_Tip,SW_HIDE);
#endif


#ifndef _NOADS	
				gif_ah.Attach(ads_ah);
				wchar_t cFileName[1024] = L"";
				GetCurrentDirectory_HD(1024,cFileName);
				wcscat(cFileName,L"\\gif\\gif_ah_def.gif");
				gif_ah.Load(cFileName);
				gif_ah.Draw();
#endif
				PostMessage(hwndDlg,WM_SIZE,NULL,NULL);
			}

#endif
			
#ifdef _NOADS
	ShowWindow(hAutoHide_Tip,SW_HIDE);
#endif

		}
		
		return FALSE;
	case WM_SETCURSOR:
		{
#ifndef _NOADS	

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADS);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				SetCursor(hHand);
			}
			else
			{
				SetCursor(hNorm);
			}
#endif
			return TRUE;
			break;
		}
	case WM_LBUTTONDOWN:
		{
#ifndef _NOADS	

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADS);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					urlArray[7]->cUrl,NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			else
			{
				SetCursor(hNorm);
			}
#endif
			
			return TRUE;
			break;
		}
		
		break;
	case WM_SIZING:
	case WM_SIZE:
		{
			hAutoHideList = GetDlgItem(hwndDlg,IDC_AUTOHIDE_LIST);
			hAutoHide_Add = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADD);
			hAutoHide_Del = GetDlgItem(hwndDlg,IDC_AUTOHIDE_DEL);
			hAutoHide_Tip = GetDlgItem(hwndDlg,IDC_AUTOHIDE_TIP);
			hChk_EnableAH = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ENABLEAH);
			
			
			hAutoHide_Ads = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADLINK);
			ShowWindow(hAutoHide_Ads,SW_HIDE);

			


			RECT pageRect;
			GetWindowRect(hwndDlg,&pageRect);

#ifndef _NOADS	

			SetWindowPos(hAutoHideList,NULL,10,120,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-189,SWP_NOZORDER);

#else


			SetWindowPos(hAutoHideList,NULL,10,50,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-90,SWP_NOZORDER);



#endif
			



			RECT addRect;
			GetWindowRect(hAutoHide_Add,&addRect);
			LONG addLen = addRect.right - addRect.left;

#ifndef _NOADS	

			SetWindowPos(hAutoHide_Del,NULL,pageRect.right-pageRect.left-addLen-9,
				pageRect.bottom-pageRect.top-60,0,0,SWP_NOZORDER|SWP_NOSIZE);
#else

			SetWindowPos(hAutoHide_Del,NULL,pageRect.right-pageRect.left-addLen-9,
				pageRect.bottom-pageRect.top-30,0,0,SWP_NOZORDER|SWP_NOSIZE);

#endif


			RECT delRect;
			GetWindowRect(hAutoHide_Del,&delRect);
			LONG delLen = delRect.right - delRect.left;

#ifndef _NOADS	


			SetWindowPos(hAutoHide_Add,NULL,pageRect.right-pageRect.left-delLen*2-11,
				pageRect.bottom-pageRect.top-60,0,0,SWP_NOZORDER|SWP_NOSIZE);

#else




			SetWindowPos(hAutoHide_Add,NULL,pageRect.right-pageRect.left-delLen*2-11,
				pageRect.bottom-pageRect.top-30,0,0,SWP_NOZORDER|SWP_NOSIZE);

#endif







#ifndef _NOADS	

			SetWindowPos(hChk_EnableAH,NULL,10,
				pageRect.bottom-pageRect.top-55,0,0,SWP_NOZORDER|SWP_NOSIZE);

#else


			SetWindowPos(hChk_EnableAH,NULL,10,
				pageRect.bottom-pageRect.top-25,0,0,SWP_NOZORDER|SWP_NOSIZE);


#endif



#ifndef _NOADS	

			SetWindowPos(hAutoHide_Tip,NULL,10,75,
				pageRect.right-pageRect.left - 20,30,SWP_NOZORDER);
#else

			SetWindowPos(hAutoHide_Tip,NULL,10,5,
				pageRect.right-pageRect.left - 20,30,SWP_NOZORDER);

#endif

			RECT adlinkRect;
			GetWindowRect(hAutoHide_Ads,&adlinkRect);
			RECT btnRect;
			GetWindowRect(hAutoHide_Add,&btnRect);
			LONG btnLen = btnRect.right - btnRect.left;
			LONG adlinkLen = adlinkRect.right - adlinkRect.left;
			SetWindowPos(hAutoHide_Ads,NULL,pageRect.right-pageRect.left-delLen*2-92,
				pageRect.bottom-pageRect.top-20,pageRect.right-pageRect.left - (btnLen + 50),
				20,SWP_NOZORDER);


			
			HWND ads_ah = GetDlgItem(hwndDlg,IDC_AUTOHIDE_ADS);
			SetWindowPos(ads_ah,NULL,10,5,
				pageRect.right-pageRect.left - 20,60,SWP_NOZORDER);
		}
		break;

	case WM_NOTIFY:
		{
			if(&lParam == NULL)
			{
				return FALSE;
			}
			switch((((LPNMHDR)lParam)->code))
			{
				case NM_RCLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem != -1 && pIA->hdr.idFrom == IDC_AUTOHIDE_LIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						EnableMenuItem(hAutoHideListMenu_Sub,IDM_AUTOHIDE_DEL,MF_BYCOMMAND|
							MF_ENABLED);
						TrackPopupMenu(hAutoHideListMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
						
						break;
					}
					else if(pIA->iItem < 0 && pIA->hdr.idFrom == IDC_AUTOHIDE_LIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						EnableMenuItem(hAutoHideListMenu_Sub,IDM_AUTOHIDE_DEL,MF_BYCOMMAND|
							MF_GRAYED);
						TrackPopupMenu(hAutoHideListMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
						
						
					}
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		DestroyMenu(hAutoHideListMenu);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_AUTOHIDE_ENABLEAH:
			if(IsDlgButtonChecked(hwndDlg,IDC_AUTOHIDE_ENABLEAH) == BST_CHECKED)
			{
				sOP.bAutoHideProgram = 1;
				if(g_AutoHideHandle ==  NULL)
				{
					CreateAutoHideThread();
				}
			}
			else
			{
				sOP.bAutoHideProgram = 0;
			}
			WriteOptionFile(sOP);
			
			break;
		case IDC_AUTOHIDE_DEL:
		case IDM_AUTOHIDE_DEL:
			{
				DelAutoHideItem();
			}
			break;
		case IDC_AUTOHIDE_ADD:
		case IDM_AUTOHIDE_ADD:
			AddAutoHideFromFile();
			//MessageBox(NULL,L"ADD",L"DDD",MB_OK);
			break;

			return FALSE;
		}
		break;
	}
	
	
	return FALSE;
}

unsigned __stdcall OpenFileThreadProc(void * pParam)
{
	OPENFILENAME OFN;
	wchar_t * szFile = NULL;
	wchar_t * pFileType = NULL, * pDialogTitle = NULL, * pT1 = NULL,
		* pT2 = NULL, * pT3 = NULL, * pT4 = NULL;
	
	wchar_t * cCurDir = NULL;




	szFile = new wchar_t [MAX_PATH];
	memset(szFile,0,MAX_PATH * sizeof(wchar_t));
	memset(&OFN,0,sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrDefExt = L".EXE";
	OFN.hwndOwner = hMainDialog;
	OFN.hInstance = NULL;
	if(g_iUseLanguage == 0)
	{
		OFN.lpstrFilter = L"Executable Files (*.EXE)\0*.EXE\0All Files (*.*)\0*.*\0";
	}
	else
	{
		pFileType = new wchar_t[1024];
		memset(pFileType,0,1024 * sizeof(wchar_t));
/*		pFileType = new wchar_t[1024];*///上面NEW了一次
		pT1 = new wchar_t[1024];
		pT2 = new wchar_t[1024];
		pT3 = new wchar_t[1024];
		pT4 = new wchar_t[1024];
		
		
		
		
		memset(pFileType,0,1024 * sizeof(wchar_t));
		wcscpy(pT1,LoadItemStringW(L"IDS_QUICKHIDE_FILETYPE1",pFileType,L"Executable Files (*.EXE)"));
		wcscpy(pT2 , LoadItemStringW(L"IDS_QUICKHIDE_FILETYPE2",pFileType,L"*.EXE"));
		wcscpy(pT3 , LoadItemStringW(L"IDS_QUICKHIDE_FILETYPE3",pFileType,L"All Files (*.*)"));
		wcscpy(pT4 , LoadItemStringW(L"IDS_QUICKHIDE_FILETYPE4",pFileType,L"*.*"));
		
		wchar_t * pHead = pFileType;
		memset(pFileType,0,1024 * sizeof(wchar_t));
		memcpy(pFileType ,pT1,wcslen(pT1) * 2);
		pFileType += wcslen(pT1) + 1;
		memcpy(pFileType,pT2,wcslen(pT2) * 2);
		pFileType += wcslen(pT2) +1 ;
		memcpy(pFileType,pT3,wcslen(pT3) * 2);
		pFileType += wcslen(pT3) + 1;
		memcpy(pFileType,pT4,wcslen(pT4) * 2);
		pFileType = pHead;
		
		
		OFN.lpstrFilter = pFileType;
	}
	OFN.lpstrCustomFilter = NULL;
	OFN.nMaxCustFilter = 0;
	OFN.nFilterIndex = 1;
	OFN.lpstrFile = szFile;
	OFN.nMaxFile = 256;
	if(g_iUseLanguage == 0)
	{
		OFN.lpstrTitle = L"Please Select AutoHide Executable File";
	}
	else
	{
		pDialogTitle = new wchar_t[1024];
		memset(pDialogTitle,0,1024 * sizeof(wchar_t));
		OFN.lpstrTitle = LoadItemStringW(L"IDS_AUTOHIDE_DAILOGTITLE",
			pDialogTitle,L"Please Select Auto Hide Executable File");
	}
	OFN.Flags = OFN_CREATEPROMPT|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	
	if(GetOpenFileName(&OFN))
	{
// 		SetDlgItemText(hwndDlg,IDC_EDIT_PPATH,szFile);
// 		PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
		
		

		DBClass db;
		AutoHideItem * pItem = new AutoHideItem;
		pItem->cProcessName = new wchar_t[1024];
		memset(pItem->cProcessName,0,1024 * sizeof(wchar_t));
		//GetProcessName(g_QuickHideAllTray[i].lPID,pItem->cProcessName);
		//PathRemoveFileSpec(szFile);
		int ch = '\\';
		wchar_t * pBegin = wcsrchr(szFile,ch);

		wcscpy(pItem->cProcessName,pBegin+1);
		//wcscpy(pItem->cProcessName,szFile);

		
		pItem->iType = 1;//window
		int iRet = db.AddAutoHideItem(pItem);
		if(iRet == 1)
		{
			Insert2AutoHideList(pItem);//For listview
			AddDBPorcessList(pItem);//For list data 
			// 				if(g_AutoHideHandle ==  NULL)
			// 				{
			// 					CreateAutoHideThread();
			// 				}
			// 				if(IsDlgButtonChecked(g_TabWnd[7],IDC_AUTOHIDE_ENABLEAH) == FALSE)
			// 				{
			// 					CheckDlgButton(g_TabWnd[7],IDC_AUTOHIDE_ENABLEAH,BST_CHECKED);
			// 				}
		}	
			
		delete [] pItem->cProcessName;
		pItem->cProcessName = NULL;
		delete pItem;
		pItem = NULL;

	}
	if(g_iUseLanguage && pFileType)
	{
		delete [] pFileType;
		delete [] pT1;
		delete [] pT2;
		delete [] pT3;
		delete [] pT4;
	}
	if(g_iUseLanguage && pDialogTitle)
		delete [] pDialogTitle;
	memset(szFile,0,MAX_PATH * sizeof(wchar_t));
	delete [] szFile;
	///////////////////////////////////////////////////////////////
	cCurDir = new wchar_t[1024];
	ZeroMemory(cCurDir,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cCurDir,1024);
	PathRemoveFileSpec(cCurDir);
	SetCurrentDirectory(cCurDir);
	delete [] cCurDir;

	CloseHandle(g_hOpenFileHandle);
	g_hOpenFileHandle = NULL;



	return 1;

	
}



int AddAutoHideFromFile()
{
	if(g_hOpenFileHandle == NULL)
	{
		g_hOpenFileHandle = (HANDLE)_beginthreadex(NULL,0,OpenFileThreadProc,NULL,0,NULL);
		if(g_hOpenFileHandle == INVALID_HANDLE_VALUE)
		{
			g_hOpenFileHandle = NULL;
		}
	}



	return 1;
}


int DelAutoHideItem()
{
	WndAndHandle * pTemp = NULL;

	LVITEM bkItem;
	memset(&bkItem,0,sizeof(bkItem));

	int iSelIndex = ListView_GetSelectionMark(g_AutoHideList);
	if(iSelIndex < 0)
	{
		HDMessageBoxW(L"IDS_AUTOHIDE_PLEASESELITEM",L"PLEASESELITEM");
		return FALSE;
	}
	bkItem.iItem = iSelIndex;
	bkItem.mask = LVIF_PARAM;
	ListView_GetItem(g_AutoHideList,&bkItem);
// 	pTemp = (WndAndHandle*)bkItem.lParam;
// 	pTemp->cWndTitle = NULL;
// 	HWND hWnd = (HWND)1;
// 	pTemp->hWnd = hWnd;
	


	AutoHideItem * pAutoItem = new AutoHideItem;
	memset(pAutoItem,0,sizeof(AutoHideItem));

	pAutoItem->cProcessName = new wchar_t[1024];
	memset(pAutoItem->cProcessName,0,1024*sizeof(wchar_t));
	ListView_GetItemText(g_AutoHideList,iSelIndex,1,pAutoItem->cProcessName,1000);

	//wcscpy(pAutoItem->cProcessName,pTemp->cProcessName);
	//pAutoItem->iType = 
	DBClass db;
	
	db.DelAutoHideItem(pAutoItem);
	DelDBProcessList(pAutoItem);

	delete [] pAutoItem->cProcessName;
	delete [] pAutoItem;



	ListView_DeleteItem(g_AutoHideList,iSelIndex);

	FillAutoHideList();

	return 1;
}





CHyperLink m_lnk_bk;
HWND hBK_ADLink;

CPictureEx gif_bk;









INT_PTR CALLBACK BKPageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hBKList,hBtn_Show,hCK_HidePop;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(g_brushBKPage == NULL)
		{
			g_brushBKPage = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
		}
		hButton_ShowBK = GetDlgItem(hwndDlg,IDC_TAB_SHOWBK);
		EnableWindow(hButton_ShowBK,FALSE);



		hBKListMenu = LoadMenu(hInst,
			MAKEINTRESOURCE(IDR_MENU_BKITEM));
		hBKListMenu_Sub = GetSubMenu(hBKListMenu,0);

		
		hBKListMenu_Wizard = LoadMenu(hInst,
			MAKEINTRESOURCE(IDR_MENU_BKTAB));
		hBKListMenu_WizardSub = GetSubMenu(hBKListMenu_Wizard,0);

		
		hBK_ADLink = GetDlgItem(hwndDlg,IDC_BK_ADLINK);
#ifndef _NOADS
		m_lnk_bk.Attach(hBK_ADLink);
// 		m_lnk_bk.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.store3000.com/hidedragon", TRUE);

		{
			wchar_t * cDefaultText = new wchar_t[1024];
			memset(cDefaultText,0,1024*sizeof(wchar_t));
			LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
			
			wchar_t * cDefaultLink = new wchar_t[1024];
			memset(cDefaultLink,0,1024*sizeof(wchar_t));
			
			LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.store3000.com/hidedragon");
			
			
			m_lnk_bk.SetLink(cDefaultText,cDefaultLink, TRUE);
			
			
			delete [] cDefaultText;
			cDefaultText = NULL;
			delete [] cDefaultLink;
			cDefaultLink = NULL;
		}

		
		{		
			HWND ads_bk = GetDlgItem(hwndDlg,IDC_BK_ADS);


			ShowWindow(ads_bk,FALSE);
			
#ifndef _NOADS
			gif_bk.Attach(ads_bk);
			wchar_t cFileName[1024] = L"";
			GetCurrentDirectory_HD(1024,cFileName);
			wcscat(cFileName,L"\\gif\\gif_bk_def.gif");
			gif_bk.Load(cFileName);

			gif_bk.Draw();

#endif

			PostMessage(hwndDlg,WM_SIZE,NULL,NULL);
		}


#endif

#ifdef _NOADS
		ShowWindow(hBK_ADLink,SW_HIDE);

#endif
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_TAB_BKLIST);
		}

		    
		if(g_UseIcon == 1)
		{
			HWND hShowThisItem = GetDlgItem(hwndDlg,IDC_TAB_SHOWBK);
			
						
			HICON hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_BK_SHOWITEM),IMAGE_ICON,16,16,NULL);
			
			
			
			SendMessage(hShowThisItem,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);
			
			
			
			RECT rtM;
			rtM.left = 0;
			rtM.right = 20;
			rtM.top = 0;
			rtM.bottom = 00;
			Button_SetTextMargin(hShowThisItem,&rtM);
			
		}




		return FALSE;
	case WM_SETCURSOR:
		{
#ifndef _NOADS
			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_BK_ADS);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				SetCursor(hHand);
			}
			else
			{
				SetCursor(hNorm);
			}
#endif
			
			return TRUE;
			break;
		}
	case WM_LBUTTONDOWN:
		{
#ifndef _NOADS

			POINT pMouse;
			HWND hwndGif = GetDlgItem(hwndDlg,IDC_BK_ADS);
			RECT rtGif;
			GetWindowRect(hwndGif,&rtGif);
			
			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtGif,2);
			
			GetCursorPos(&pMouse);
			ScreenToClient(hwndDlg,&pMouse);
			
			if (PtInRect(&rtGif,pMouse))
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					urlArray[2]->cUrl,NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			else
			{
				SetCursor(hNorm);
			}
			
#endif
			return TRUE;
			break;
		}
		
		break;
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)(wParam),GetSysColor(CTLCOLOR_DLG));
		return (LRESULT)g_brushBKPage;
	case WM_CTLCOLORDLG:
		return (LRESULT)g_brushBKPage;
	case WM_SIZING:
	case WM_SIZE:
		{
			hBKList = GetDlgItem(hwndDlg,IDC_TAB_BKLIST);
			hBtn_Show = GetDlgItem(hwndDlg,IDC_TAB_SHOWBK);
			hCK_HidePop = GetDlgItem(hwndDlg,IDC_TAB_CHECK_AUTOHIDE);



			RECT pageRect;
			GetWindowRect(hwndDlg,&pageRect);


#ifndef _NOADS


			SetWindowPos(hBKList,NULL,10,120,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-150,SWP_NOZORDER);
#else

			SetWindowPos(hBKList,NULL,10,50,
				pageRect.right-pageRect.left - 20,pageRect.bottom-pageRect.top-79,SWP_NOZORDER);

#endif

			HWND ads_bk = GetDlgItem(hwndDlg,IDC_BK_ADS);

#ifdef _NOADS


			ShowWindow(ads_bk,FALSE);

#endif





			SetWindowPos(ads_bk,NULL,10,5,
				pageRect.right-pageRect.left - 20,60,SWP_NOZORDER);
			
			RECT showRect;
			GetWindowRect(hBtn_Show,&showRect);
			LONG showLen = showRect.right - showRect.left;

#ifndef _NOADS


			SetWindowPos(hBtn_Show,NULL,10/*pageRect.right-pageRect.left-showLen-9*/,
				80/*pageRect.bottom-pageRect.top-25*/,0,0,SWP_NOZORDER|SWP_NOSIZE);
#else


			SetWindowPos(hBtn_Show,NULL,10/*pageRect.right-pageRect.left-showLen-9*/,
				10/*pageRect.bottom-pageRect.top-25*/,0,0,SWP_NOZORDER|SWP_NOSIZE);

#endif


			RECT adlinkRect;
			GetWindowRect(hBK_ADLink,&adlinkRect);
			RECT btnRect;
			GetWindowRect(hBtn_Show,&btnRect);
			LONG btnLen = btnRect.right - btnRect.left;
			LONG adlinkLen = adlinkRect.right - adlinkRect.left;
			SetWindowPos(hBK_ADLink,NULL,btnLen + 30,85,pageRect.right-pageRect.left - (btnLen + 50),
				20,SWP_NOZORDER);


			SetWindowPos(hCK_HidePop,NULL,10,
				pageRect.bottom-pageRect.top-23,0,0,SWP_NOZORDER|SWP_NOSIZE);

		}
		break;
	case WM_NOTIFY:
		{
			if(&lParam == NULL)
			{
				return FALSE;
			}
			switch((((LPNMHDR)lParam)->code))
			{
			case NM_CLICK:
				if(sOP.bBeginnerModel == 0)
				{
					break;
				}
			case NM_DBLCLK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem >= 0 && pIA->hdr.idFrom == IDC_TAB_BKLIST)
					{
						SendMessage(hwndDlg,WM_COMMAND,IDM_BKITEM_SHOW,lParam);
					}
				}
				break;
			case NM_RCLICK:
				{
					NMITEMACTIVATE* pIA = (NMITEMACTIVATE*)lParam;
					if(pIA->iItem < 0 && pIA->hdr.idFrom == IDC_TAB_BKLIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);
						TrackPopupMenu(hBKListMenu_WizardSub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);



						break;
					}
					else if(pIA->iItem >= 0 && pIA->hdr.idFrom == IDC_TAB_BKLIST)
					{
						POINT CurMouPos;
						GetCursorPos(&CurMouPos);

						TrackPopupMenu(hBKListMenu_Sub,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
					}
				}

			default:
				break;
			}
		}
		break;
	case WM_DESTROY:
		DestroyMenu(hBKListMenu_Wizard);
		DestroyMenu(hBKListMenu);
		DestroyWindow(g_TabWnd[2]);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_TAB_CHECK_AUTOHIDE:
				if(IsDlgButtonChecked(hwndDlg,IDC_TAB_CHECK_AUTOHIDE) == BST_CHECKED)
				{
					sOP.bAutoHide = 1;
				}
				else
				{
					sOP.bAutoHide = 0;
				}
				WriteOptionFile(sOP);

				break;
			case IDCANCEL:

				break;
			case IDM_BKITEM_SHOW:
				SendMessage(hMainDialog,WM_COMMAND,IDC_SHOWBK,0);
				
				break;
			case IDC_TAB_SHOWBK:
				SendMessage(hMainDialog,WM_COMMAND,IDC_SHOWBK,0);
				break;
			case IDM_BKTAB_BKWIZARD:
				SendMessage(hMainDialog,WM_COMMAND,IDC_BKWIZARD,0);
				break;

			default:
				return FALSE;
		}
	}


	return FALSE;
}


int CreateInfoPanel()
{
	HWND hInfoPanel = CreateDialog(hInst,MAKEINTRESOURCE(IDD_PANEL_ALLINFO),
		hMainDialog,InfoPanelProc);
	
	g_hInfoPanel = hInfoPanel;
	ShowWindow(hInfoPanel,SW_SHOW);
	

	RECT mainRect;
	GetWindowRect(hMainDialog,&mainRect);
	SetWindowPos(hInfoPanel,NULL,10,30,
		mainRect.right - mainRect.left,(mainRect.bottom-mainRect.top)/5,SWP_NOZORDER);




	return 1;
}



INT_PTR CALLBACK InfoPanelProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_PANEL_ALLINFO);
		}


		if(g_UseIcon == 1)
		{
			HWND hOption_info = GetDlgItem(hwndDlg,IDC_INFO_OPTION);
			HWND hHideIcon_info = GetDlgItem(hwndDlg,IDC_INFO_HIDEICON);
			HWND hWizard_info = GetDlgItem(hwndDlg,IDC_INFO_BKWIZARD);
			
			
			HICON hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_INFO_OPTION),IMAGE_ICON,16,16,NULL);
						
			SendMessage(hOption_info,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);


			
			hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_INFO_HIDEICON),IMAGE_ICON,16,16,NULL);		
			
			SendMessage(hHideIcon_info,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);


			hI3 = (struct HICON__*)LoadImage(
				hInst,MAKEINTRESOURCE(IDI_INFO_BKWIZARD),IMAGE_ICON,16,16,NULL);
			
			SendMessage(hWizard_info,BM_SETIMAGE,(WPARAM)IMAGE_ICON, (LPARAM)(HANDLE)hI3);
			
			DestroyIcon(hI3);




			
			
			RECT rtM;
			rtM.left = 0;
			rtM.right = 20;
			rtM.top = 0;
			rtM.bottom = 00;
			Button_SetTextMargin(hOption_info,&rtM);
			
			Button_SetTextMargin(hHideIcon_info,&rtM);
			Button_SetTextMargin(hWizard_info,&rtM);
		}







		return FALSE;

	case WM_DESTROY:
		DestroyWindow(g_hInfoPanel);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDCANCEL:

				break;
			case IDC_INFO_OPTION:
				SendMessage(hMainDialog,WM_COMMAND,IDC_OPTION,NULL);
				break;
			case IDC_INFO_HIDEICON:
				SendMessage(hMainDialog,WM_COMMAND,IDC_HIDEICON,NULL);
				break;
			case IDC_INFO_BKWIZARD:
				SendMessage(hMainDialog,WM_COMMAND,IDC_BKWIZARD,NULL);
				break;

			default:
				return FALSE;
		}
	}


	return FALSE;
}

int ExpendInfoPanel(int iVisible /* == 0*/)
{
	RECT mainRect;
	RECT tabRect;
	GetWindowRect(hMainDialog,&mainRect);
	DWORD dwVer = GetOSVersion();

	
	if(IsWindowVisible(g_hInfoPanel) == TRUE || iVisible == 1)
	{
		AnimateWindow(g_hInfoPanel,100,AW_HIDE|AW_VER_NEGATIVE);
		if(iVisible == 1)
		{
			ShowWindow(g_hInfoPanel,SW_HIDE);
		}

		if(dwVer < 6)
		{
			SetWindowPos(g_Tab,NULL,10,30,mainRect.right-mainRect.left-(25),
				mainRect.bottom-mainRect.top-110,SWP_NOZORDER);
		}
		else
		{
			SetWindowPos(g_Tab,NULL,10,30,mainRect.right-mainRect.left-(30),
				mainRect.bottom-mainRect.top-110,SWP_NOZORDER);
		}
		
		
		
		GetWindowRect(g_Tab,&tabRect);

		SetWindowPos(g_TabWnd[0],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[1],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[2],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[3],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
#ifdef _HIDEPROCESS
		SetWindowPos(g_TabWnd[4],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
#endif
		SetWindowPos(g_TabWnd[5],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[6],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

		SetWindowPos(g_TabWnd[7],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);



		sOP.bCloseInfoPanel = 1;
	}
	else
	{
		AnimateWindow(g_hInfoPanel,100,AW_VER_POSITIVE);
	
		if(dwVer < 6)
		{
			SetWindowPos(g_Tab,NULL,10,130,mainRect.right-mainRect.left-(25),
				mainRect.bottom-mainRect.top-210,SWP_NOZORDER);
		}
		else
		{
			SetWindowPos(g_Tab,NULL,10,130,mainRect.right-mainRect.left-(30),
				mainRect.bottom-mainRect.top-210,SWP_NOZORDER);
		}
		
		
		
		InvalidateRect(g_Tab,NULL,TRUE);
		GetWindowRect(g_Tab,&tabRect);


		SetWindowPos(g_TabWnd[0],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[1],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[2],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[3],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
#ifdef _HIDEPROCESS
		SetWindowPos(g_TabWnd[4],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
#endif
		SetWindowPos(g_TabWnd[5],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[6],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
		SetWindowPos(g_TabWnd[7],NULL,1,30,
			tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);

		sOP.bCloseInfoPanel = 0;
	}
	WriteOptionFile(sOP);

	return 1;
}


int ResizeInfoPanel()
{
	HWND hStaticMouse,hStaticHideSelf,hStaticHideAll,hStaticMouseInfo,hHK_HA,hHK_SS,
		hCK_Top,hBtn_Option,hBtn_HideIcon,hBtn_BKWizard,hInfo_Group;



	hStaticMouse = GetDlgItem(g_hInfoPanel,IDC_STATIC_INFO_MOUSE);
	hStaticHideSelf = GetDlgItem(g_hInfoPanel,IDC_STATIC_INFO_SHOWHIDEDRAGON);
	hStaticHideAll = GetDlgItem(g_hInfoPanel,IDC_STATIC_INFO_HIDEALLWINDOW);
	hStaticMouseInfo = GetDlgItem(g_hInfoPanel,IDC_INFO_MOUSE);
	hHK_HA = GetDlgItem(g_hInfoPanel,IDC_HOTKEY_INFO_HA);
	hHK_SS = GetDlgItem(g_hInfoPanel,IDC_HOTKEY_INFO_SS);
	hCK_Top = GetDlgItem(g_hInfoPanel,IDC_CHECK_INFO_ONTOP);
	hBtn_Option = GetDlgItem(g_hInfoPanel,IDC_INFO_OPTION);
	hBtn_HideIcon = GetDlgItem(g_hInfoPanel,IDC_INFO_HIDEICON);
	hBtn_BKWizard = GetDlgItem(g_hInfoPanel,IDC_INFO_BKWIZARD);
	hInfo_Group = GetDlgItem(g_hInfoPanel,IDC_STATIC_INFO_GROUP);




	
	RECT mainRect;
	GetWindowRect(hMainDialog,&mainRect);
	
	RECT panelRect;
	GetWindowRect(g_hInfoPanel,&panelRect);



	
	SetWindowPos(g_hInfoPanel,NULL,10,30,
		mainRect.right - mainRect.left - 20,
		100/*(mainRect.bottom-mainRect.top)/5*/,SWP_NOZORDER);


	RECT topRect,minfoRect;
	GetWindowRect(g_hInfoPanel,&panelRect);
	GetWindowRect(hCK_Top,&topRect);
	GetWindowRect(hStaticMouseInfo,&minfoRect);
	RECT hideiconRect,bkwizardRect;
	GetWindowRect(hBtn_HideIcon,&hideiconRect);
	GetWindowRect(hBtn_BKWizard,&bkwizardRect);

	LONG lpanelLen = panelRect.right - panelRect.left;
	LONG lTopLen = topRect.right - topRect.left;
	
	float xPos = ((float)(bkwizardRect.left - minfoRect.right) - lTopLen)/(float)2;
	SetWindowPos(hCK_Top,NULL,xPos+(minfoRect.right-panelRect.left),
		26,0,0,SWP_NOZORDER|SWP_NOSIZE);


	LONG lBtnWidth = hideiconRect.right - hideiconRect.left;
	LONG lBtnWizardWidth = bkwizardRect.right - bkwizardRect.left;
	LONG lBtnHeight = hideiconRect.bottom - hideiconRect.top;
	SetWindowPos(hBtn_Option,NULL,panelRect.right - panelRect.left - lBtnWidth * 2 - 22,
		20,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowPos(hBtn_HideIcon,NULL,panelRect.right - panelRect.left - lBtnWidth -20,
		20,0,0,SWP_NOZORDER|SWP_NOSIZE);
	SetWindowPos(hBtn_BKWizard,NULL,panelRect.right - panelRect.left - lBtnWizardWidth - 20,
		20 + lBtnHeight,0,0,SWP_NOZORDER|SWP_NOSIZE);

	SetWindowPos(hInfo_Group,NULL,0,0,
		panelRect.right - panelRect.left - 10,(panelRect.bottom-panelRect.top) - 10,
		SWP_NOZORDER);




	return 1;
}
  
HWND hwndStatus; 

int ResizeTabWnd()
{
	RECT tabRect;
	RECT mainRect;
	if(IsWindowVisible(hMainDialog) != TRUE)
	{
		return 0;
	}
	GetWindowRect(hMainDialog,&mainRect);
	RECT tipRect;
	LONG mainLen = 0;
	LONG tipLen = 0;
	if(hUsageTip && IsWindow(hUsageTip))
	{
		GetWindowRect(hUsageTip,&tipRect);

		mainLen = mainRect.right - mainRect.left;
		tipLen = tipRect.right - tipRect.left;
	}


	HWND hStaticExpandBtn = GetDlgItem(hMainDialog,IDC_STATIC_EXPANDINFO);

	float ftipPos = (float)(mainLen-tipLen)/(float)2;
	SetWindowPos(hUsageTip,NULL,ftipPos,5,
		0,0,SWP_NOZORDER|SWP_NOSIZE);
	
	SetWindowPos(hStaticExpandBtn,NULL,mainLen- 50,
		9,0,0,SWP_NOZORDER|SWP_NOSIZE);

	RECT statusRect;
	GetWindowRect(hwndStatus,&statusRect);








	LONG statusHeight = statusRect.bottom - statusRect.top;
	LONG mainHeight = mainRect.bottom - mainRect.top;
	SetWindowPos(hwndStatus,NULL,0,
		mainHeight  - statusHeight,mainLen,statusHeight,SWP_NOZORDER);




	SetWindowPos(g_hStatus_Link,NULL,20,
		3/*mainHeight  - statusHeight*/,mainLen-30,statusHeight-5,SWP_NOZORDER);
	 

    RECT rcClient; 
    HLOCAL hloc; 
    LPINT lpParts; 
    int nWidth; 
	
    hloc = LocalAlloc(LHND, sizeof(int) * 2); 
    lpParts = (int*)LocalLock(hloc); 
	
    GetClientRect(hMainDialog, &rcClient); 
    nWidth = rcClient.right / 3; 

	
    lpParts[0] = nWidth * 2;
    lpParts[1] = -1; 


    // Tell the status bar to create the window parts. 
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) 2, 
        (LPARAM) lpParts); 
	
    LocalUnlock(hloc); 
    LocalFree(hloc); 
	
	
	
	
	
	
	
	


	DWORD dwVer = GetOSVersion();
	
	
	ShowWindow(hwndStatus,SW_SHOWNORMAL);
	
	if(IsWindowVisible(g_hInfoPanel) == TRUE)
	{
		if(dwVer < 6)
		{
			SetWindowPos(g_Tab,NULL,10,130,mainRect.right-mainRect.left-(25),
				mainRect.bottom-mainRect.top-210,SWP_NOZORDER);
		}
		else
		{
			SetWindowPos(g_Tab,NULL,10,130,mainRect.right-mainRect.left-(30),
				mainRect.bottom-mainRect.top-210,SWP_NOZORDER);
		}
	}
	else
	{
		if(dwVer < 6)
		{
			SetWindowPos(g_Tab,NULL,10,30,mainRect.right-mainRect.left-(25),
				mainRect.bottom-mainRect.top-110,SWP_NOZORDER);
		}
		else
		{
			SetWindowPos(g_Tab,NULL,10,30,mainRect.right-mainRect.left-(30),
				mainRect.bottom-mainRect.top-110,SWP_NOZORDER);
		}
		InvalidateRect(g_Tab,NULL,TRUE);
	}
	GetWindowRect(g_Tab,&tabRect);
	SetWindowPos(g_TabWnd[0],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[1],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[2],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[3],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
#ifdef _HIDEPROCESS
	SetWindowPos(g_TabWnd[4],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
#endif
	SetWindowPos(g_TabWnd[5],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[6],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);
	SetWindowPos(g_TabWnd[7],NULL,1,30,
		tabRect.right-tabRect.left-5,tabRect.bottom-tabRect.top-35,SWP_NOZORDER);


	return 1;
}



int InitTab()
{
	{
		DoCreateTabControl(hMainDialog);
		CreateInfoPanel();
		OnTabSelChange(0);
		OnTabSelChange(1);
		OnTabSelChange(2);
		OnTabSelChange(3);

		
		OnTabSelChange(4);
		//OnTabSelChange(5);
		//OnTabSelChange(6);


		OnTabSelChange(0);
	}


	return 1;
}



CHyperLink m_lnk1;



HWND DoCreateStatusBar()
{
    RECT rcClient; 
    HLOCAL hloc; 
    LPINT lpParts; 
    int nWidth; 
 
    // Ensure that the common control DLL is loaded. 
 
    // Create the status bar. 
    hwndStatus = CreateWindowEx( 
        0,                       // no extended styles 
        STATUSCLASSNAME,         // name of status bar class 
        (LPCTSTR) NULL,          // no text when first created 
        SBARS_SIZEGRIP |         // includes a sizing grip 
        WS_CHILD,                // creates a child window 
        0, 0, 0, 0,              // ignores size and position 
        hMainDialog,              // handle to parent window 
        (HMENU) 100000,       // child window identifier 
        hInst,                   // handle to application instance 
        NULL);                   // no window creation data 
 
 
    // Allocate an array for holding the right edge coordinates. 
    hloc = LocalAlloc(LHND, sizeof(int) * 2); 
    lpParts = (int*)LocalLock(hloc); 
 
    // Calculate the right edge coordinate for each part, and 
    // copy the coordinates to the array. 
    // Get the coordinates of the parent window's client area. 
    GetClientRect(hMainDialog, &rcClient); 
    nWidth = rcClient.right ;

	
    lpParts[0] = nWidth - 10;
    lpParts[1] = 10; 

#ifdef _NOADS
    // Tell the status bar to create the window parts. 
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) 2, 
        (LPARAM) lpParts); 

	wchar_t cText[1024] = L"\0",* pText = NULL;
	pText = new wchar_t[1024];
	memset(pText,0,1024);


	wcscpy(cText,LoadItemStringW(L"IDS_STATUSBAR_WELCOME",pText,L"Welcome to use HideDragon!"));
	delete [] pText;


    SendMessageW(hwndStatus, SB_SETTEXT, 0|0, 
        (LPARAM) cText); 
#endif
 
    // Free the array, and return. 
    LocalUnlock(hloc); 
    LocalFree(hloc); 

//	g_pWndProcStatus = (WNDPROC)SetWindowLong(hwndStatus, GWL_WNDPROC, 
//		(LPARAM)(WNDPROC)NewStatusProc);


#ifndef _NOADS

	g_hStatus_Link = CreateWindowEx(NULL, L"static",L"ttttttttttttttttt",
		WS_CHILD | WS_VISIBLE,0,0,0,0,
		hwndStatus,(HMENU)3333,NULL,NULL);

	m_lnk1.Attach(g_hStatus_Link);
// 	m_lnk1.SetLink(L"隐身专家广告联盟，让伙伴更强！", 
// 			L"http://www.store3000.com/hidedragon", TRUE);

				{
					wchar_t * cDefaultText = new wchar_t[1024];
					memset(cDefaultText,0,1024*sizeof(wchar_t));
					LoadItemStringW(L"IDS_ADSTEXT_DEFAULT",cDefaultText,L"隐身专家广告联盟，让伙伴更强！");
					
					wchar_t * cDefaultLink = new wchar_t[1024];
					memset(cDefaultLink,0,1024*sizeof(wchar_t));
					
					LoadItemStringW(L"IDS_ADSLINK_DEFAULT",cDefaultLink,L"http://www.store3000.com/hidedragon");
					
					
					m_lnk1.SetLink(cDefaultText,cDefaultLink, TRUE);
					
					
					delete [] cDefaultText;
					cDefaultText = NULL;
					delete [] cDefaultLink;
					cDefaultLink = NULL;
				}





#endif



    return hwndStatus; 
}








int SetStatusLanguage()
{
	wchar_t cText[1024] = L"\0",* pText = NULL;
	pText = new wchar_t[1024];
	memset(pText,0,1024);


	wcscpy(cText,LoadItemStringW(L"IDS_STATUSBAR_WELCOME",pText,L"Welcome to use HideDragon!"));
	delete [] pText;


    SendMessageW(hwndStatus, SB_SETTEXT, 0|0, 
        (LPARAM) cText); 
	



	return 1;
}











int SavePosSize()
{
	RECT mainRect;

	if(IsWindowVisible(hMainDialog) != TRUE)
	{
		return 0;
	}

	GetWindowRect(hMainDialog,&mainRect);
	


	DBClass db;
	SizePosItem SPI;
	SPI.id = 0;
	SPI.left = mainRect.left;
	SPI.right = mainRect.right;
	SPI.top = mainRect.top;
	SPI.bottom = mainRect.bottom;

	
	db.SaveSizePos(&SPI);







	return 1;
}



int SetWindowSizePos()
{
	DBClass db;

	SizePosItem SPI;
	db.ReadSizePos(&SPI);



	LONG mainLen = 0,mainHei = 0;

	mainLen = SPI.right - SPI.left;
	mainHei = SPI.bottom - SPI.top;

	
	RECT rtDesktop;
	
	HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop,&rtDesktop);

	HWND hTray    = ::FindWindow(L"Shell_TrayWnd", NULL);

	HWND hReBar   = FindWindowEx( hTray    , 0, L"ReBarWindow32"   , NULL );


	RECT rtTaskbar;

	GetWindowRect(hReBar,&rtTaskbar);
	
	LONG tkHeight = rtTaskbar.bottom - rtTaskbar.top;
	

	if(SPI.right > rtDesktop.right)
	{
		SPI.left = rtDesktop.right - mainLen;
	}
	if(SPI.bottom > rtDesktop.bottom)
	{
		SPI.top = rtDesktop.bottom - mainHei - tkHeight;
	}
	if(SPI.left < rtDesktop.left)
	{
		SPI.left = rtDesktop.left;
	}
	if(SPI.top < rtDesktop.top)
	{
		SPI.top = rtDesktop.top;
	}

	SetWindowPos(hMainDialog,NULL,SPI.left,SPI.top,mainLen,mainHei,SWP_NOZORDER);



	SendMessage(hMainDialog,WM_SIZING,NULL,NULL);


	//ExpendInfoPanel();
	
	ExpendInfoPanel(sOP.bCloseInfoPanel);


	return 1;
}


int ChangeMenuState(int iTabNum)
{
	switch(iTabNum)
	{
	case 0:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_ENABLED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_ENABLED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_ENABLED);
			
			DrawMenuBar(hMainDialog);
		}
		
		break;
	case 1:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	case 2:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	case 3:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	case 4:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	case 5:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	case 6:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	case 7:
		{			
			HMENU hMainMenu = GetMenu(hMainDialog);
			EnableMenuItem(hMainMenu,1,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,2,MF_BYPOSITION|MF_GRAYED);
			EnableMenuItem(hMainMenu,3,MF_BYPOSITION|MF_GRAYED);
			
			DrawMenuBar(hMainDialog);
		}
		break;
	default:
		break;
	}



	return 1;
}




int CreateAutoChangeThread()
{
	if(g_ChangeIconTitleHandle == NULL)
	{
		g_ChangeIconTitleHandle = (HANDLE)_beginthreadex(NULL,0,AutoChangeIconTitle,NULL,0,NULL);
		if(g_ChangeIconTitleHandle == INVALID_HANDLE_VALUE)
		{
			g_hUpdateLangHandle = NULL;
			return 0;
		}
	}

	return 1;
}





int CreateAutoHideThread()
{
	if(g_AutoHideHandle == NULL)
	{
		g_AutoHideHandle = (HANDLE)_beginthreadex(NULL,0,AutoHideThreadProc,NULL,0,NULL);
		if(g_AutoHideHandle == INVALID_HANDLE_VALUE)
		{
			g_AutoHideHandle = NULL;
			return 0;
		}
	}
	else
	{
		return 0;
	}


	return 1;
}



ProcessList g_ProcessList1,g_ProcessList2,g_NewProcessList;
DBProcessList g_DBProcessList;



int AllocProcessList()
{
	int i = 0;

	for(i = 0;i < 150;i ++)
	{
		NewProcessInfo * pInfo = new NewProcessInfo;
		pInfo->pName = new wchar_t[1024];
		memset(pInfo->pName,0,1024);
		pInfo->dwPID = 0;
		
		g_ProcessList1.push_back(pInfo);
	}

	for(i = 0;i < 150;i ++)
	{
		NewProcessInfo * pInfo = new NewProcessInfo;
		pInfo->pName = new wchar_t[1024];
		memset(pInfo->pName,0,1024);
		pInfo->dwPID = 0;
		
		g_ProcessList2.push_back(pInfo);
	}
	for(i = 0;i < 150;i ++)
	{
		NewProcessInfo * pInfo = new NewProcessInfo;
		pInfo->pName = new wchar_t[1024];
		memset(pInfo->pName,0,1024);
		pInfo->dwPID = 0;
		
		g_NewProcessList.push_back(pInfo);
	}






	return 1;
}



int ResetPorcessList(ProcessList & processList)
{
	ProcessList::iterator iNode;
	for(iNode = processList.begin();iNode != processList.end();iNode ++)
	{
		NewProcessInfo * pI = *iNode;
		memset(pI->pName,0,1024*sizeof(wchar_t));
		pI->dwPID = 0;
	}
	


	return 1;
}




int FreeProcessList(ProcessList & processList)
{
	ProcessList::iterator pNode;
	for(pNode = processList.begin(); pNode != processList.end(); /*pNode ++*/)
	{
		NewProcessInfo * pInfo = *pNode;
		processList.erase(pNode ++);
		//pNode --;
		
		
		delete [] pInfo->pName;
		
		delete pInfo;
	}



	return 1;
}

int GetNewProcessName(ProcessList & newProcessList)
{
	ResetPorcessList(g_NewProcessList);


	ProcessList::iterator oNode;
	for(oNode = g_ProcessList2.begin(); oNode != g_ProcessList2.end(); oNode ++)
	{
		NewProcessInfo * pInfo = *oNode;

		

		ProcessList::iterator nNode;
		int iFoundFlag = 0;
		for(nNode = g_ProcessList1.begin(); nNode != g_ProcessList1.end(); nNode ++)
		{
			NewProcessInfo * oldInfo = *nNode;
			if(oldInfo->dwPID == pInfo->dwPID)
			{
				iFoundFlag = 1;
				break;
			}
		}
		if(iFoundFlag == 0)
		{
			Add2NewProcessList(pInfo);
		}
	}
	
	return 1;
}





int Add2NewProcessList(NewProcessInfo * pInfo)
{
	ProcessList::iterator oNode;
	for(oNode = g_NewProcessList.begin(); oNode != g_NewProcessList.end(); oNode ++)
	{
		NewProcessInfo * pNewItem = *oNode;

		if(pNewItem->dwPID == 0)
		{
			wcscpy(pNewItem->pName,pInfo->pName);
			pNewItem->dwPID = pInfo->dwPID;
			break;
		}	
	}
	return 1;
}


int g_iProcessCount = 0;

int GetProcessList(ProcessList & processList)
{
	PROCESSENTRY32 pe32;
	
	
	ResetPorcessList(processList);
	
	
	HANDLE hProcessSnap = NULL;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First((void*)hProcessSnap,&pe32);
	ProcessList::iterator iNode;
	
	iNode = processList.begin();
	while(Process32Next((void*)hProcessSnap,&pe32) && iNode != processList.end())
	{
		NewProcessInfo * pI = *iNode;
		


		wcscpy(pI->pName,pe32.szExeFile);
		pI->dwPID = pe32.th32ProcessID;
		
		iNode ++;

		g_iProcessCount ++;
	}
	CloseHandle((HANDLE)hProcessSnap);
	
	
		



	return 1;
}



unsigned __stdcall AutoHideThreadProc(void * pParam)
{
	int iCountFlag = ReadDBProcessList();


	AllocResourceForTrayList_au();
	AllocAutoHideWindow();

	while(1)
	{
		ResetAutoHideWindow();
		EnumAllVisibleWindow_au();

		EnumAllVisibleTray_au();
		Sleep(1000);
		if(sOP.bAutoHideProgram == 0)
		{
			break;
		}
	}

	ReleaseResourceForTrayList_au();

	ReleaseDBProcessList();
	ReleaseTrayHidedList();
	
	ReleaseAutoHideWindow();

	ReleaseHidedWndList();


	CloseHandle(g_AutoHideHandle);
	g_AutoHideHandle = NULL;
	

	return 1;
}




int ReadDBProcessList()
{
	DBClass db;


		
	AutoHideItem * pItem = new AutoHideItem;
	pItem->cProcessName = new wchar_t[1024];
	memset(pItem->cProcessName,0,1024*sizeof(wchar_t));
	pItem->iType = 0;
	int iExist = 0;
		
	
	int iRet = 1;
	iRet = db.ReadAHItem(pItem,&iExist,Insert2DBProcessList);



	delete [] pItem->cProcessName;
	delete pItem;




	return iRet;
}



int Insert2DBProcessList(AutoHideItem * pItem)
{
	AutoHideItem * pNewItem = new AutoHideItem;
	pNewItem->cProcessName = new wchar_t[1024];
	memset(pNewItem->cProcessName,0,1024*sizeof(wchar_t));


	wcscpy(pNewItem->cProcessName,wcslwr(pItem->cProcessName));
	pNewItem->iType = pItem->iType;




	g_DBProcessList.push_back(pNewItem);




	return 1;
}



int AddDBPorcessList(AutoHideItem* pItem)
{
	AutoHideItem * pAHItem = new AutoHideItem;
	pAHItem->cProcessName = new wchar_t[1024];
	memset(pAHItem->cProcessName,0,1024*sizeof(wchar_t));
	wcscpy(pAHItem->cProcessName,wcslwr(pItem->cProcessName));
	
	pAHItem->iType = pItem->iType; //后面加上TYPE

	g_DBProcessList.push_back(pAHItem);





	return 1;
}



int DelDBProcessList(AutoHideItem* pItem)
{
	DBProcessList::iterator cNode;
	for(cNode = g_DBProcessList.begin();cNode != g_DBProcessList.end(); /*cNode ++*/)
	{
		AutoHideItem * pA =  *cNode;
		if(wcscmp(wcslwr(pA->cProcessName),wcslwr(pItem->cProcessName)) == 0)
		{
			g_DBProcessList.erase(cNode++);
			//cNode --;
			
			delete [] pA->cProcessName;
			delete pA;
			
			break;
		}
	}
		


	return 1;
}

int	ReleaseDBProcessList()
{
	DBProcessList::iterator cNode;
	for(cNode = g_DBProcessList.begin();cNode != g_DBProcessList.end(); /*cNode ++*/)
	{
		AutoHideItem * pA =  *cNode;
		g_DBProcessList.erase(cNode++);
		//cNode --;
		
		delete [] pA->cProcessName;
		delete pA;
	}
	



	return 1;
}




int HideAutoHideItem(AutoHideItem * pItem)
{
	wchar_t cPID[20] = L"";

	
	ProcessList::iterator oNode;
	for(oNode = g_NewProcessList.begin(); oNode != g_NewProcessList.end(); oNode ++)
	{
		NewProcessInfo * pNewItem = *oNode;
		
		if(wcscmp(pNewItem->pName,pItem->cProcessName) == 0)
		{
			SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
			if(pItem->iType == 1)
			{
				SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
				int i = 0;
				for(i = 0;i < iBaSW;i ++)
				{
					if(wcscmp(wcslwr(pItem->cProcessName),wcslwr(sW[i].cProcessName)) == 0)
					{
						memset(cPID,0,20*sizeof(wchar_t));
						swprintf(cPID,L"%X",sW[i].lPID);
						ConstructBKSW(cPID,sW[i].cWndTitle);
						
						HideSelected(&sBKSel,1,0);
						Switch2BKTab();
						break;
					}
				}
			}
			if(pItem->iType == 2)
			{
				WndAndHandle * pTemp = NULL;
				
				FillTrayList();
				SendMessage(g_TabWnd[1],WM_COMMAND,IDC_TAB_BTN_REFRESHLIST,0);
				wchar_t * cProcessName = new wchar_t[1024];
				int i = 0;
				
				
				while(g_QuickHideAllTray[i].lPID != 0 && i < 100)
				{
					memset(cProcessName,0,1024*sizeof(wchar_t));
					
					GetProcessName(g_QuickHideAllTray[i].lPID,cProcessName);
					if(wcscmp(cProcessName,pItem->cProcessName) == 0)
					{
						pTemp = g_QuickHideAllTray + i;
						pTemp->cWndTitle = NULL;
						HWND hWnd = (HWND)1;
						pTemp->hWnd = hWnd;
						HideSelected(&pTemp,1,0);
						FillTrayList();		
						break;
					}
					i ++;
				}
				
				delete [] cProcessName;
			}	
		
		}
	}
	
	
	
	
	return 1;
}




unsigned __stdcall AutoChangeIconTitle(void * pParam)
{
	PROCESSENTRY32 pe32;
	IconTitleItem * pItem = new IconTitleItem;
	memset(pItem,0,sizeof(IconTitleItem));
	pItem->cIconPath = new wchar_t[1024 * 3];
	pItem->cTitle = new wchar_t[1024 * 3];
	HANDLE hProcessSnap = NULL;

	while(g_ExitChangeIconThread == 0)
	{
		if(sOP.bAutoChangeIconTitle == 1)
		{
			hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
			memset(&pe32,0,sizeof(PROCESSENTRY32));
			pe32.dwSize = sizeof(PROCESSENTRY32);
			Process32First((void*)hProcessSnap,&pe32);

			memset(pItem->cProcessName,0,50 * sizeof(wchar_t));
			memset(pItem->cIconPath,0,1024 * 3 * sizeof(wchar_t));
			memset(pItem->cTitle,0,1024 * 3 * sizeof(wchar_t));


			while(Process32Next((void*)hProcessSnap,&pe32))
			{
				wcscpy(pItem->cProcessName,pe32.szExeFile);

				int iRet = IsInIconTitleDB(pItem);

				if(iRet == 1)
				{
					SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
					ChangeTitleIcon(pe32.th32ProcessID,pItem);
				}
			}
			CloseHandle((HANDLE)hProcessSnap);

			int iCount = GetHidedProcessCount();

			int i = 0;


			wchar_t cProcessName[50] = L"";
			for(i = 0;i < iCount;i ++)
			{
				memset(cProcessName,0,50 * sizeof(wchar_t));
				GetHidedProcessByIdx(i,cProcessName);
				wcscpy(pItem->cProcessName,cProcessName);

				int iRet = IsInIconTitleDB(pItem);

				if(iRet == 1)
				{
					SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,NULL);
					ChangeTitleIcon(pe32.th32ProcessID,pItem);
				}
			}
		}
//		SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,0);
		Sleep(2000);
	}
	delete [] pItem->cIconPath;
	delete [] pItem->cTitle;
	delete pItem;



	CloseHandle(g_ChangeIconTitleHandle);
	g_ChangeIconTitleHandle = NULL;

	return 1;
}







int SaveAutoChangeIconTitle()
{
	sOP.bAutoChangeIconTitle = (sOP.bAutoChangeIconTitle == 0) ? 1:0;
	WriteOptionFile(sOP);

	if(sOP.bAutoChangeIconTitle == 1)
	{
		g_ExitChangeIconThread = 0;
		CreateAutoChangeThread();

	}
	else
	{
		CloseHandle(g_ChangeIconTitleHandle);
		g_ChangeIconTitleHandle = NULL;
		
		g_ExitChangeIconThread = 1;
	}

	return 1;
}


int SaveChangedIconInfo(IconTitleItem * pItem)
{
	DBClass db;


	db.SaveChangedIconTitle(pItem,0);

	return 1;
}



int SaveChangedTitleInfo(IconTitleItem * pItem)
{
	DBClass db;


	db.SaveChangedIconTitle(pItem,1);


	return 1;
}

int IsInIconTitleDB(IconTitleItem * pItem)
{
	DBClass db;

	int iExist = 0;
	db.ReadIconTitleItem(pItem,&iExist);
	
	
	return iExist;
}






int ChangeTitleIcon(DWORD PID,IconTitleItem * pItem)
{
//	SendMessage(hMainDialog,WM_COMMAND,IDC_REFRESH,0);
	int iCount = ListView_GetItemCount(hListView);
	int i = 0;

	wchar_t cPID[20],cPID2[20];

	DWORD dwPID = 0;
	for(i = 0;i < iCount;i ++)
	{
		memset(cPID,0,20*sizeof(wchar_t));
		memset(cPID2,0,20*sizeof(wchar_t));
		ListView_GetItemText(hListView,i,1,cPID,20);
		swprintf(cPID2,L"%X",PID);
		
		if(wcscmp(cPID,cPID2) == 0)
		{
			if(IsInIconList(pItem,cPID) == 0)
			{
				ChangeWindowIcon(i,pItem->cIconPath);
			}
			if(IsInTitleList(pItem,cPID) == 0)
			{
				if(wcscmp(pItem->cTitle,L""))
				{
					ChangeWndTitle(i,pItem->cTitle);
				}
			}
		}
	}


	return 1;
}


int IsInIconList(IconTitleItem * pItem,wchar_t * cPID)
{
	int i = 0;
	sIconTitle * pIconTemp = NULL,* pIconPrev = NULL;
		
	ITList::iterator iNode;
	TWList::iterator tNode;
		
	int iICSize = g_ICList.size();
	for(iNode = g_ICList.begin();iNode != g_ICList.end(); iNode ++)
	{
		sIconTitle * pICDel = * iNode;
		if(wcscmp(pICDel->cPID,cPID) == 0 && wcscmp(pICDel->cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(pICDel->cProcessName,L"explorer.exe") != 0)
		{
			return 1;
		}
		else if(wcscmp(pICDel->cPID,cPID) == 0 && (wcscmp(pICDel->cProcessName,L"iexplore.exe") == 0 || 
			wcscmp(pICDel->cProcessName,L"explorer.exe") == 0))
		{
			for(tNode = pICDel->twlist.begin(); tNode != pICDel->twlist.end(); tNode ++)
			{
				sTitleWnd * pTWDel = *tNode;
				if((wcscmp(pICDel->cProcessName,L"iexplore.exe") == 0 || 
					wcscmp(pICDel->cProcessName,L"explorer.exe") == 0))
				{
					if(wcscmp(pTWDel->pOrgTitle,sW[i].cWndTitle) == 0)
					{
						return 1;
					}
				}
			}
		}
	}


	return 0;
}



int IsInTitleList(IconTitleItem * pItem,wchar_t * cPID)
{
	int i = 0;
		
	ITList::iterator iNode;
	TWList::iterator tNode;
		
	int iICSize = g_ITList.size();
	for(iNode = g_ITList.begin();iNode != g_ITList.end(); iNode ++)
	{
		sIconTitle * pITDel = * iNode;
		if(wcscmp(pITDel->cPID,cPID) == 0 && wcscmp(pITDel->cProcessName,L"iexplore.exe") != 0 && 
			wcscmp(pITDel->cProcessName,L"explorer.exe") != 0)
		{
			return 1;
		}
		else if(wcscmp(pITDel->cPID,cPID) == 0 && wcscmp(pITDel->cProcessName,L"iexplore.exe") == 0 ||
			wcscmp(pITDel->cProcessName,L"explorer.exe") == 0)
		{
			for(tNode = pITDel->twlist.begin(); tNode != pITDel->twlist.end(); tNode ++)
			{
				sTitleWnd * pTWDel = *tNode;
				if((wcscmp(pITDel->cProcessName,L"iexplore.exe") == 0 ||
					wcscmp(pITDel->cProcessName,L"explorer.exe") == 0))
				{
					if(wcscmp(pTWDel->pOrgTitle,pItem->cTitle) == 0)
					{
						return 1;
					}
				}
			}
		}
	}


	return 0;
}




int DeleteIconTitleFromDB(wchar_t * pName,int iFlag/* == 0*/)
{
	DBClass db;


	IconTitleItem * pItem = new IconTitleItem;
	memset(pItem->cProcessName,0,50 * sizeof(wchar_t));
	
	wcscpy(pItem->cProcessName,pName);
	pItem->cIconPath = NULL;
	pItem->cTitle = NULL;

	db.DeleteIconTitleItem(pItem,iFlag);

	delete pItem;



	return 1;
}







//LRESULT CALLBACK ExpandBtnSubclassProc(HWND hEwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
//{
//
////	return CallWindowProc(wpOrgExpandProc,hEwnd,uMsg,wParam,lParam);
//}


int WriteTimes(int iTimes)
{
	char cFileContent[20] = "\0";


	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
//	GetModuleFileName(hInst,cARFileName,1024);
//	PathRemoveFileSpec(cARFileName);
	GetSystemDirectory(cARFileName,1024);
	wcscat(cARFileName,L"\\hd_syslog.arf");
	
	HANDLE hTimeFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	delete [] cARFileName;
	
	if(hTimeFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	
	*(int*)cFileContent = sOP.iUsedTimes;
	
	
	DWORD lWritten = 0;

	WriteFile(hTimeFile,cFileContent,sizeof(int),&lWritten,NULL);
	CloseHandle(hTimeFile);	





	return 0;
}



int ReadTimes(int * pTimes)
{
	*pTimes = 0;

	char cFileContent[20] = "\0";


	wchar_t *cARFileName = (wchar_t*) new wchar_t[1024];
	memset(cARFileName,0,1024 * sizeof(wchar_t));
	GetModuleFileName(hInst,cARFileName,1024);
	PathRemoveFileSpec(cARFileName);
	//GetSystemDirectory(cARFileName,1024);
	wcscat(cARFileName,L"\\hd_syslog.arf");
	
	HANDLE hTimeFile = CreateFile(cARFileName,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	delete [] cARFileName;
	
	if(hTimeFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	
	DWORD dwReaded = 0;
	int iTimes = 1;
	ReadFile(hTimeFile,cFileContent,sizeof(char),&dwReaded,NULL);
	if(dwReaded != 0)
	{
		*pTimes = *(int*)cFileContent;
	}
	else
	{
		*(int*)cFileContent = iTimes;
		
		
		DWORD lWritten = 0;

		WriteFile(hTimeFile,cFileContent,sizeof(int),&lWritten,NULL);

		*pTimes = 0;
	}
	
	
	


	CloseHandle(hTimeFile);


	return 1;
}


int SetTrayTipLanguage()
{



	return 1;
}





int	CreateGetAdsLinkThread()
{
	g_GetAdsLinkHandle = (HANDLE)_beginthreadex(NULL,0,GetAdsLinkProc,NULL,0,NULL);
	if(g_GetAdsLinkHandle == INVALID_HANDLE_VALUE)
	{
		g_GetAdsLinkHandle = NULL;
		return 0;
	}
	



	return 1;
}




int g_GetWeb1Fail = 0,g_GetWeb2Fail = 0;


unsigned __stdcall GetAdsLinkProc(void * pParam)
{
	DWORD dwErr = 0;
	BOOL bRet = FALSE;
	wchar_t * pLink_w = new wchar_t[1024];
	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_DBLINK_STATUS",pLink_w,L"/cn/union/webstatus.db");


	char * pLink_a = new char[1024];
	memset(pLink_a,0,1024);
	
	
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);

	int iRet = GetFileFromServer(pLink_a);



	GetGifLink();




	if(iRet == 1)
	{
		g_GetWeb1Fail = 1;
		if(g_SetAdsTimer == NULL)
		{
			g_SetAdsTimer = SetTimer(hMainDialog,IDT_SETADSTEXT,3 * 1000,SetStatusTextLink);
		}
	}
	else
	{
		g_GetWeb1Fail = 1;
		if(g_ReConnTimer == NULL)
		{
			 srand(time(NULL));
			 int iMinute = rand() % 10;
			 g_ReConnTimer = SetTimer(hMainDialog,IDT_RECONNECT,
				iMinute * 60 * 1000,ReconnectTimer);
		}
	}


	memset(pLink_w,0,1024*sizeof(wchar_t));
	LoadItemStringW(L"IDS_DBLINK_WND",pLink_w,L"/cn/union/webstatus.db");
	memset(pLink_a,0,1024);
	
	
	
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pLink_w,
		1024,pLink_a,1024,NULL,FALSE);

	iRet = GetFileFromServer(pLink_a /*"/cn/union/wndads.db"*/);
	if(iRet == 1)
	{
		if(g_SetWndAdsTimer == NULL)
		{
			g_SetAdsTimer = SetTimer(hMainDialog,IDT_SETWNDADSTEXT,2 * 1000,SetWndTextLink);
		}
		g_GetWeb2Fail = 0;
	}
	else
	{
		g_GetWeb2Fail = 1;
		if(g_ReConnTimer == NULL)
		{
			 srand(time(NULL));
			 int iMinute = rand() % 10;
			 g_ReConnTimer = SetTimer(hMainDialog,IDT_RECONNECT,
				iMinute * 60  * 1000,ReconnectTimer);
		}
	}


	if(g_GetWeb2Fail == 0 && g_GetWeb1Fail == 0)
	{
		KillTimer(hMainDialog,IDT_RECONNECT);
		g_ReConnTimer = NULL;
	}

	delete [] pLink_w;
	pLink_w = NULL;
	delete [] pLink_a;
	pLink_a = NULL;


	GetGifLink();


	GetGifFile();





	CloseHandle(g_hUpdateLangHandle);
	g_hUpdateLangHandle = NULL;

	return 1;
}






VOID CALLBACK  ReconnectTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{

	CreateGetAdsLinkThread();


	return;
}





int g_iAdsIdx = 0;

VOID CALLBACK  SetStatusTextLink(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	CWebDB db;

	wchar_t cText[1024] = L"",cLink[1024] = L"";

	if(db.Init() == 0)
	{
		return ;
	}

	int iCount = db.GetCount();



	db.GetStatusTextLink(g_iAdsIdx,cText,cLink);
	g_iAdsIdx ++;

	if(g_iAdsIdx >= iCount)
	{
		g_iAdsIdx = 0;
	}
	

	
	m_lnk1.SetLink(cText, cLink, TRUE);
	
	



	return ;
}





int g_iWndAdsIdx = 0;
extern CHyperLink m_lnk_cleanpri;


VOID CALLBACK  SetWndTextLink(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	CWebDB db("wndads.db");

	wchar_t cText[1024] = L"",cLink[1024] = L"";

	if(db.Init() == 0)
	{
		return ;
	}

	int iCount = db.GetCount();



	db.GetWndTextLink(g_iWndAdsIdx,cText,cLink);
	g_iWndAdsIdx ++;

	if(g_iWndAdsIdx >= iCount)
	{
		g_iWndAdsIdx = 0;
	}
	

	
	m_lnk_adv.SetLink(cText, cLink, TRUE);
	m_lnk_hideip.SetLink(cText, cLink, TRUE);
	m_lnk_tray.SetLink(cText, cLink, TRUE);
	m_lnk_process.SetLink(cText, cLink, TRUE);
	m_lnk_bk.SetLink(cText, cLink, TRUE);
	m_lnk_autohide.SetLink(cText, cLink, TRUE);
	m_lnk_wndlist.SetLink(cText, cLink, TRUE);
	m_lnk_cleanpri.SetLink(cText, cLink, TRUE);


	return ;
}






int RestoreIP()
{
	UseProxyDlg dlg;
	dlg.RestoreIP();


	return 1;
}


HANDLE g_hChangeLangHandle;

struct sChangeLang
{
	WORD wParam_low;
	wchar_t * pMenuString;

};

int ChangeLanguage(WORD loWparam,wchar_t * pMenuString)
{

	sChangeLang * pCL = new sChangeLang;
	pCL->wParam_low = loWparam;
	pCL->pMenuString = new wchar_t[1024];
	memset(pCL->pMenuString,0,1024*sizeof(wchar_t));
	wcscpy(pCL->pMenuString,pMenuString);

	if(g_hChangeLangHandle == NULL)
	{
		g_hChangeLangHandle = (HANDLE)_beginthreadex(NULL,0,ChangeLanguageThreadProc,(void*)pCL,0,NULL);
		if(g_hChangeLangHandle == INVALID_HANDLE_VALUE)
		{
			g_hChangeLangHandle = NULL;
		}

	}


	return 1;

}

extern HWND g_hGenWnd;
extern HWND g_hAdvWnd;



extern HWND g_IPList;

extern HWND g_hCleanStatus;




unsigned __stdcall ChangeLanguageThreadProc(void * pParam)
{
	sChangeLang * pCL = (sChangeLang*)pParam;
	g_iUseLanguage = 1;
	SetDialogTextW(&hMainDialog,IDD_MAINDIALOG);
	SetDialogTextW(&g_hInfoPanel,IDD_PANEL_ALLINFO);
	SetDialogTextW(&g_TabWnd[0],IDD_TAB_ALLWND);
	SetDialogTextW(&g_TabWnd[1],IDD_TAB_TRAYICON);
	SetDialogTextW(&g_TabWnd[2],IDD_TAB_BKLIST);
	SetDialogTextW(&g_TabWnd[3],IDD_TAB_WEBPAGE);
#ifdef _HIDEPROCESS
	SetDialogTextW(&g_TabWnd[4],IDD_TAB_PROCESSLIST);
#endif

#ifdef _HIDEPROCESS
	SetDialogTextW(&g_TabWnd[5],IDD_TAB_CLEANPRIV);
	SetDialogTextW(&g_TabWnd[6],IDD_TAB_HIDEIP);
#else
	SetDialogTextW(&g_TabWnd[5],IDD_TAB_CLEANPRIV);
	SetDialogTextW(&g_TabWnd[6],IDD_TAB_HIDEIP);

#endif

	SetDialogTextW(&g_TabWnd[7],IDD_TAB_AUTOHIDE);

	SetDialogTextW(&g_hGenWnd,IDD_TAB_HIDEIP_GENERAL);
	SetDialogTextW(&g_hAdvWnd,IDD_TAB_HIDEIP_ADVANCE);
	SetDialogTextW(&g_hCleanStatus,IDD_DIALOG_CLEANPROGRESS);
	SetTabLanguage();
#ifdef _NOADS
	SetStatusLanguage();
#endif
	SetTrayTipLanguage();

	SetWindowListViewColumn(hListView);
	SetBKListColumn(hList_BK);
	SetTrayListColumn(g_TrayList);
	SetProcessListColumn(g_ProcessList);
	SetIPListColumn(g_IPList);
	SetAHListColumn(g_AutoHideList);
	SetMenuString();
	ResetPriTree();


	ModifyMenuW(g_hLangMenu,/*LOWORD*/(pCL->wParam_low),MF_CHECKED|MF_STRING,
		/*LOWORD*/(pCL->wParam_low),pCL->pMenuString);

	g_CurrLangID = /*LOWORD*/(pCL->wParam_low);
	
	//ModifyMenu(g_hLangMenu,g_OldLangID,MF_UNCHECKED|MF_,g_OldLangID,g_OldString);
	CheckMenuItem(g_hLangMenu,g_OldLangID,MF_UNCHECKED);
	g_OldLangID = g_CurrLangID;
	//FillList();
	/*MENUITEMINFO MII;
	MII.cbSize = sizeof(MII);
	MII.fMask = MIIM_FTYPE;
	MII.fType = MFT_RADIOCHECK;
	BOOL bRet = SetMenuItemInfo(g_hLangMenu,LOWORD(wParam),FALSE,&MII);
	DWORD dwError = GetLastError();*/


	delete [] pCL->pMenuString;
	delete [] pCL;

	CloseHandle(g_hChangeLangHandle);
	g_hChangeLangHandle = NULL;

	return 1;
}



















