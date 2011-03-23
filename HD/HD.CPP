/*************************************************************************

  Dongdapeng All rights reserved.
  http://kinghome.51.net
  mailto:dongdapeng8@sohu.com
  Since 2003.4.6 or 2002.12

**************************************************************************/
#include "stdafx.h"
//Add for support ATL then use flash control

#include <atlbase.h>

// Required ATL module
CComModule _Module;

#include <atlcom.h>
#include <atlwin.h>
#include <atlhost.h>
#include <afxres.h>
#include <oledlg.h>
#include <crtdbg.h>

#include "winnls.h"
#include "windows.h"
#include "resource.h"
#include "shellapi.h"
#include "string.h"
#include "../hooklib.h"
#include "hd.h"
#include "update.h"
#include "commctrl.h"
#include "process.h"
#include "prsht.h"
#include "windowsx.h"  
#include "richedit.h"
#include "mmsystem.h"
#include "stdio.h"
#include "shtypes.h"
#include "shlobj.h"
#include "shobjidl.h" 
#include "shlwapi.h"  
#include "objidl.h"  
#include "tlhelp32.h"
#include "winsock2.h"
#include "wininet.h"
#include "objbase.h"
#include "ProcessData.h"

















using namespace std;



//////////////////define variables///////////////////////////////////////////////////////////////////
HINSTANCE hInst;
HWND hMainDialog,hAboutDialog,hOptionDialog,hComWindows,hComSelf,hKeyboard,hKeySelf,hToolTip,hListView;
HWND hRestAll,hAds,hMouse,hStatic_Update,hButton_Update,hRadio_ShowAll,hDlg_Other,hCombo_FunctionKey;
TTTOOLINFOA ti;
MSG Msg;
NOTIFYICONDATA nID;
HMENU hMainMenu ,hTrayMenu,hMainWndMenu,hFileMenu;
POINT CurMouPos;
int TrayFlag,iTotalSelected,bReserveOperation = 0;
ATOM aHideWindowsHotKey,aShowSelf,aFuncKey;
char * cWndText ,cItemString[80],cRese[] = "{[保留]}",cKeyText[80] = "Ctrl + ";
int  iSelectedItems [100],iA1[100];
char cTitle [80][80],cTitle2 [80][80],cPIDs[80][80],cStrings[30][5],*cPath_QuickLaunch,cOrgCode[20];
char cCode[4][5] = {"dddd", "dddd", "dddd" ,"dddd"};
int iTemp,iResTemp ,i,bFir,bRef = 1,iForSW,iForSWC,iBaSW,iBaSWC;
char * d,cHT,cRegKey[100],*cCurrentPath,*cWorkingDic,*cSelWndTitle;
HANDLE hOptionFile;DWORD FileSizeHigh,FileSizeLow,hProcessSnap,hProcessSnap2;
PROCESSENTRY32 pe32,pe322;
DWORD lWritten,lReaded;char lpBuff [10];
WndAndHandle * sW,sWC[100];
//option dialog varialbes///////////////////////////////////////////////////
int iBuff[10];
DWORD dwPathSize=0;
int bUseMouse,iRefInterval,iKorS,iForState;
UINT  iHideWindows ,iShowSelf ;
HHOOK hGM;
LVITEM lvItem ;
char * pszText;
char * cOptionPacket,*cShortcutName,*cMBShortcutName;
OptionFile sOP;
IShellLink * pShellLink;
IPersistFile * ppf;
HKEY hRegAutoRun;
LPITEMIDLIST lpItemIdList;
HRESULT hr;
HWND hEdit_Password = NULL ,hEdit_Validate,hCheck_UsePassword;

//Register dialog////////////////////////////////////////////////////////////////
HWND hEdit_MiddleCode,hEdit_RC[5];
GUID hd_guid,hd_guid2;
HWND hRegDialog;



/////////////////////////////////////////////////////
HWND hEmail,hHttp,hEditCre;
RECT rMail,rHttp,rAds;
long xP,yP;
POINT pMouse;
HCURSOR hHand,hNorm;
HICON hI1,hI2;
HBRUSH wbr = CreateSolidBrush(RGB(255,255,255));
WndParam sWP;
HDC hHttpDC,hEmailDC;

//NetworkThread variables////////////////////////////////////////////////////////////////
HANDLE hNetworkThread;
SOCKET sock_Scan;
char * cRequestHeader,*cResponseHeader;
WSADATA wsd;
char * cObject = NULL,*cHostName =NULL,*cValue = NULL,*cRecvBuff = NULL,*pVerBuff = NULL;
long lHeaderLength = 0;

//Tray icon operation///////////////////////////////////////////////////////////////////////
HWND hTrayWnd;
TRAYDATA arrTray[30];
HIMAGELIST hIconList;

//BK Operation////////////////////////////////////////////////////////////////////////////////
HWND hList_SelWI,hList_Confirm,hList_BK,hStatic_BKG,hButton_ShowBK,hHideMain,hHideIcon;
WndAndHandle * sBKSel,**sBKWI,*sBK_All, *pWH, *pNoTitle;
int iW_Sel,iTotalBK,iBK_All,iNoTitle;
char * bkText;
HIMAGELIST hImage_BK,hImage_Sel,hImage_Confirm;
HICON hIcon_Unknown,hIcon_NoIcon;



///define contans
#define WM_MY_TRAY_NOTIFICATION (WM_USER + 101)
#define INFO_DISPLAY WM_USER + 108





//define functions

INT_PTR CALLBACK MainProc1(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK AboutProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK HotKeyProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK PasswordProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK BKProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK OtherProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK ReadMeProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK HistoryProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK RegisterProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK SelWndIconProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK FinishProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK BeginBKProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);







BOOL CALLBACK EnumWindowsProc(HWND,LPARAM);
BOOL CALLBACK EnumWindowsForBKProc(HWND,LPARAM);
VOID CALLBACK ScanWebProc(HWND,UINT,UINT_PTR,DWORD);
int FillList();
int DeleteAll();
VOID CALLBACK TimerProc(HWND,UINT,UINT_PTR,DWORD);
int CreateOptionProps();
int CreateAboutProps();
int WriteOptionFile(OptionFile);
int ReadOptionFile(OptionFile*);
int GarbageRecycle();
HWND FindTrayToolbarWindow();
int IsHaveTrayIcon(WndAndHandle * psW ,TRAYDATA * pTD);
int OpenBKWizard();
int AddSelListHeader();
int UpdateSelList();
int AddConfirmListHeader();
int UpdateConfirmList();
int HideSelected(WndAndHandle ** pWH,int iSize);
int AddBKListHeader();
int ShowSelected(WndAndHandle * pWH,int iSize);




DWORD WINAPI NetworkThread(LPVOID lp);

int HD_Connect(SOCKET sock_Scan,char *cHostName,int iPort);
int HD_FormatRequestHeader(char * cObject,char * cHostName,long * lHeaderLength,char * cRequestHeader);
int HD_SendRequest(SOCKET sock_Scan,char * cRequestHeader);
int HD_RecvResponseHeader(SOCKET sock_Scan,char * cResponseHeader,long * lHeaderLength);
int HD_GetHeaderField(char * cResponseHeader,char * cField,char * cValue);
int HD_GetState(char*cResponseHeader);
int HD_GarbageRecycle();




DWORD CALLBACK EditStreamCallback(DWORD_PTR,LPBYTE,LONG,LONG*);

LRESULT CALLBACK GetMsgProc(int ,WPARAM,LPARAM);

//Define shared variables for only one instance run.

#pragma data_seg("ForOnlyOne")
	int InstNum = 0;
	HWND hMainWindow = NULL;
#pragma data_seg()

#pragma comment(linker,"/SECTION:ForOnlyOne,RWS")






int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	InstNum ++;
	if (InstNum >1)
	{
		::ShowWindow(hMainWindow,SW_SHOWNORMAL);
		SetForegroundWindow(hMainWindow);
		//SetActiveWindow(hMainWindow);
		exit(1);
	}
	

	//Added for support ATL 
	
	  // Initialize COM
	  CoInitialize(0);

	  // Initialize the ATL module
	  _Module.Init(0, hInst);

	  // Initialize support for control containment
	  AtlAxWinInit();









	INITCOMMONCONTROLSEX icc;
	icc.dwSize   =  sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC    =  ICC_BAR_CLASSES|ICC_LISTVIEW_CLASSES|ICC_TAB_CLASSES ;

	InitCommonControlsEx(&icc);
	//CoInitialize(NULL); 
	WSAStartup(MAKEWORD(2,2),&wsd);

	//Allocate memory for R/W option file
	cOptionPacket = (char*)VirtualAlloc(NULL,sizeof(OptionFile),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	
	sW = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	sBKSel = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	sBKWI = (WndAndHandle**)VirtualAlloc(NULL,sizeof(WndAndHandle*)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	sBK_All = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pNoTitle = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);


	
	for(int i = 0;i< 100;i++)
	{
		sW[i].cWndTitle = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sW[i].cIconTip = (char *)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sBKSel[i].cWndTitle = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		//sBKWI[i].cWndTitle = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		//sBKWI[i].cIconTip = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		pNoTitle[i].cWndTitle = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		pNoTitle[i].cIconTip = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

		//sBK_All[i].cWndTitle = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		//sBK_All[i].cIconTip = (char *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);


	}
	cWndText = (char *)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pszText = (char *)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	bkText = (char *)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

	cSelWndTitle = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cRequestHeader = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cResponseHeader = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	sOP.cSerialNumber = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	sOP.cMiddleCode = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cMBShortcutName = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

	CopyFile("temp\\HDUpdate.exe","HDUpdate.exe",FALSE);
	DeleteFile("temp\\HDUpdate.exe");
	RemoveDirectory("temp");


	//Register operation variables///////////////////////////////////////////////////////////////
	HRESULT hResult;
	int iReturn = 0,i2=0;
	char cTemp[80],*cTemp2 = NULL,*cTemp3 = NULL,cTemp4[50];
	DWORD dwError = 0,dwVolumeSerialNumber=0,dwF=0,dwL=0;

	//File operation
	hOptionFile = CreateFile("Misc.hdo",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		/*OPEN_EXISTING*/OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
		if (hOptionFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL,"文件不完整，需要重新安装隐身专家!","隐身专家",MB_OK);
			dwError = GetLastError();
			exit(0);
		}
		FileSizeLow = GetFileSize(hOptionFile,&FileSizeHigh);
		if (FileSizeLow == 0)
		{
			CloseHandle(hOptionFile);

			sOP.iHideWindows = 72;
			sOP.iShowSelf = 87;
			sOP.bUseMouse = 1;
			sOP.iRefInterval = 1;
			sOP.iKorS = 'S';
			sOP.bDisableSound = 1;
			sOP.bAutoRun = 0;
			sOP.bAddQuickLaunch = 1;
			sOP.bOnTop = 0;
			sOP.iUsedTimes = 1;
			sOP.bRegistered = 0;
			sOP.bUsePassword = 0;
			strcpy(sOP.cPassword,"");
			sOP.bUseFunctionKey = 0;
			sOP.iFunctionKey = 0;
			sOP.bUseBKMouse = 0;
			sOP.bAutoHide = 0;
			sOP.bHideMain = 0;
			sOP.bHideIcon = 0;
			WriteOptionFile(sOP);
			
			/*WriteFile(hOptionFile,"H\n",2,&lWritten,NULL);
			iHideWindows = 72;iShowSelf = 87;
			WriteFile(hOptionFile,"W\n",2,&lWritten,NULL);
			lpBuff[0] = char(1);lpBuff[1] = char (10);
			WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);
			bUseMouse = lpBuff[0];
			iRefInterval = 1;
			lpBuff[0] = char (1);lpBuff[1] = char (10);
			WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);
			lpBuff [0] = 'S',lpBuff [1] = char (10);
			iKorS = 'S';
			WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);*/

		}
		else
		{
			ReadOptionFile(&sOP);
			if(sOP.iUsedTimes)
			{
				hResult = CoCreateGuid(&hd_guid);
				GetVolumeInformation("C:\\",NULL,0,&dwVolumeSerialNumber,&dwL,&dwF,NULL,0);
				memset(cOrgCode,0,20);
				
				dwVolumeSerialNumber ^= 0x98989898;				
				sprintf(cOrgCode,"%X",dwVolumeSerialNumber);
				for(i = 0;i<2;i++)
				{
					for(i2=0;i2<4;i2++)
					{
						cCode[i][i2] = cOrgCode[i*4+i2];
					}
				}
				dwVolumeSerialNumber ^= 0x68686868;
				sprintf(cOrgCode,"%X",dwVolumeSerialNumber);
				for ( i =2;i<4;i++)
				{
					for ( i2=0;i2<4;i2++)
					{
						cCode[i][i2] = cOrgCode[(i-2)*4 +i2];
					}
				}



				hd_guid.Data1 ^= 88888888;
				hd_guid.Data2 ^= 8888;
				hd_guid.Data3 ^= 8888;
				hd_guid.Data4[3] ^= 88;
				hd_guid.Data4[7] ^= 8888;
				memset(sOP.cSerialNumber,0,1024);
				memset(cTemp,0,80);
				StringFromGUID2(hd_guid,(unsigned short*)cTemp,80);
				iReturn = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
					(unsigned short*)cTemp,72,sOP.cSerialNumber,80,NULL,NULL);
				iReturn = strlen(sOP.cSerialNumber);
				cTemp2 = strchr(sOP.cSerialNumber,'{');
				cTemp2++;
				cTemp3 = strchr(sOP.cSerialNumber,'}');
				//memset(sOP.cSerialNumber,0,1024);
				memset(cTemp4,0,50);

				memcpy(cTemp4,cTemp2,cTemp3-cTemp2);
				memset(sOP.cSerialNumber,0,1024);
				strcpy(sOP.cSerialNumber,cTemp4);
				iReturn = strlen(sOP.cSerialNumber);
				//sOP.iUsedTimes ++;
				WriteOptionFile(sOP);
			}
			
			{
				sOP.iUsedTimes ++;
				WriteOptionFile(sOP);
				if(sOP.iUsedTimes >= 100 && sOP.bRegistered == 0)
				{
					iReturn = DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_REGISTER),NULL,RegisterProc);
					if (iReturn == 0)
					{
						exit(0);
					}
					
					
				}
			}

			/*ReadFile(hOptionFile,lpBuff,2,&lReaded,NULL);
			iHideWindows = lpBuff[0];
			ReadFile(hOptionFile,lpBuff,2,&lReaded,NULL);
			iShowSelf = lpBuff[0];
			//SetFilePointer(hOptionFile,2,NULL,FILE_CURRENT);
			ReadFile(hOptionFile,lpBuff,2,&lReaded,NULL);
			bUseMouse = lpBuff[0];
			ReadFile(hOptionFile,lpBuff,2,&lReaded,NULL);
			iRefInterval = lpBuff[0];
			ReadFile(hOptionFile,lpBuff,2,&lReaded,NULL);
			iKorS = lpBuff[0];*/
		}
			


	if(sOP.bUseMouse)
	{
		SetMouseHook(1);
	}


	//AutoRun and Add shortcut to Quick launch area////////////////////////////////////////////////////////////
	dwPathSize = 1024;
	cCurrentPath = (char*)VirtualAlloc(NULL,dwPathSize,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	GetModuleFileName(NULL,cCurrentPath,dwPathSize);

	strcpy(cRegKey,"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	RegOpenKey(HKEY_LOCAL_MACHINE,cRegKey,&hRegAutoRun);
	if(sOP.bAutoRun == 1)
	{
		RegSetValueEx(hRegAutoRun,"隐身专家",NULL,REG_SZ,(const BYTE*)cCurrentPath,strlen(cCurrentPath));
	}
	//Quick launch area////////////////////////////////////////////////////////////////////
	cPath_QuickLaunch = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&lpItemIdList);
	SHGetPathFromIDList(lpItemIdList,cPath_QuickLaunch);
	if(CreateFile(cPath_QuickLaunch,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL) == NULL)
	{
		//MessageBox(hwndDlg,"path not exist","隐身专家",MB_OK);
	}
	CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&pShellLink);
	pShellLink->QueryInterface(IID_IPersistFile,(void**)&ppf);
	//cCurrentPath = (char*)VirtualAlloc(NULL,dwPathSize,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	GetModuleFileName(NULL,cCurrentPath,dwPathSize);
	
	if (pShellLink->SetPath((LPCTSTR)cCurrentPath) == NOERROR)
	{
		//MessageBox(hwndDlg,"OK","OK",MB_OK);
	}
	cWorkingDic = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(cWorkingDic,0,1024);
	strcpy(cWorkingDic,cCurrentPath);
	PathRemoveFileSpec(cWorkingDic);
	if(pShellLink->SetWorkingDirectory(cWorkingDic)==NOERROR)
	{
		//MessageBox(NULL,"SETOK","OK",MB_OK);
	}
	//pShellLink->SetArguments(NULL);*/

	cShortcutName = (char*)VirtualAlloc(NULL,dwPathSize,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	strcpy(cShortcutName,cPath_QuickLaunch);
	strcat(cShortcutName,"\\Microsoft\\Internet Explorer\\Quick Launch\\");
	strcat(cShortcutName,"隐身专家 2.5");
	strcat(cShortcutName,".lnk");
	memset(cMBShortcutName,0,1024);
	MultiByteToWideChar(CP_ACP,NULL,cShortcutName,strlen(cShortcutName),(unsigned short*)cMBShortcutName,500);
	if(sOP.bAddQuickLaunch == 1)
	{
		hr = ppf->Save( (unsigned short *)cMBShortcutName,FALSE);
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	hInst = hInstance;

	hHand = LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
	hNorm = LoadCursor(NULL,IDC_ARROW);
	hIcon_Unknown = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_UNKNOWN));
	hIcon_NoIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_NOICON));

	hMainDialog = AtlAxCreateDialog(hInstance,MAKEINTRESOURCE(IDD_MAINDIALOG),NULL,(DLGPROC)MainProc1,NULL);
	hMainWindow = hMainDialog;



	hCurWnd     = hMainDialog;
	hLV = ::GetDlgItem(hCurWnd,IDC_LISTVIEW);
	hAll = ::GetDlgItem(hCurWnd,IDC_RESTORE);


	//iShowSelf = 87;
	aHideWindowsHotKey = GlobalAddAtom("HideWindowsHotKey");
	aShowSelf = GlobalAddAtom("ShowSelf");
	aFuncKey = GlobalAddAtom("FunctionKeyList");
	RegisterHotKey(hMainDialog,aHideWindowsHotKey,MOD_CONTROL,sOP.iHideWindows);
	RegisterHotKey(hMainDialog,aShowSelf,MOD_CONTROL,sOP.iShowSelf);
	if (sOP.bUseFunctionKey == 1)
	{
		RegisterHotKey(hMainDialog,aFuncKey,0,sOP.iFunctionKey);
	}

	hI1 = (struct HICON__*)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON2),IMAGE_ICON,16,16,NULL);
	hI2 = (struct HICON__*)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON3),IMAGE_ICON,16,16,NULL);

	nID.cbSize           = sizeof(NOTIFYICONDATA);
	nID.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
	nID.uID              = IDI_ICON2;
	nID.hIcon            = hI1;
	nID.hWnd             = hMainWindow;
	strcpy(nID.szTip,"隐身专家");
	nID.uCallbackMessage = WM_MY_TRAY_NOTIFICATION;
	Shell_NotifyIcon(NIM_ADD,&nID);

	
	hMainMenu = LoadMenu(hInst,MAKEINTRESOURCE(IDR_MENU1));
	hTrayMenu = GetSubMenu(hMainMenu,0);

	

	::ShowWindow(hMainDialog,SW_SHOWNORMAL);
	while(GetMessage(&Msg,NULL,0,0))
	{
		if (!IsDialogMessage(hMainDialog,&Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	
	// Terminate the ATL module
	//_Module.Term();
	// Uninitialize COM
	CoUninitialize();

	WSACleanup();


	return (Msg.wParam);

}



INT_PTR WINAPI MainProc1(HWND hwndDlg,UINT uMsg, WPARAM wParam ,LPARAM lParam)
{
	//HMENU hSysMenu;
	TOOLINFO ti;
	LPTOOLTIPTEXT lpttt;
	int idCtrl,iItem,iSelIndex=0;
	LVCOLUMN lvColumn;
	NMLISTVIEW nmListView;
	DWORD lvStyle;
	HANDLE hCurSelWnd; 
	LVITEM bkItem;
	//RECT rc;
	char cSubItem[20],cPID_local [10],cPID_Current[10];
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hMouse = ::GetDlgItem(hwndDlg,IDC_MOUSE);
		hStatic_Update = ::GetDlgItem(hwndDlg,IDC_STATIC_UPDATE);
		hButton_Update = ::GetDlgItem(hwndDlg,IDC_UPDATE);
		if(sOP.bUseMouse)
		{
			::SetWindowText(hMouse,"左键 + 右键");
		}
		else
		{
			::SetWindowText(hMouse,"禁用");
		}
		SendMessage(hwndDlg,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1)));
		hKeyboard = ::GetDlgItem(hwndDlg,IDC_KEYBOARD);
		hKeySelf = ::GetDlgItem(hwndDlg,IDC_CALLWINDOW);
		lpBuff[0] = char(sOP.iHideWindows);
		strcat(cKeyText,lpBuff);
		::SetWindowText(hKeyboard,cKeyText);
		lpBuff[0] = char (sOP.iShowSelf);
		strcpy(cKeyText,"Ctrl + ");
		strcat(cKeyText,lpBuff);
		::SetWindowText(hKeySelf,cKeyText);
		///填充ListView列
		hListView = ::GetDlgItem(hwndDlg,IDC_LISTVIEW);
		lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
		lvColumn.fmt  = LVCFMT_LEFT;
		lvColumn.cx   = 400;
		lvColumn.pszText = "窗口标题";
		ListView_InsertColumn(hListView,0,&lvColumn);
		lvColumn.cx    = 60;
		lvColumn.pszText = "进程ID";
		ListView_InsertColumn(hListView,1,&lvColumn);
		lvColumn.cx   = 80;
		lvColumn.pszText = "托盘图标";
		ListView_InsertColumn(hListView,2,&lvColumn);
		lvStyle = ListView_GetExtendedListViewStyle(hListView);
		lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyleEx(hListView,0,lvStyle);
		
		hList_BK = GetDlgItem(hwndDlg,IDC_LIST_BK);
		AddBKListHeader();

		hTrayWnd = FindTrayToolbarWindow();
		hTrayWnd_dll = hTrayWnd;
		if(hTrayWnd == NULL || !IsWindow(hTrayWnd))
		{
			MessageBox(hwndDlg,"Find traytoolbar window error","隐身专家",MB_OK);
		}
				
		FillList();
		if (sOP.iRefInterval != 0)
		{
			::SetTimer(hwndDlg,IDT_REFRESH,1000*sOP.iRefInterval,(TIMERPROC(TimerProc)));
		}
		else 
		{
			//::SetTimer(hwndDlg,IDT_REFRESH,1000,(TIMERPROC(TimerProc)));
		}
		hRestAll = ::GetDlgItem(hwndDlg,IDC_RESTORE);	
		//::EnableWindow(hRestAll,FALSE);
		hAds = ::GetDlgItem(hwndDlg,IDC_ADS);
		SetClassLong(hAds,GCL_HCURSOR,NULL);
		
		/*hSysMenu = ::GetSystemMenu(hwndDlg,FALSE);
		DeleteMenu(hSysMenu,4,MF_BYPOSITION);
		DeleteMenu(hSysMenu,0,MF_BYPOSITION);
		DeleteMenu(hSysMenu,1,MF_BYPOSITION);*/
		hToolTip = CreateWindowEx(NULL,TOOLTIPS_CLASS,NULL,WS_POPUP|TTS_ALWAYSTIP,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwndDlg,NULL,hInst,NULL);
		
		ti.cbSize     = sizeof (TOOLINFO);
		ti.hinst      = hInst;
		ti.uFlags     = TTF_IDISHWND;
		ti.hwnd       = hwndDlg;
		ti.uId        = (UINT_PTR)hAds;
		ti.lpszText   = /*"Welcome !";*/LPSTR_TEXTCALLBACK;
		SendMessage(hToolTip,TTM_ADDTOOL,0,(LPARAM)&ti);
		SendMessage(hToolTip,TTM_SETDELAYTIME,TTDT_INITIAL,MAKELONG(100,0));
		SendMessage(hToolTip,TTM_SETDELAYTIME,TTDT_AUTOPOP,MAKELONG(10000,0));
		hGM = SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,NULL,GetCurrentThreadId());
 
 
		if(sOP.bOnTop == 1)
		{  
			::SetWindowPos(hwndDlg,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			::CheckDlgButton(hwndDlg,IDC_CHECK_ONTOP,BST_CHECKED);
		}
		if(sOP.bAutoHide == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_AUTOHIDE,BST_CHECKED);
		}
		

		hIconList = ImageList_Create(16,16,ILC_COLOR16,6,6);
		ListView_SetImageList(hListView,hIconList,LVSIL_SMALL);
		//SendMessage(hwndDlg,WM_SETICON,ICON_SMALL,(LPARAM)LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON2)));


		SetTimer(hwndDlg,IDT_SCANWEB,2*1000*60/*1000*60*30*/,(TIMERPROC)ScanWebProc);
		
		hStatic_BKG = GetDlgItem(hwndDlg,IDC_STATIC_BKGROUP);
		hButton_ShowBK = GetDlgItem(hwndDlg,IDC_SHOWBK);
		EnableWindow(hButton_ShowBK,FALSE);
		
		return FALSE;
	case WM_NOTIFY:

		if(((LPNMHDR)lParam)->hwndFrom == hList_BK)
		{
			switch(((LPNMHDR)lParam)->code)
			{
			case TTN_GETDISPINFO:
				break;
			default:
				break;
			}
		}
		switch((((LPNMHDR)lParam)->code ) )
		{
		case  TTN_GETDISPINFO:
			idCtrl = ::GetDlgCtrlID((HWND)((LPNMHDR)lParam)->idFrom);
			lpttt  = (LPTOOLTIPTEXT)lParam;
			switch(idCtrl)
			{
			case IDC_ADS:
				lpttt ->lpszText = "到我的主页看看吧!";
				break;
			default:
				break;
			}
			break;
		case NM_SETFOCUS:
			bRef = 0;
			break;
		case NM_KILLFOCUS:
			bRef = 1;
			break;
		case LVN_ITEMCHANGED:
			if (iForState == 1)
			{
				break;
			}
			nmListView = *((LPNMLISTVIEW)lParam);
			iItem = ((LPNMLISTVIEW)lParam) ->iItem;
			if ( iItem >= 0)
			{
				if(nmListView.uNewState == 8192 && nmListView.uOldState != 0)
				{
					//MessageBox(hwndDlg,"ok","ok",MB_OK);
					ListView_GetItemText(hListView,iItem,0,sWP.cT[iTotalSelected],78);
					ListView_GetItemText(hListView,iItem,1,sWP.cP[iTotalSelected],78);
					iTotalSelected ++;

					//2.2 method ////////
					ListView_GetItemText(hListView,iItem,1,cPID_Current,10);
					ListView_GetItemText(hListView,iItem,0,cSelWndTitle,1024);
					for(int i2 = 0;i2 < iBaSW;i2++)
					{
						sprintf(cPID_local,"%X",sW[i2].lPID);
						if (!strcmp(cPID_local,cPID_Current))
						{
							if(strcmp(strlwr(sW[i2].cProcessName),"iexplore.exe")==0 &&
								strcmp(sW[i2].cWndTitle,cSelWndTitle)==0)
							{
								sW[i2].bReserved = 1;
								hCurSelWnd = sW[i2].hWnd;			
								
							}
							else if(strcmp(strlwr(sW[i2].cProcessName),"iexplore.exe")!=0)
							{
								sW[i2].bReserved = 1;
							}
						}
					}

					for ( i2=0;i2<iBaSW;i2++)
					{
						if (sW[i2].iWindowType==1&& ::GetParent(sW[i2].hWnd) == hCurSelWnd )
						{
							sW[i2].bReserved = 1;
						}
					}

					iTemp = iTotalSelected ;
					::EnableWindow(hRestAll,TRUE);

					ReserveWnds(sW,iBaSW);
					
				}
				if(nmListView.uNewState == 4096 && nmListView.uOldState != 0)
				{
					//MessageBox(hwndDlg,"ok","ok",MB_OK);
					ListView_GetItemText(hListView,iItem,0,pszText,1000);
					ListView_GetItemText(hListView,iItem,1,cSubItem,18);


					//2.2 method//////////////////////////////////////////////////////////
					ListView_GetItemText(hListView,iItem,1,cPID_Current,10);
					ListView_GetItemText(hListView,iItem,0,cSelWndTitle,1024);
					

					for (int i2 = 0;i2<iBaSW;i2++)
					{
						sprintf(cPID_local,"%X",sW[i2].lPID);

						if(strcmp(cPID_local,cPID_Current)== 0 && strcmp(cSelWndTitle,sW[i2].cWndTitle)==0 &&
							sW[i2].iWindowType == 0)
						{
							hCurSelWnd = sW[i2].hWnd;
						}
					}
					for(i2 = 0;i2 < iBaSW;i2++)
					{
						sprintf(cPID_local,"%X",sW[i2].lPID);

						if (::GetParent(sW[i2].hWnd) == hCurSelWnd)
						{
							sW[i2].bReserved = 0;
						}
						else if (strcmp(cPID_local,cPID_Current)==0 && strcmp(cSelWndTitle,sW[i2].cWndTitle)!=0&& 
							strcmp(strlwr(sW[i2].cProcessName),"iexplore.exe") == 0 && sW[i2].bReserved == 1 )
						{
							continue;
						}
						else if (!strcmp(cPID_local,cPID_Current))
						{
							sW[i2].bReserved = 0;
							/*for(int i3 = 0;i3 < iBaSWC;i3++)
							{
								if (sWC[i3].lPID == sW[i2].lPID)
								{
									sWC[i3].bReserved = 0;
								}
							}*/
						}

					}
					iTotalSelected = iTemp;
					ReserveWnds(sW,iBaSW);
				
				}

			}
			break;
		/*case LVN_ODSTATECHANGED:
			MessageBox(hwndDlg,"od","od",MB_OK);
			break;*/

		}
		
		return 0;


	/*case WM_SETCURSOR:
		GetWindowRect(hAds,&rAds);
		GetCursorPos(&pMouse);
		MapWindowPoints(GetDesktopWindow(),hwndDlg,(struct tagPOINT*)&rAds,2);
		ScreenToClient(hwndDlg,&pMouse);
		if (PtInRect(&rAds,pMouse))
		{
			SetCursor(hHand);
		}
		else
		{
			SetCursor(hNorm);
		}
		return TRUE;*/ 
 
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			ShowWindow(hwndDlg,SW_HIDE);
			return FALSE;
		}
		return 0;
	case WM_HOTKEY:
		if ((HIWORD(lParam) == sOP.iHideWindows && LOWORD(lParam) == MOD_CONTROL)||HIWORD(lParam) == sOP.iFunctionKey)
		{
			MyHideWindows();
			//MessageBox(NULL,"OK","OK",MB_OK);
			return 1;
		}
		if (HIWORD(lParam) == sOP.iShowSelf && LOWORD(lParam) == MOD_CONTROL)
		{
			SendMessage(hMainDialog,WM_COMMAND,IDM_SHOWWINDOW,0);
			return 1;
		}
		return 1;



	case WM_MY_TRAY_NOTIFICATION:
		switch(lParam)
		{
		case WM_RBUTTONDOWN:
			SetMenuDefaultItem(hTrayMenu,0,TRUE);
			SetForegroundWindow(hwndDlg);
			GetCursorPos(&CurMouPos);
			if (::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE)
			{
				ModifyMenu(hTrayMenu,0,MF_BYPOSITION,IDM_SHOWWINDOW,"隐藏主窗口(\&W)");
			}
			else if(!(::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE))
			{
				ModifyMenu(hTrayMenu,0,MF_BYPOSITION,IDM_SHOWWINDOW,"显示主窗口(\&W)");
			}
			if (::IsWindow(hAboutDialog)|| ::IsWindow(hOptionDialog)|| ::IsWindow(hRegDialog))
			{
				EnableMenuItem(hTrayMenu,IDM_ABOUT,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_OPTION,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_SHOWWINDOW,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_REGISTER,MF_DISABLED|MF_GRAYED);

			}
			else if (!::IsWindow(hAboutDialog)||!::IsWindow(hOptionDialog))
			{
				EnableMenuItem(hTrayMenu,IDM_ABOUT,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_OPTION,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_SHOWWINDOW,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_REGISTER,MF_ENABLED);

			}

			if (sOP.bRegistered == 1)
			{
				EnableMenuItem(hTrayMenu,IDM_REGISTER,MF_DISABLED|MF_GRAYED);
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_ONTOP) == BST_CHECKED)
			{
				CheckMenuItem(hTrayMenu,IDM_TRAYONTOP,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hTrayMenu,IDM_TRAYONTOP,MF_BYCOMMAND|MF_UNCHECKED);
			}


			TrackPopupMenu(hTrayMenu,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
			break;
		case WM_LBUTTONDOWN:
			if(GetActiveWindow() != hwndDlg && IsWindowVisible(hMainDialog))
			{
				SetForegroundWindow(hwndDlg);
			}
			else
			{
				SendMessage(hwndDlg,WM_COMMAND,IDM_SHOWWINDOW,0);
			}

			/*else if(IsWindowVisible(hMainDialog))
			{
				SendMessage(hwndDlg,WM_SYSCOMMAND,SC_MINIMIZE,0);
			}
			else if(!IsWindowVisible(hMainDialog))
			{
				ShowWindow(hwndDlg,SW_MINIMIZE);
				SendMessage(hwndDlg,WM_SYSCOMMAND,SC_RESTORE,0);
				SetForegroundWindow(hwndDlg);				
			}*/
			break;
		default:
			return FALSE;
		}
		return FALSE;
	case WM_INITMENUPOPUP:
		hMainWndMenu = GetMenu(hwndDlg);
		hFileMenu = GetSubMenu(hMainWndMenu,0);

		if(LOWORD(lParam) == 0)
		{
			if(TrayFlag == 1)
			{
				ModifyMenu(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND,IDM_HIDEICON,"显示图标(\&I)");
			}
			else
			{
				ModifyMenu(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND,IDM_HIDEICON,"隐藏图标(\&I)");
			}

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_ONTOP) == BST_CHECKED)
			{
				CheckMenuItem(hFileMenu,IDM_ONTOP,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hFileMenu,IDM_ONTOP,MF_BYCOMMAND|MF_UNCHECKED);
			}
			return 0;
		}
		
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_SHOWWINDOW:
			if(!(::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE))
			{
				::ShowWindow(hwndDlg,SW_MINIMIZE);
				SendMessage(hwndDlg,WM_SYSCOMMAND,SC_RESTORE,0);
				SetForegroundWindow(hwndDlg);
			}
			else if (::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE)
			{
				SendMessage(hwndDlg,WM_SYSCOMMAND,SC_MINIMIZE,0);
				//::ShowWindow(hwndDlg,SW_HIDE);
			}


			break;
		case IDC_UPDATE:
			//hNetworkThread = (HANDLE)_beginthreadex(NULL,0,(unsigned int(__stdcall*)(void*))NetworkThread,NULL,0,NULL);
			SendMessage(hwndDlg,WM_COMMAND,MAKEWPARAM(IDM_UPDATE,0),0);
			::ShowWindow(hStatic_Update,SW_HIDE);
			::ShowWindow(hButton_Update,SW_HIDE);
			break;
		case IDC_HIDEICON:
			if (TrayFlag == 0)
			{
				Shell_NotifyIcon(NIM_DELETE,&nID);
				::SetWindowText(::GetDlgItem(hwndDlg,IDC_HIDEICON),"显示图标(\&I)");
				TrayFlag = 1;
			}
			else if (TrayFlag == 1)
			{
				TrayFlag = 0;
				Shell_NotifyIcon(NIM_ADD,&nID);
				::SetWindowText(::GetDlgItem(hwndDlg,IDC_HIDEICON),"隐藏图标(\&I)");
			}

			break;
		case IDC_OPTION:
			CreateOptionProps();
			
			break;
		case IDM_OPTION:
			CreateOptionProps();
			break;
		case IDM_HIDEICON:
			SendMessage(hwndDlg,WM_COMMAND,IDC_HIDEICON,NULL);
			break;
		case IDM_UPDATE:
			ShellExecute(NULL,"open","HDUpdate.exe",NULL,NULL,SW_SHOWNORMAL);
			break;
		case IDM_EXIT:
			SendMessage(hwndDlg,WM_COMMAND,IDCANCEL,0);
			break;
		case IDM_ABOUT:
			SendMessage(hwndDlg,WM_COMMAND,IDC_ABOUT,0);
			break;
		case IDC_ABOUT:
			CreateAboutProps();

			/*hAboutDialog = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOGABOUT),hwndDlg,(DLGPROC)AboutProc);
			::ShowWindow(hAboutDialog,SW_SHOW);*/
			EnableMenuItem(hTrayMenu,IDM_ABOUT,MF_DISABLED);
			break;


		case IDC_EXIT:
			SendMessage(hwndDlg,WM_COMMAND,IDCANCEL,0);
			break;
		case IDC_REFRESH:
			DeleteAll();
			FillList();
			break;
		case IDC_RESTORE:
			/*i = iTotalSelected;
			iTotalSelected = iTemp = 0;
			while(i >= 0)
			{
				strcpy(sWP.cT[i],"");
				i--;
			}     
			i = 0;
			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			ClearAll();*/
			//::EnableWindow(hRestAll,FALSE);

			//2.2 method//////////////////////
			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			for(i = 0;i<iBaSW;i++)
			{
				sW[i].bReserved = 0;
			}
			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			break;

   

		case IDCANCEL:
			RestoreAllWindows(sOP.iKorS);
			Shell_NotifyIcon(NIM_DELETE,&nID);
			if(sOP.bUseMouse)
			{
				SetMouseHook(0);
			}
			DeleteObject(wbr);
			CloseHandle(hOptionFile);

			GarbageRecycle();
			PostQuitMessage(1);
			break;
		/*case IDC_WINDOWSLIST:
			if (HIWORD(wParam) == LBN_SETFOCUS)
			{
				bRef = 0;
			}
			if (HIWORD(wParam) == LBN_KILLFOCUS)
			{
				bRef = 1;
			}

			break;*/
		case IDC_ADS:
			if(HIWORD(wParam) == STN_CLICKED)
			{
				ShellExecute(hwndDlg,"Open","http://kinghome.51.net",NULL,NULL,SW_SHOWMAXIMIZED);
			}
			break;
		case IDC_CHECK_ONTOP:
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECK_ONTOP) == BST_CHECKED)
			{
                ::SetWindowPos(hwndDlg,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				sOP.bOnTop = 1;
				
			}
			else
			{
				::SetWindowPos(hwndDlg,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				sOP.bOnTop = 0;
			}
			WriteOptionFile(sOP);
			break;
		case IDM_ONTOP:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_ONTOP) == BST_CHECKED)
			{
				CheckDlgButton(hwndDlg,IDC_CHECK_ONTOP,BST_UNCHECKED);
			}
			else
			{
				CheckDlgButton(hwndDlg,IDC_CHECK_ONTOP,BST_CHECKED);
			}
			SendMessage(hwndDlg,WM_COMMAND,IDC_CHECK_ONTOP,0);
			break;
		case IDM_TRAYONTOP:
			SendMessage(hwndDlg,WM_COMMAND,IDM_ONTOP,NULL);
			break;
		case IDC_BKWIZARD:
			//MessageBox(hMainDialog,"You click the bkwizard button","hidedragon",MB_OK);
			OpenBKWizard();
			break;
		case IDC_SHOWBK:
			if (ListView_GetSelectedCount(hList_BK) == 0)
			{
				MessageBox(hMainDialog,"请选择一个要显示的条目","隐身专家",MB_OK|MB_ICONINFORMATION);
				break;
			}
			else
			{
				iSelIndex = ListView_GetSelectionMark(hList_BK);
				bkItem.iItem = iSelIndex;
				ListView_GetItem(hList_BK,&bkItem);
				ShowSelected((WndAndHandle*)bkItem.lParam,1);
				ListView_DeleteItem(hList_BK,iSelIndex);
				
			}
			break;
		case IDC_CHECK_AUTOHIDE:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTOHIDE) == BST_CHECKED)
			{
				sOP.bAutoHide = 1;
			}
			else
			{
				sOP.bAutoHide = 0;
			}
			WriteOptionFile(sOP);
			break;
		case IDM_SUGGEST:
			ShellExecute(NULL,"open","http://kinghome.51.net/cn/notbook/notbook.php",NULL,NULL,SW_SHOW);
			break;
		case IDM_USERMANUAL:
			ShellExecute(NULL,"open","http://kinghome.51.net/cn/ttt2/Frameset.htm",NULL,NULL,SW_SHOW);
			break;
		case IDM_REGISTER:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_REGISTER),hMainDialog,RegisterProc);
			break;

		default:
			return FALSE;
		}

	default:
		return FALSE;
	}
}


INT_PTR CALLBACK AboutProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	char sContentCredit [] = "如果你有任何的建议或问题请给我发电子邮件，我会在下一版时改进。\r\n\r\n"
							"欢迎您注册，注册后您将获得一年的免费升级和技术支持。\r\n\r\n"
							"你可自由的复制和传播该程序，但要用于商业用途需经作者书面同意。";
	static HBRUSH bmenu = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	static int iForLink = 1,iForIn = 0,iForOut = 0;
	TOOLINFO ti;
	LPTOOLTIPTEXT lpttt;
	int idCtrl;
	HWND hEditQQ = NULL;
	HDC hScreenDC;
	int iX = 0,iY = 0;
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hAboutDialog = ::GetParent(hwndDlg);
		if(!IsWindowVisible(hMainDialog))
		{
			hScreenDC = GetDC(NULL);
			iX = GetDeviceCaps(hScreenDC,HORZRES)/4;
			iY = GetDeviceCaps(hScreenDC,VERTRES)/8;
			SetWindowPos(hAboutDialog,NULL,iX,iY,0,0,SWP_NOSIZE|SWP_NOZORDER);
			ReleaseDC(NULL,hScreenDC);
		}

		hEmail = ::GetDlgItem(hwndDlg,IDC_STATICEMAIL);
		hHttp = ::GetDlgItem(hwndDlg,IDC_STATICHTTP);
		hEditCre = ::GetDlgItem(hwndDlg,IDC_EDITCREDIT);
		hEditQQ = GetDlgItem(hwndDlg,IDC_EDIT_QQ);
		SendMessage(hEditCre,WM_SETTEXT,0,(LPARAM)sContentCredit);
		SetClassLong(hEmail,GCL_HCURSOR,(long)NULL );
		SetClassLong(hHttp,GCL_HCURSOR,(long)NULL);
		SetFocus(hEditCre);
		hHttpDC = GetDC(hHttp);
		hEmailDC = GetDC(hEmail);

		ti.cbSize     = sizeof (TOOLINFO);
		ti.hinst      = hInst;
		ti.uFlags     = TTF_IDISHWND;
		ti.hwnd       = hwndDlg;
		ti.uId        = (UINT_PTR)hHttp;
		ti.lpszText   = /*"Welcome !";*/LPSTR_TEXTCALLBACK;
		SendMessage(hToolTip,TTM_ADDTOOL,0,(LPARAM)&ti);
		ti.uId        = (UINT_PTR)hEmail;
		SendMessage(hToolTip,TTM_ADDTOOL,0,(LPARAM)&ti);
		SetWindowText(hEditQQ,"297500820");

		
		
		return FALSE;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hEditCre)
		{
			return (LRESULT)wbr;
		}
		if((HWND)lParam == hHttp||(HWND)lParam == hEmail)
		{
			if (iForLink == 1)
			{
				if (SetTextColor((HDC)(wParam),RGB(33,3,255)) == CLR_INVALID)
				{
					
				}
			}
			else
			{
				SetTextColor((HDC)(wParam),RGB(255,0,0));
			}
			SetBkColor((HDC)(wParam),GetSysColor(COLOR_3DFACE));
			
			return (LRESULT)bmenu;
		}

		SetBkColor((HDC)(wParam),GetSysColor(COLOR_3DFACE));
		return (LRESULT)bmenu;
		
	case WM_CTLCOLORDLG:
		return (LRESULT)bmenu;
	case WM_SETCURSOR:
		GetWindowRect(hEmail,&rMail);
		GetWindowRect(hHttp,&rHttp);
		MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rMail,2);	
		MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rHttp,2);
		GetCursorPos(&pMouse);
		ScreenToClient(hwndDlg,&pMouse);
		if (PtInRect(&rHttp,pMouse)||PtInRect(&rMail,pMouse))
		{	
			iForOut = 0;
			SetCursor(hHand);
			iForLink = 0;
			//SendMessage(hwndDlg,WM_CTLCOLORSTATIC,(WPARAM)hHttpDC,(LPARAM)hHttp);
			if ((++iForIn)  <= 1)
			{
				if(PtInRect(&rHttp,pMouse))
					InvalidateRect(hHttp,NULL,FALSE);
				else
					InvalidateRect(hEmail,NULL,FALSE);
			}
		}
		else
		{
			iForIn = 0;
			iForLink = 1;
			SetCursor(hNorm);
			SendMessage(hwndDlg,WM_CTLCOLORSTATIC,(WPARAM)hHttpDC,(LPARAM)hHttp);
			if((++iForOut) <= 1)
			{
					InvalidateRect(hEmail,NULL,FALSE);
					InvalidateRect(hHttp,NULL,FALSE);
			}

		}

		return TRUE;
		break;
	case WM_NOTIFY:
		switch(((LPNMHDR)(lParam))->code)
		{
		case PSN_SETACTIVE:
			PostMessage(hwndDlg,INFO_DISPLAY,0,0);
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
		case  TTN_GETDISPINFO:
			idCtrl = ::GetDlgCtrlID((HWND)((LPNMHDR)lParam)->idFrom);
			lpttt  = (LPTOOLTIPTEXT)lParam;
			switch(idCtrl)
			{
			case IDC_STATICHTTP:
				lpttt ->lpszText = "获取隐身专家最新信息";
				break;
			case IDC_STATICEMAIL:
				lpttt ->lpszText = "反馈意见和建议";
			default:
				return FALSE;
			}
			break;
		default :
			return 0;
		}
		return 0;
	case INFO_DISPLAY:
		SetFocus(hEditCre);
		return TRUE;
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_STATICEMAIL:
				if (HIWORD(wParam) == STN_CLICKED)
				{
					//MessageBox(hwndDlg,"ok","ok",MB_OK);
					ShellExecute(hwndDlg,NULL,"mailto:dongdapeng8@sohu.com",NULL,NULL,SW_SHOWMAXIMIZED);
				}
				break;
			case IDC_STATICHTTP:
				if(HIWORD(wParam) == STN_CLICKED)
				{
					//MessageBox(hwndDlg,"ok","ok",MB_OK);
					ShellExecute(hwndDlg,"Open","http://kinghome.51.net",NULL,NULL,SW_SHOWMAXIMIZED);
				}
				break;

			
			default:
				return FALSE;
		}
	


	default:
		return FALSE;
	}
}


int FillList()
{
	iBaSW = iForSW;
	iBaSWC = iForSWC;
	int j = 0;
	//2.2 method/////////////////////////
	for(int i = 0;i < iBaSW;i++)
	{
		sW[i].bExist = 0;
	}

	ImageList_RemoveAll(hIconList);

	EnumWindows(EnumWindowsProc,0);


	for( i=0;i<iBaSW-1;i++)
	{
		for(int j = 0;j <iBaSW-1-i;j++)
		{
			if (sW[j].bExist ==0 && sW[j].bHide != 1)
			{
				sW[j].lPID = sW[j+1].lPID;
				sW[j].hWnd = sW[j+1].hWnd;
	    			sW[j].iWindowType = sW[j+1].iWindowType;
				sW[j].bExist = sW[j+1].bExist;
				sW[j].bReserved = sW[j+1].bReserved;
				strcpy(sW[j].cWndTitle,sW[j+1].cWndTitle);
				sW[j].bHide = sW[j+1].bHide;
				strcpy(sW[j].cProcessName,sW[j+1].cProcessName);
				sW[j+1].bExist = 0;
				sW[j].iHasTrayIcon = sW[j+1].iHasTrayIcon;
				sW[j].iIconIndex = sW[j+1].iIconIndex;
				sW[j].trayicon = sW[j+1].trayicon;
				memset(sW[j].cIconTip,0,1022);
				strcpy(sW[j].cIconTip,sW[j+1].cIconTip);
			}
		}
	}


	int i2 = 0;
	for(i2 = 0;sW[i2].bExist != 0||sW[i2].bHide == 1;i2++)
	{
		
	}
	iBaSW = iForSW= i2;
	ReserveWnds(sW,iBaSW); 

	return 1;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
	char  cPID[10] ;
	//char cClassName[200];
	HWND hOnw;
	DWORD lPID;
	int i1 = 0,iFlag = 0,iconIndex = 0,imageindex = 0,iItemCount = 0;
	TRAYDATA * pTrayinfo = NULL;
	WndAndHandle  temp_SW,*popWnd = NULL;
	HICON hMainIcon;
	LVITEM bkItem;
	if (::GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{	
		GetWindowText(hwnd,cWndText,1024);
		GetWindowThreadProcessId(hwnd,&lPID);
		if (!strcmp(cWndText,"")||!strcmp(cWndText,"Program Manager"))
			return 1;
		if (hwnd == hMainDialog||::GetParent(hwnd) == hMainDialog)
		{
			return 1;
		}
		
		
		//Auto hide background window's popup window
		if (sOP.bUseBKMouse == 1)
		{
			iItemCount = ListView_GetItemCount(hList_BK);
			for (i = 0;i<iItemCount;i++)
			{
				popWnd = (WndAndHandle *)VirtualAlloc(NULL,sizeof(WndAndHandle),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				popWnd->cWndTitle  = (char*) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				popWnd->cIconTip = (char*) VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				bkItem.iItem = i;
				ListView_GetItem(hList_BK,&bkItem);
				if(((WndAndHandle*)bkItem.lParam)->lPID == lPID)
				{
					ShowWindow(hwnd,SW_HIDE);
					bkItem.mask = LVIF_PARAM;
					bkItem.iSubItem = 0;
					popWnd->hWnd = hwnd;
					popWnd->lPID = lPID;
					popWnd->Next = NULL;
					((WndAndHandle*)bkItem.lParam)->Next = popWnd;
					ListView_SetItem(hList_BK,&bkItem);
				}
			}
		}




		//hMainIcon = (HICON)PostMessage(hwnd,WM_GETICON,ICON_SMALL,NULL);
		//if(hMainIcon == NULL)
		//{
			hMainIcon = (HICON)GetClassLongPtr(hwnd,GCLP_HICONSM);
		//}

		//////////////////////////////////////////////////////////////////////////////////////////
		
		lvItem.mask = LVIF_TEXT|LVIF_IMAGE;	
		lvItem.pszText = pszText;
		
		strcpy(lvItem.pszText,cWndText);
		lvItem.iItem = SendMessage(hListView,LVM_GETITEMCOUNT,0,0);
		lvItem.iSubItem = 0;

		//2.2 method////////
		for(i1 = 0;i1<iBaSW;i1++)
		{
			if( lPID == sW[i1].lPID &&sW[i1].bReserved == 1&&
				strcmp(strlwr(sW[i1].cProcessName),"iexplore.exe")==0 && hwnd != sW[i1].hWnd/*strcmp(sW[i1].cWndTitle,cWndText)*/
				&&sW[i1].iWindowType == 0)
			{
				if (::GetParent(hwnd) != sW[i1].hWnd)
				{
					continue;
				}
				for (int i2 = 0;i2<iBaSW;i2++)
				{
					if(hwnd == sW[i2].hWnd && sW[i2].bReserved == 1 && sW[i2].iWindowType == 0)
					{
						iFlag = 1;
						break;
					}
				}
				if(iFlag == 1)
				{
					iFlag = 0;
					//i1+=iBaSW;

					//break;
				}
				else if(::GetParent(hwnd) == sW[i1].hWnd)
				{
					for(int i2 = 0;i2<iBaSW;i2++)
					{
						if (hwnd==sW[i2].hWnd)
						{
							sW[i2].bReserved = 1;
							//i1 += iBaSW;
							break;
						}
					}
					break;
				}
				else
				{
					i1+=iBaSW;
				}
				
			}
			else if(lPID == sW[i1].lPID && sW[i1].bReserved == 1 && hwnd != sW[i1].hWnd && sW[i1].iWindowType == 0)
			{
				strcpy(sW[iForSW].cWndTitle,cWndText);
				sW[iForSW].lPID = lPID;
				sW[iForSW].hWnd = hwnd;
				sW[iForSW].bExist = 1;
				sW[iForSW].iWindowType = 0;
				sW[iForSW].bReserved = 1;
				sW[iForSW].bHide = 0;
				

				if (::GetParent(hwnd) != NULL)
				{
					sW[iForSW].iWindowType = 1;
					iForSW ++;
					iBaSW = iForSW;
					

					return 1;
				}
				imageindex = ImageList_AddIcon(hIconList,hMainIcon);
				lvItem.iImage = imageindex;
				lvItem.pszText = pszText;
				strcpy(lvItem.pszText,sW[iForSW].cWndTitle);
				ListView_InsertItem(hListView,&lvItem);
				sprintf(cPID,"%X",sW[iForSW].lPID);

				strcpy(lvItem.pszText ,cPID);
				lvItem.iSubItem = 1;
				ListView_SetItem(hListView,&lvItem);
				ZeroMemory(cPID,10);

				pTrayinfo = NULL;
				iconIndex = IsHaveTrayIcon(&sW[iForSW],pTrayinfo);
				if (iconIndex != 0 || pTrayinfo != NULL)
				{
					sW[iForSW].iIconIndex =  iconIndex;
					sW[iForSW].trayicon =  arrTray[iconIndex];
					sW[iForSW].iHasTrayIcon = 1;

					lvItem.iSubItem = 2;
					strcpy(lvItem.pszText,"have");
					ListView_SetItem(hListView,&lvItem);
				}

				iForState = 1;
				ListView_SetCheckState(hListView,lvItem.iItem,TRUE);
				iForState = 0;

				iForSW ++;
				iBaSW = iForSW;

				return 1;
			}


			if (hwnd == sW[i1].hWnd || sW[i1].bHide == 1)
			{
				sW[i1].bExist = 1;
				if(hwnd == sW[i1].hWnd && sW[i1].bReserved == 0)// && ::GetParent(hwnd)==NULL)
				{
					strcpy(sW[i1].cWndTitle,cWndText);//For window titile change,ues the new title///////////////////////
					break;

				}else if (hwnd == sW[i1].hWnd && sW[i1].bReserved == 1)
				{
					strcpy(sW[i1].cWndTitle,cWndText);
					if (sW[i1].iWindowType == 1)
					{
						return 1;
					}
					imageindex = ImageList_AddIcon(hIconList,hMainIcon);
					lvItem.iImage = imageindex;
					lvItem.pszText = pszText;
					strcpy(lvItem.pszText,sW[i1].cWndTitle);
					ListView_InsertItem(hListView,&lvItem);
					sprintf(cPID,"%X",sW[i1].lPID);

					strcpy(lvItem.pszText,cPID);
					lvItem.iSubItem = 1;
					ListView_SetItem(hListView,&lvItem);
					ZeroMemory(cPID,10);

					
					pTrayinfo = NULL;
					iconIndex = IsHaveTrayIcon(&sW[i1],pTrayinfo);
					if (iconIndex >= 0 )
					{
						sW[i1].iIconIndex =  iconIndex;
						sW[i1].trayicon = arrTray[iconIndex];
						sW[i1].iHasTrayIcon = 1;
						//imageindex = ImageList_AddIcon(hIconList,sW[i1].trayicon.hIcon);

						//lvItem.mask = LVIF_IMAGE|LVIF_TEXT;
						lvItem.iSubItem = 2;
						//lvItem.iImage = imageindex;
						strcpy(lvItem.pszText,"have");
						ListView_SetItem(hListView,&lvItem);
					}


					iForState = 1;
					ListView_SetCheckState(hListView,lvItem.iItem,TRUE);
					iForState = 0;
					return 1;
				}
			}
		}
		////////For two same windows example VC7////////////////////////////////////////////////////////////////////
		/*if (!strcmp(cWndText,sW[iForSW-1].cWndTitle) && lPID == sW[iForSW-1].lPID)
		{
			return 1;
		}*/

		if(i1>=iBaSW)
		{
			strcpy(sW[iForSW].cWndTitle,cWndText);
			sW[iForSW].lPID = lPID;


			hProcessSnap = (unsigned long)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
			pe32.dwSize = sizeof(PROCESSENTRY32);
			Process32First((void*)hProcessSnap,&pe32);
			while(Process32Next((void*)hProcessSnap,&pe32))
			{
				if(pe32.th32ProcessID == lPID)
				{
					if(strcmp(strlwr(pe32.szExeFile),"iexplore.exe")==0)
					{
						strcpy(sW[iForSW].cProcessName,pe32.szExeFile);
						//MessageBox(NULL,"IE","HD",MB_OK);
					}
					else
					{
						strcpy(sW[iForSW].cProcessName,"");
					}					
				}
				
			}


			sW[iForSW].hWnd = hwnd;
			sW[iForSW].bExist = 1;
			sW[iForSW].iWindowType = 0;
			sW[iForSW].bReserved = 0;
			sW[iForSW].bHide = 0;

			pTrayinfo = NULL;
			iconIndex = IsHaveTrayIcon(&sW[iForSW],pTrayinfo);
			if (iconIndex >= 0 )
			{
				sW[iForSW].iIconIndex =  iconIndex;
				sW[iForSW].trayicon = arrTray[iconIndex];
				sW[iForSW].iHasTrayIcon = 1;
				

			}


			iForSW ++;
			iBaSW = iForSW;
			hOnw = ::GetParent(hwnd);
			if(hOnw !=NULL)
			{

				//2.2 method//////
				sW[iForSW-1].iWindowType = 1;
				return 1;
			}
		}

		hOnw = ::GetParent(hwnd);
		if(hOnw !=NULL)
		{
			return 1;
		}
		
		imageindex = ImageList_AddIcon(hIconList,hMainIcon);
		lvItem.iImage = imageindex;
		//lvItem.stateMask = LVIS_SELECTED;
				
		ListView_InsertItem(hListView,&lvItem);

		sprintf(cPID,"%X",lPID);
		strcpy(lvItem.pszText,cPID);
		lvItem.iSubItem = 1;
		ListView_SetItem(hListView,&lvItem);
		ZeroMemory(cPID,10);

		pTrayinfo = NULL;
		temp_SW.lPID = lPID;
		temp_SW.cIconTip = NULL;
		iconIndex = IsHaveTrayIcon(&temp_SW,pTrayinfo);
		if (iconIndex >= 0 )
		{
			lvItem.iSubItem = 2;
			strcpy(lvItem.pszText,"have");
			ListView_SetItem(hListView,&lvItem);
		}
	}
	return 1;
}

int DeleteAll()
{
	ListView_DeleteAllItems(hListView);
	return 1;
}

INT_PTR CALLBACK HotKeyProc(HWND hwndDlg, UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	int iSelInd = 0,iCurSel = 0,iCurSel2 = 0,iCurSel3 = 0;
	 char cItemText[3],cHK = NULL,cConText[80] = "Ctrl + ";
	BOOL bSuc;
	HDC hScreenDC;
	int fX = 0,fY = 0;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hOptionDialog = ::GetParent(hwndDlg);
		if(!IsWindowVisible(hMainDialog))
		{
			hScreenDC = GetDC(NULL);
			fX = GetDeviceCaps(hScreenDC,HORZRES)/4;
			fY = GetDeviceCaps(hScreenDC,VERTRES)/8;
			SetWindowPos(hOptionDialog,NULL,fX,fY,0,0,SWP_NOSIZE|SWP_NOZORDER);
			ReleaseDC(NULL,hScreenDC);
		}
		hComWindows = ::GetDlgItem(hwndDlg,IDC_COMBOWINDOWS);
		hComSelf = ::GetDlgItem(hwndDlg,IDC_COMBOSELF);
		hCombo_FunctionKey = GetDlgItem(hwndDlg,IDC_COMBO_FUNCTIONKEY);
		while(iSelInd < 16)
		{
			iSelInd ++;
			LoadString(hInst,iSelInd,cStrings[iSelInd],3);
			if ((unsigned int)(cHK = cStrings[iSelInd][0]) == sOP.iHideWindows)
			{
				iCurSel = iSelInd;
			}
			if((unsigned int )(cHK = cStrings[iSelInd][0]) == sOP.iShowSelf)
			{
				iCurSel2 = iSelInd;
			}

			SendMessage(hComWindows,CB_ADDSTRING,0,(LPARAM)cStrings[iSelInd]);
			SendMessage(hComSelf,CB_ADDSTRING,0,(LPARAM)cStrings[iSelInd]);
		}
		while(iSelInd<26)
		{
			iSelInd ++;
			LoadString(hInst,iSelInd,cStrings[iSelInd],4);
			SendMessage(hCombo_FunctionKey,CB_ADDSTRING,0,(LPARAM)cStrings[iSelInd]);
			if(sOP.bUseFunctionKey == 1 && (sOP.iFunctionKey == iSelInd + 95))
			{
				iCurSel3 = iSelInd;
			}
		}
		iSelInd = 0;
		SendMessage(hComWindows,CB_SETCURSEL,iCurSel-1,0);
		SendMessage(hComSelf,CB_SETCURSEL,iCurSel2 - 1,0);

		if (sOP.bUseMouse)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECKMOUSE,BST_CHECKED);
		}
		if(sOP.bUseFunctionKey == 0)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_FUNCTIONKEY,BST_UNCHECKED);
			EnableWindow(hCombo_FunctionKey,FALSE);
			SendMessage(hCombo_FunctionKey,CB_SETCURSEL,3,0);
		}
		else if(sOP.bUseFunctionKey == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_FUNCTIONKEY,BST_CHECKED);
			EnableWindow(hCombo_FunctionKey,TRUE);
			SendMessage(hCombo_FunctionKey,CB_SETCURSEL,iCurSel3-18,0);
		}
		return FALSE;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam) ->code)
		{
		case PSN_KILLACTIVE:
			iSelInd = SendMessage(hComWindows,CB_GETCURSEL,0,0);
			SendMessage(hComWindows,CB_GETLBTEXT,iSelInd,(LPARAM)cItemText);
			cHK = cItemText[0];
			iSelInd = SendMessage(hComSelf,CB_GETCURSEL,0,0);
			SendMessage(hComSelf,CB_GETLBTEXT,iSelInd,(LPARAM)cItemText);
			if(cHK == cItemText[0])
			{
				MessageBox(hwndDlg,"不可以使用相同的快捷键!","隐身专家",MB_OK);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;
			}


			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_APPLY:
			//MessageBox(hwndDlg,"apply","ok",MB_OK);
			//SetFilePointer(hOptionFile,4,NULL,FILE_BEGIN);
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKMOUSE) == BST_UNCHECKED)
			{
				lpBuff [0] = char (0);
				lpBuff [1] = char (10);
				sOP.bUseMouse = lpBuff[0];
				::SetWindowText(hMouse,"禁用");
			}
			else
			{
				lpBuff [0] = char (1);
				lpBuff [1] = char (10);
				sOP.bUseMouse = lpBuff[0];
				::SetWindowText(hMouse,"左键 + 右键");
			}
			if (hMouseHook == NULL && sOP.bUseMouse)
			{
				SetMouseHook(1);
			}
			else if(hMouseHook != NULL && sOP.bUseMouse == 0)
			{
				SetMouseHook(0);
			}



			//WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);
			//WriteOptionFile(sOP);


////////////////////////////////////////////////////////////////////////////////////////////////
			iSelInd = SendMessage(hComWindows,CB_GETCURSEL,0,0);
			SendMessage(hComWindows,CB_GETLBTEXT,iSelInd,(LPARAM)cItemText);
			cHK = cItemText[0];
			sOP.iHideWindows = cHK;
			UnregisterHotKey(hMainDialog,aHideWindowsHotKey);
			aHideWindowsHotKey = GlobalAddAtom("HideWindowsHotKey");
			bSuc = RegisterHotKey(hMainDialog,aHideWindowsHotKey,MOD_CONTROL,cHK);
			if (bSuc == 0)
			{
				MessageBox(hwndDlg,"注册隐藏所有窗口热键失败，请更换再注册!","隐身专家",MB_OK);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return 1;
			}
			//SetEndOfFile(hOptionFile);
			/*SetFilePointer(hOptionFile,0,NULL,FILE_BEGIN);
			WriteFile(hOptionFile,cItemText,1,&lWritten,NULL);*/
			//WriteOptionFile(sOP);
			strcat(cConText,(const char *)cItemText);
			::SetWindowText(hKeyboard,cConText);
//////////////////////////////////////////////////////////////////////////////////////////
			iSelInd = SendMessage(hComSelf,CB_GETCURSEL,0,0);
			SendMessage(hComSelf,CB_GETLBTEXT,iSelInd,(LPARAM)cItemText);
			cHK = cItemText[0];
			sOP.iShowSelf = cHK; 
			UnregisterHotKey(hMainDialog,aShowSelf);
			aShowSelf = GlobalAddAtom("ShowSelf");
			bSuc = RegisterHotKey(hMainDialog,aShowSelf,MOD_CONTROL,cHK);
			if (bSuc == 0)
			{
				MessageBox(hwndDlg,"注册显示程序主窗口热键失败，请更换再注册!","隐身专家",MB_OK);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return 1;
			}
/////////////////////////////////////////////////////////////////////////////////////////////
			if (IsDlgButtonChecked(hwndDlg,IDC_CHECK_FUNCTIONKEY)) 
			{
				iSelInd = SendMessage(hCombo_FunctionKey,CB_GETCURSEL,0,0);
				UnregisterHotKey(hMainDialog,aFuncKey);
				aFuncKey = GlobalAddAtom("FunctionKeyList");
				bSuc = RegisterHotKey(hMainDialog,aFuncKey,0,iSelInd+113);
				if (bSuc == 0)
				{
					MessageBox(hwndDlg,"注册功能键失败!","隐身专家",MB_OK);
					SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
					return TRUE;
				}
				sOP.bUseFunctionKey = 1;
				sOP.iFunctionKey = iSelInd+113;
			}
			else
			{
				UnregisterHotKey(hMainDialog,aFuncKey);
				sOP.bUseFunctionKey = 0;
				sOP.iFunctionKey = 0;
			}
////////////////////////////////////////////////////////////////////////////////////////////////
			WriteOptionFile(sOP);

			strcpy(cConText,"Ctrl + ");
			strcat(cConText,(const char *)cItemText);
			::SetWindowText(hKeySelf,cConText);

			//PropSheet_CancelToClose(::GetParent(hwndDlg));
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);


			
			return FALSE;
		default:
			return FALSE;
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CHECKMOUSE:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKMOUSE) == BST_UNCHECKED)
			{
				MessageBox(hwndDlg,"如果不与其它程序冲突，不推荐取消此选项!","隐身专家",MB_OK);
			}
		return TRUE;   
		case IDC_CHECK_FUNCTIONKEY:
			if (IsWindowEnabled(hCombo_FunctionKey))
			{
				EnableWindow(hCombo_FunctionKey,FALSE);
			}
			else
			{
				EnableWindow(hCombo_FunctionKey,TRUE);
			}
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_COMBOWINDOWS:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			}

			break;
		case IDC_COMBOSELF:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			}
			break;
		case IDC_COMBO_FUNCTIONKEY:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			}
		
		default:
			return TRUE;
		}
	default:
		return FALSE;

	}
	//return FALSE;
}

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
		if (bRef != 0)
		{
			SendMessage(hwnd,WM_COMMAND,IDC_REFRESH,0);
		}
		


}


int CreateOptionProps()
{
	PROPSHEETPAGE psp[4];
	PROPSHEETHEADER psh ;


	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInst;
	psp[0].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGHOTKEY);
	psp[0].pszIcon = NULL;
	psp[0].pfnDlgProc =(int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))HotKeyProc;
	psp[0].pszTitle = "热键";
	psp[0].lParam = NULL;


	psp[1].dwSize = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInst;
	psp[1].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGPASSWORD);
	psp[1].pszIcon = NULL;
	psp[1].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))PasswordProc;
	psp[1].pszTitle = "密码保护";
	psp[1].lParam = NULL;



	psp[2].dwSize = sizeof(PROPSHEETPAGE);
	psp[2].dwFlags = PSP_USETITLE;
	psp[2].hInstance = hInst;
	psp[2].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGBK);
	psp[2].pszIcon = NULL;
	psp[2].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))BKProc;
	psp[2].pszTitle = "后台运行";
	psp[2].lParam = NULL;


	psp[3].dwSize = sizeof(PROPSHEETPAGE);
	psp[3].dwFlags = PSP_USETITLE;
	psp[3].hInstance = hInst;
	psp[3].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGOTHER);
	psp[3].pszIcon = NULL;
	psp[3].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))OtherProc;
	psp[3].pszTitle = "其它";
	psp[3].lParam = NULL;











	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE;
	psh.hwndParent = hMainDialog;
	psh.hInstance = hInst;
	psh.pszIcon = NULL;
	psh.pszCaption = "选项";
	psh.nPages = 4;
	psh.ppsp = (_PROPSHEETPAGEA *)&psp;

	return (PropertySheet(&psh));



}


INT_PTR CALLBACK OtherProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	static HWND hSpin = NULL,hEditInterval = NULL;
	static int iR = 0;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		hOptionDialog = ::GetParent(hwndDlg);
		hRadio_ShowAll = GetDlgItem(hwndDlg,IDC_RADIOSHOW);
		hHideMain = GetDlgItem(hwndDlg,IDC_CHECK_HIDEMAINWND);
		hHideIcon = GetDlgItem(hwndDlg,IDC_CHECK_HIDEICON);

		hDlg_Other = hwndDlg;
		if(sOP.iKorS == 'S')
		{
			CheckRadioButton(hwndDlg,IDC_RADIOKILL,IDC_RADIOSHOW,IDC_RADIOSHOW);
		}
		else
		{
			CheckRadioButton(hwndDlg,IDC_RADIOKILL,IDC_RADIOSHOW,IDC_RADIOKILL);
		}
		hSpin = ::GetDlgItem(hwndDlg,IDC_SPIN);
		hEditInterval = ::GetDlgItem(hwndDlg,IDC_EDITINTERVAL);
		SendMessage(hSpin,UDM_SETRANGE,0,MAKELONG(9,1));
		SendMessage(hEditInterval,EM_LIMITTEXT,1,0);
		if (sOP.iRefInterval == 0)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECKREFRESH,BST_CHECKED);
			::EnableWindow(hSpin,FALSE);
			::EnableWindow(hEditInterval,FALSE);
		}
		else
		{
			SetDlgItemInt(hwndDlg,IDC_EDITINTERVAL,sOP.iRefInterval,FALSE);
		}

		if(sOP.bDisableSound == 1)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECK_DISABLESOUND,BST_CHECKED);
		}
		
		if(sOP.bAutoRun == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_AUTORUN,BST_CHECKED);
			EnableWindow(hHideMain,TRUE);
			EnableWindow(hHideIcon,TRUE);

			if(sOP.bHideMain == 1)
			{
				
				CheckDlgButton(hwndDlg,IDC_CHECK_HIDEMAINWND,BST_CHECKED);
			}
			if(sOP.bHideIcon == 1)
			{
				CheckDlgButton(hwndDlg,IDC_CHECK_HIDEICON,BST_CHECKED);
			}
		}
		else
		{
			EnableWindow(hHideMain,FALSE);
			EnableWindow(hHideIcon,FALSE);
		}

		if(sOP.bAddQuickLaunch == 1)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECK_ADDQL,BST_CHECKED);
		}


		return FALSE;
	case WM_NOTIFY:
		if(((LPNMUPDOWN)lParam )->hdr.code == (UINT)UDN_DELTAPOS )
		{
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
		}



		switch((LPNMHDR(lParam))->code )
		{
		case PSN_SETACTIVE:
			if (sOP.bUsePassword == 1)
			{
				CheckRadioButton(hwndDlg,IDC_RADIOKILL,IDC_RADIOSHOW,IDC_RADIOKILL);
				EnableWindow(hRadio_ShowAll,FALSE);
			}
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_KILLACTIVE:
			iR = GetDlgItemInt(hwndDlg,IDC_EDITINTERVAL,NULL,FALSE);
			if ((iR <1 || iR>10)&& ::IsDlgButtonChecked(hwndDlg,IDC_CHECKREFRESH)!= BST_CHECKED)
			{
				MessageBox(hwndDlg,"刷新间隔只能是1~9之间的数字!","隐身专家",MB_OK);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;
			}
			//break;
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_APPLY:
			//SetFilePointer(hOptionFile,6,NULL,FILE_BEGIN);
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKREFRESH) == BST_CHECKED)
			{
				KillTimer(hMainDialog,IDT_REFRESH);
				lpBuff[0] = 0;
			}
			else
			{
				lpBuff[0] = char(iR);
				::SetTimer(hMainDialog,IDT_REFRESH,lpBuff[0]*1000,(TIMERPROC)TimerProc);
			}
			lpBuff[1] = char (10);
			//WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);
			sOP.iRefInterval = lpBuff[0];
			/////////////////////////////////////////////////////////////////////////////////////////
			if(::IsDlgButtonChecked(hwndDlg,IDC_RADIOKILL) == BST_CHECKED)
			{
				lpBuff[0] = 'K';lpBuff[1] = char (10);
			}
			else if(::IsDlgButtonChecked(hwndDlg,IDC_RADIOSHOW) == BST_CHECKED)
			{
				lpBuff[0] = 'S';lpBuff[1] = char (10);
			}
			sOP.iKorS = lpBuff[0];
			//WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);

			////////////////////////////////////////////////////////////////////////////////////////////
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECK_DISABLESOUND) == BST_CHECKED)
			{
				sOP.bDisableSound = 1;
			}
			else
			{
				sOP.bDisableSound = 0;
			}
			if (::IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTORUN)==BST_CHECKED)
			{
				sOP.bAutoRun = 1;
				RegSetValueEx(hRegAutoRun,"隐身专家",NULL,REG_SZ,(const BYTE*)cCurrentPath,strlen(cCurrentPath));
				if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEMAINWND))
				{
					sOP.bHideMain = 1;	
				}
				else
				{
					sOP.bHideMain = 0;
				}
				if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEICON))
				{
					sOP.bHideIcon = 1;
				}
				else
				{
					sOP.bHideIcon = 0;
				}
			}
			else
			{
				sOP.bAutoRun = 0;
				sOP.bHideMain = 0;
				sOP.bHideIcon = 0;
				RegDeleteValue(hRegAutoRun,"隐身专家");

			}
			if (::IsDlgButtonChecked(hwndDlg,IDC_CHECK_ADDQL)== BST_CHECKED)
			{
				sOP.bAddQuickLaunch = 1;
				hr = ppf->Save( (unsigned short *)cMBShortcutName,FALSE);
				if (SUCCEEDED(hr))
				{
					//MessageBox(hwndDlg,"OK","OK",MB_OK);
				}
			}
			else
			{
				sOP.bAddQuickLaunch = 0;
				DeleteFile(LPCTSTR(cShortcutName));
			}

			WriteOptionFile(sOP);



			//PropSheet_CancelToClose(::GetParent(hwndDlg));
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
		default:
			return FALSE;
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CHECKREFRESH:
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKREFRESH) == BST_CHECKED)
			{
				MessageBox(hwndDlg,
					"如果取消此选项，你必须点击‘刷新’按钮来刷新窗口列表，如果在选取保留窗口之前忘记手动刷新，程序有时会出错。如果系统性能较好，不推荐选中此选项!",
					"隐身专家",MB_OK);
				::EnableWindow(hSpin,FALSE);
				::EnableWindow(hEditInterval,FALSE);
			}
			else 
			{
				::EnableWindow(hSpin,TRUE);
				::EnableWindow(hEditInterval,TRUE);
			}
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			//PropSheet_UnChanged(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case (IDC_RADIOKILL/*||IDC_RADIOSHOW*/):
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case (IDC_RADIOSHOW):
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_DISABLESOUND:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_AUTORUN:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTORUN))
			{
				EnableWindow(hHideMain,TRUE);
				EnableWindow(hHideIcon,TRUE);
				CheckDlgButton(hwndDlg,IDC_CHECK_HIDEMAINWND,BST_CHECKED);
			}
			else
			{
				EnableWindow(hHideMain,FALSE);
				EnableWindow(hHideIcon,FALSE);
			}
			
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_HIDEMAINWND:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_HIDEICON:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_ADDQL:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		default:
			return FALSE;

		}
		return FALSE;
	default:
		return FALSE;
	}

	return FALSE;

}


INT_PTR CALLBACK BKProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(sOP.bAutoHide == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_AUTOHIDEBK,BST_CHECKED);
			
		}
		return FALSE;
	case WM_NOTIFY:
		switch((LPNMHDR(lParam))->code )
		{
		case PSN_APPLY:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTOHIDEBK))
			{
				sOP.bAutoHide = 1;
				CheckDlgButton(hMainDialog,IDC_CHECK_AUTOHIDE,BST_CHECKED);
			}
			else
			{
				sOP.bAutoHide = 0;
				CheckDlgButton(hMainDialog,IDC_CHECK_AUTOHIDE,BST_UNCHECKED);
			}
			WriteOptionFile(sOP);
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			break;
		default:
			return FALSE;
		}
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CHECK_AUTOHIDEBK:
			PropSheet_Changed(GetParent(hwndDlg),hwndDlg);
			
			break;
		default:
			return FALSE;
		}

	default:
		return FALSE;

	}

	return FALSE;
}

int CreateAboutProps()
{
	PROPSHEETPAGE psp[3];
	PROPSHEETHEADER psh;

	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInst;
	psp[0].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGABOUT);
	psp[0].pszIcon = NULL;
	psp[0].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))AboutProc;
	psp[0].pszTitle = "版权";
	psp[0].lParam = NULL;

	psp[1].dwSize = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInst;
	psp[1].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGREADME);
	psp[1].pszIcon = NULL;
	psp[1].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))ReadMeProc;
	psp[1].pszTitle = "自述";
	psp[1].lParam = NULL;

	psp[2].dwSize = sizeof(PROPSHEETPAGE);
	psp[2].dwFlags = PSP_USETITLE;
	psp[2].hInstance = hInst;
	psp[2].pszTemplate = MAKEINTRESOURCE(IDD_DIALOGHISTORY);
	psp[2].pszIcon = NULL;
	psp[2].pfnDlgProc = (int (_stdcall*)(struct HWND__*,unsigned int,unsigned int ,long))HistoryProc;
	psp[2].pszTitle = "版本历史";
	psp[2].lParam = NULL;


	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE|PSH_NOAPPLYNOW|PSH_NOCONTEXTHELP;
	psh.hwndParent = hMainDialog;
	psh.hInstance = hInst;
	psh.pszIcon = NULL;
	psh.pszCaption = "关于...";
	psh.nPages = 3;
	psh.ppsp = (_PROPSHEETPAGEA *)&psp;

	

	LoadLibrary("RICHED20.DLL");






	return (PropertySheet(&psh));

}


INT_PTR CALLBACK ReadMeProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	
	static HANDLE hReadMe ;
	EDITSTREAM es;
	static HWND hRE;
	CHARRANGE crSel;


 

	switch(uMsg)
	{
	case WM_INITDIALOG:
		//hEditReadme = ::GetDlgItem(hwndDlg,IDC_EDITREADME);
		//::SetWindowText(hEditReadme,cCont);
		hOptionDialog = ::GetParent(hwndDlg);
		hRE = ::GetDlgItem(hwndDlg,IDC_RICHEDIT2);
		if (hReadMe = CreateFile("readme.rtf",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL))
		{
			es.dwCookie = (DWORD_PTR)hReadMe;
			es.dwError  = 0;
			es.pfnCallback  = EditStreamCallback;

			SendMessage(hRE,EM_STREAMIN,(LPARAM)SF_RTF,(WPARAM)&es);
			CloseHandle(hReadMe);
		}
		//PropSheet_SetCurSel(::GetParent(hwndDlg),NULL,0);
     	PostMessage(hwndDlg,INFO_DISPLAY,0,0);

		
		return FALSE;
	case INFO_DISPLAY:
			crSel.cpMin = 0;
			crSel.cpMax = 0;
			SendMessage(hRE,EM_EXSETSEL,0,(LPARAM)&crSel);
			return TRUE;


	case WM_NOTIFY:
		switch(((LPNMHDR)lParam) ->code)
		{
		case PSN_SETACTIVE:
			PostMessage(hwndDlg,INFO_DISPLAY,0,0);

			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);

		default :
			return 0;
		}
		return 0;

	case WM_CTLCOLORSTATIC:
		return (LRESULT)wbr;
	default:
		return FALSE;
	}
		

	return FALSE;
}


INT_PTR CALLBACK HistoryProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	
	static HANDLE hVerHist ;
	EDITSTREAM es;
	static HWND hRE;
	CHARRANGE crSel;


	switch(uMsg)
	{
	case WM_INITDIALOG:
		hOptionDialog = ::GetParent(hwndDlg);
		hRE = ::GetDlgItem(hwndDlg,IDC_RICHEDITHIST);
		if (hVerHist = CreateFile("VerHistory.rtf",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL))
		{
			es.dwCookie = (DWORD_PTR)hVerHist;
			es.dwError  = 0;
			es.pfnCallback  = EditStreamCallback;

			SendMessage(hRE,EM_STREAMIN,(LPARAM)SF_RTF,(WPARAM)&es);
			CloseHandle(hVerHist);
		}
		//PropSheet_SetCurSel(::GetParent(hwndDlg),NULL,0);
     	PostMessage(hwndDlg,INFO_DISPLAY,0,0);

		
		return FALSE;
	case INFO_DISPLAY:
			crSel.cpMin = 0;
			crSel.cpMax = 0;
			SendMessage(hRE,EM_EXSETSEL,0,(LPARAM)&crSel);
			return TRUE;


	case WM_NOTIFY:
		switch(((LPNMHDR)lParam) ->code)
		{
		case PSN_SETACTIVE:
			PostMessage(hwndDlg,INFO_DISPLAY,0,0);

			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);

		default :
			return 0;
		}
		return 0;

	/*case WM_CTLCOLORSTATIC:
		return (LRESULT)wbr;*/
	default:
		return FALSE;
	}
		

	return FALSE;
}

DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG *pcb)
{
	ReadFile((HANDLE)dwCookie,pbBuff,cb,(LPDWORD)pcb,NULL);

	return 0;
}



LRESULT CALLBACK GetMsgProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	MSG * pMsg;
	pMsg = (MSG*)lParam;
	if ((nCode <0))//|| !IsChild(hMainDialog,pMsg ->hwnd))
	{
		return (CallNextHookEx(hGM,nCode,wParam,lParam));
	}
	switch(pMsg ->message)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:

		MSG msg;
		msg.lParam    = pMsg ->lParam ;
		msg.wParam    = pMsg ->wParam ;
		msg.message   = pMsg ->message ;
		msg.hwnd      = pMsg ->hwnd ;
		SendMessage(hToolTip,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);
		break;
	default:
		break;
	}
	return (CallNextHookEx(hGM,nCode,wParam,lParam));

}

int WriteOptionFile(OptionFile sOP)
{
	


	char * temp = cOptionPacket;
	*(UINT*)cOptionPacket = sOP.iHideWindows;
	cOptionPacket += sizeof(UINT);
	*(UINT*)cOptionPacket = sOP.iShowSelf;
	cOptionPacket += sizeof(UINT);
	*(int*)cOptionPacket = sOP.bUseMouse;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.iRefInterval;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.iKorS;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bDisableSound;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bAutoRun;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bAddQuickLaunch;
	cOptionPacket += sizeof (int);
	*(int*)cOptionPacket = sOP.bOnTop;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.iUsedTimes;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bRegistered;
	cOptionPacket += sizeof(int);
	memcpy(cOptionPacket,sOP.cSerialNumber,36);
	cOptionPacket += 36;
	memcpy(cOptionPacket,sOP.cMiddleCode,36);
	cOptionPacket += 36;
	*(int*)cOptionPacket = sOP.bUsePassword;
	cOptionPacket += sizeof(int);
	memcpy(cOptionPacket,sOP.cPassword,15);
	cOptionPacket += 15;
	*(int*)cOptionPacket = sOP.bUseFunctionKey;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.iFunctionKey;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bUseBKMouse;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bAutoHide;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bHideMain;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bHideIcon;
	cOptionPacket += sizeof(int);
	

	
	


	cOptionPacket = temp;

	//File operation
	hOptionFile = CreateFile("Misc.hdo",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
		FileSizeLow = GetFileSize(hOptionFile,&FileSizeHigh);
	
	WriteFile(hOptionFile,cOptionPacket,sizeof(OptionFile)+36+36,&lWritten,NULL);
	CloseHandle(hOptionFile);

	SetParameter(sOP);


	return TRUE;
}


int ReadOptionFile(OptionFile * sOP)
{
	hOptionFile = CreateFile("Misc.hdo",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
	FileSizeLow = GetFileSize(hOptionFile,&FileSizeHigh);
	
	ReadFile(hOptionFile,cOptionPacket,FileSizeLow,&lWritten,NULL);

	char * temp = cOptionPacket;

	sOP->iHideWindows = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof (UINT);
	sOP->iShowSelf = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->bUseMouse = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->iRefInterval = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->iKorS = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bDisableSound = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bAutoRun = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bAddQuickLaunch = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bOnTop = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->iUsedTimes = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bRegistered = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	memcpy(sOP->cSerialNumber,cOptionPacket,36);
	cOptionPacket += 36;
	memcpy(sOP->cMiddleCode,cOptionPacket,36);
	cOptionPacket += 36;
	sOP->bUsePassword = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	memcpy(sOP->cPassword,cOptionPacket,15);
	cOptionPacket += 15;
	sOP->bUseFunctionKey = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->iFunctionKey = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bUseBKMouse = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bAutoHide = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bHideMain = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bHideIcon += *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);



	cOptionPacket = temp;


	CloseHandle(hOptionFile);

	SetParameter(*sOP);



	return TRUE;

}



DWORD WINAPI NetworkThread(LPVOID lp)
{
	char *pVerValue,SubBlock[100]="";
	long lResponseHeader = 0,lFileSize = 0,lDownloadSize = 0;
	int iRecvSize = 0,iError = 0,iVerLen = 0;
	HANDLE hVerInfoFile;
	DWORD dwWritten = 0,dwVerInfoSize = 0,dwError = 0;
	VersionInfo viRF;
	float fCurrentVer = 0;
	URL_COMPONENTS url_c;

	cObject = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cHostName = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cValue =  (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cRecvBuff = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pVerBuff = (char*)VirtualAlloc(NULL,3*1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	


  
	sock_Scan = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(HD_Connect(sock_Scan,"kinghome.51.net",80)==0)
	{
		HD_GarbageRecycle();
		return 0;
	}   
	memset(cObject,0,1024);
	memset(cHostName,0,1024);
	lHeaderLength = 0;
	strcpy(cObject,"/VersionInfo.hdv");
	strcpy(cHostName,"kinghome.51.net");
	
	HD_FormatRequestHeader(cObject,cHostName,&lHeaderLength,cRequestHeader);
	if(HD_SendRequest(sock_Scan,cRequestHeader)==0)
	{
		HD_GarbageRecycle();
		return 0;
	}

	memset(cResponseHeader,0,1024);
	lHeaderLength = 0;
	if(HD_RecvResponseHeader(sock_Scan,cResponseHeader,&lHeaderLength)==0)
	{
		HD_GarbageRecycle();
		return 0;
	}
	if (HD_GetState(cResponseHeader) == 0)
	{
		HD_GarbageRecycle();
		return 0;
	}
	lFileSize = 0;
	HD_GetHeaderField(cResponseHeader,"Content-Length",cValue);
	lFileSize = atol(cValue);	

	hVerInfoFile = CreateFile("versioninfo.hdv",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);



	lDownloadSize = 0;
	iRecvSize = 0;
	while(lDownloadSize < lFileSize)
	{
		iRecvSize = recv(sock_Scan,cRecvBuff,1024,0);
		if (iRecvSize == SOCKET_ERROR)
		{
			HD_GarbageRecycle();
			iError = WSAGetLastError();
			return 0;
		}
		else if (iRecvSize == 0)
		{
			//continue;
			return 0;
		}
		WriteFile(hVerInfoFile,cRecvBuff,iRecvSize,&dwWritten,NULL);

		lDownloadSize += iRecvSize;
		memset(cRecvBuff,0,1024);
	}
	CloseHandle(hVerInfoFile);
	if (ReadVersionInfoFile(&viRF) == 0)
	{
		HD_GarbageRecycle();
		return 0;
	}
	dwVerInfoSize = GetFileVersionInfoSize((LPTSTR)"HideDragon.exe",0);
	if(dwVerInfoSize == 0)
	{
		HD_GarbageRecycle();
		dwError = GetLastError();
		//MessageBox(NULL,"1812","HD",MB_OK);
		return 0;
	}
	struct LANGANDCODEPAGE {
	  WORD wLanguage;
	  WORD wCodePage;
	} *lpTranslate;


	GetFileVersionInfo((LPTSTR)"HideDragon.exe",0,dwVerInfoSize,pVerBuff);
	VerQueryValue(pVerBuff,"\\VarFileInfo\\Translation",(void **)&lpTranslate,(unsigned int*)&iVerLen);

	wsprintf( SubBlock, 
				TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
				lpTranslate[0].wLanguage,
				lpTranslate[0].wCodePage);

	VerQueryValue(pVerBuff,SubBlock,(void **)&pVerValue,(unsigned int*)&iVerLen);
	fCurrentVer = (float)atof(pVerValue);
	if(fCurrentVer<viRF.fVersionNum)
	{
		::ShowWindow(hStatic_Update,SW_SHOW);
		::ShowWindow(hButton_Update,SW_SHOW);
	}
	shutdown(sock_Scan,0);
	closesocket(sock_Scan);

	for(int i = 0;i<viRF.iSiteNum;i++)
	{
		sock_Scan = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		memset(cObject,0,1024);
		memset(cHostName,0,1024);

		url_c.dwStructSize = sizeof(URL_COMPONENTS);

		url_c.lpszUrlPath = cObject;
		url_c.lpszExtraInfo = NULL;
		url_c.lpszPassword = NULL;
		url_c.lpszScheme = NULL;
		url_c.lpszHostName = cHostName;
		url_c.lpszUserName = NULL;


		url_c.dwUrlPathLength = 100;
		url_c.dwHostNameLength = 100;
		url_c.dwExtraInfoLength = 0;
		url_c.dwPasswordLength = 0;
		url_c.dwSchemeLength = 0;
		url_c.dwUserNameLength = 0;


		if(InternetCrackUrl(viRF.sUI[i].cURLs,viRF.sUI[i].lSize,NULL,&url_c) != TRUE)
		{
			HD_GarbageRecycle();
			dwError = GetLastError();
			return 0;
		}
		if (strlen(cObject) == 0)
		{
			strcpy(cObject,"/");
		}
		if (HD_Connect(sock_Scan,cHostName,80) == 0)
		{
			continue;
		}
		memset(cRequestHeader,0,1024);
		HD_FormatRequestHeader(cObject,cHostName,&lHeaderLength,cRequestHeader);
		if (HD_SendRequest(sock_Scan,cRequestHeader)==0)
		{
			continue;
		}
		if(HD_RecvResponseHeader(sock_Scan,cResponseHeader,&lHeaderLength) == 0)
		{
			continue;
		}
		memset(cRecvBuff,0,1024);
		recv(sock_Scan,cRecvBuff,1024,0);
		shutdown(sock_Scan,0);
		closesocket(sock_Scan);
	}

 

	

	//MessageBox(hMainDialog,"一次扫描完成","隐身专家",MB_OK);
	
	CloseHandle(hNetworkThread);
	hNetworkThread = NULL;
	
	return 1;
}

int HD_Connect(SOCKET sock_Scan,char * cHostName,int iPort)
{
	hostent * pHostent=NULL;
	sockaddr_in ServerAddr;

	pHostent = gethostbyname(cHostName);

	if (pHostent == NULL)
	{
		return 0;
	}
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(iPort);
	memcpy(&(ServerAddr.sin_addr),pHostent->h_addr_list[0],4);

	if (connect(sock_Scan,(sockaddr*)&ServerAddr,sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		//MessageBox(hMainDialog,"connect error","hd",MB_OK);
		return 0;
	}

	return 1;
}

int HD_FormatRequestHeader(char * cObject,char * cHostName,long * lHeaderLength,char * cRequestHeader)
{
	memset(cRequestHeader,'\0',1024);
	

	/////////////////////////////////////////////////////////////////////////////////////////////
	strcat(cRequestHeader,"GET ");
	strcat(cRequestHeader,cObject);
	strcat(cRequestHeader," HTTP/1.1");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"Host:");
	strcat(cRequestHeader,cHostName);
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"Accept:*/*");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"Connection:Keep-Alive");
	strcat(cRequestHeader,"\r\n");

	strcat(cRequestHeader,"\r\n");

	*lHeaderLength = strlen(cRequestHeader);

	return 1;
}


int HD_SendRequest(SOCKET sock_Scan,char * cRequest)
{
	int iError = 0;
	iError = send(sock_Scan,cRequestHeader,lHeaderLength,0);
	if(iError ==0 || iError == SOCKET_ERROR)
	{
		iError = WSAGetLastError();
		//MessageBox(hMainDialog,"ERROR","HD",MB_OK);
		return 0;
	}

	return 1;
}

int HD_RecvResponseHeader(SOCKET sock_Scan,char * cResponseHeader,long * lHeaderLength)
{
	int i= 0,bEndRecv = 0,iError = 0;
	long lRecvSize = 0;
	char b = ' ';
	memset(cResponseHeader,0,1024);
	while(!bEndRecv&&i < 1024)
	{
		lRecvSize = recv(sock_Scan,&b,1,0);
		if (lRecvSize ==SOCKET_ERROR)
		{
			iError = WSAGetLastError();
			//MessageBox(hMainDialog,"接收响应头失败","隐身专家",MB_OK);
			return 0;
		}
		else if (lRecvSize == 0)
		{
			//continue;
			return 0;
		}
		cResponseHeader[i]=b;
		if(i>=3)
		{
			if(cResponseHeader[i-3]=='\r'&&cResponseHeader[i-2]=='\n'&&
				cResponseHeader[i-1]=='\r'&& cResponseHeader[i-0]=='\n')
			{
				bEndRecv=1;
			}
		}
		i++;
	}
	lRecvSize = i-1;
	*lHeaderLength = lRecvSize;

	return 1;
}


int HD_GetHeaderField(char * cResponseHeader,char * cField,char* cValue)
{
	char * pTemp = NULL,*pTemp2 = NULL;

	pTemp = strstr(cResponseHeader,cField);
	if(pTemp == NULL)
	{
		return 0;
	}
	pTemp+=strlen(cField);
	pTemp += 2;
	pTemp2 = strstr(pTemp,"\r\n");
	memset(cValue,0,1024);
	memcpy(cValue,pTemp,pTemp2-pTemp);	

	return 1;
}


int HD_GetState(char * cResponseHeader)
{
	char * temp1= NULL;
	char cState[3];

   
	temp1 = cResponseHeader;
	cState[0] = (char)temp1[9];
	cState[1] = (char)temp1[10];
	cState[2] = (char)temp1[11];

	if(atoi(cState)!=200)
	{
		return 0;
	}
	
	return 1;
}


INT_PTR CALLBACK RegisterProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int i = 0;
	char cTemp[80],cTemp2[80],*cTemp3 = NULL;
	long lRegCode = 0,lOrgCode = 0;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hEdit_MiddleCode = ::GetDlgItem(hwndDlg,IDC_EDIT_MIDDLECODE);
		hRegDialog = hwndDlg;
		for (i = 0;i<5;i++)
		{
			hEdit_RC[i] = ::GetDlgItem(hwndDlg,1050+i);
		}
		SendMessage(hEdit_RC[0],EM_LIMITTEXT,(WPARAM)8,0);
		SendMessage(hEdit_RC[1],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[2],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[3],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[4],EM_LIMITTEXT,(WPARAM)12,0);
		memset(sOP.cSerialNumber,0,1024);
		for(i = 0;i<4;i++)
		{
			strcat(sOP.cSerialNumber,cCode[i]);
			if (i<=2)
			{
				strcat(sOP.cSerialNumber,"-");
			}
		}
		SendMessage((HWND)hEdit_MiddleCode,WM_SETTEXT,0,(LPARAM)sOP.cSerialNumber);
		//::SetWindowText(hEdit_MiddleCode,cOrgCode);
 		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_COMPLETEREG:
			memset(cTemp,0,80);
			memset(cTemp2,0,80);
			MultiByteToWideChar(CP_ACP,MB_COMPOSITE,sOP.cSerialNumber,-1,(unsigned short*)cTemp,80);
			strcpy(cTemp2,"{");
			cTemp2[1] = 0;
			cTemp3 = cTemp2;
			cTemp3 += 2;
			
			memcpy(cTemp3,cTemp,75);
			cTemp2[74] = '}';
			CLSIDFromString((unsigned short*)cTemp2,(LPCLSID)&hd_guid);

			memset(cTemp2,0,80);
			memset(cTemp,0,80);
			strcpy(cTemp,"{");
			for (i = 0;i<5;i++) 
			{
				GetWindowText(hEdit_RC[i],cTemp2,16);
				strcat(cTemp,cTemp2);
				if (i < 4)
				{
					strcat(cTemp,"-"); 
				}
				memset(cTemp2,0,80);
			}
			strcat(cTemp,"}");
			memset(cTemp2,0,80);
			MultiByteToWideChar(CP_ACP,MB_COMPOSITE,cTemp,-1,(unsigned short*)cTemp2,80);
			CLSIDFromString((unsigned short*)cTemp2,(LPCLSID)&hd_guid2);

			hd_guid.Data1 ^= 99999999;
			hd_guid.Data2 ^= 9999;
			hd_guid.Data3 ^= 8989;
			hd_guid.Data4[1] ^= 6;
			hd_guid.Data4[2] ^= 9;
			hd_guid.Data4[3] ^= 3;
			hd_guid.Data4[4] ^= 1;
			hd_guid.Data4[5] ^= 6666;			
			hd_guid.Data4[6] ^= 8;
			hd_guid.Data4[7] ^= 8;


			//new method//////////////////////////////////////////////
			memset(cTemp2,0,80);
			for (i =0;i<4;i++)
			{
				GetWindowText(hEdit_RC[i],cTemp,5);
				lRegCode = strtol(cTemp,NULL,16);
				lOrgCode = strtol(cCode[i],NULL,16);
				lOrgCode ^= 3333;
				lOrgCode |= 3333;
				if (lRegCode != lOrgCode)
				{
					break;
				}
			}
			
			if(i>=4)
			{
				MessageBox(hwndDlg,"注册成功,从现在开始您可以获得一年的免费升级","隐身专家",MB_OK);
				strcpy(sOP.cMiddleCode,cTemp);
				sOP.bRegistered = 1;
				WriteOptionFile(sOP);
				hRegDialog = NULL;
				EndDialog(hwndDlg,1);
			}
			else
			{
				MessageBox(hwndDlg,"序列号不正确，请检查后重新输入!","隐身专家",MB_OK);
			}

			

			break;
		case IDCANCEL:
			hRegDialog = NULL;
			EndDialog(hwndDlg,0);
			return FALSE;
		case IDC_HOWREG:
			ShellExecute(NULL,"open","http://kinghome.51.net/cn/Register.htm",NULL,NULL,SW_SHOW);
		}
	}
	return FALSE;
}


VOID CALLBACK  ScanWebProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	if (hNetworkThread == NULL)
	{
		hNetworkThread = (HANDLE)_beginthreadex(NULL,0,(unsigned int(__stdcall*)(void*))NetworkThread,NULL,0,NULL);
	}

}

int GarbageRecycle()
{
	VirtualFree(cOptionPacket,0,MEM_RELEASE);
	for(int i = 0;i<100;i++)
	{
		VirtualFree(sW[i].cWndTitle,0,MEM_RELEASE);
	}
	VirtualFree(sW,0,MEM_RELEASE);
	VirtualFree(cWndText,0,MEM_RELEASE);
	VirtualFree(pszText,0,MEM_RELEASE);
	VirtualFree(cSelWndTitle,0,MEM_RELEASE);
	VirtualFree(cRequestHeader,0,MEM_RELEASE);
	VirtualFree(cResponseHeader,0,MEM_RELEASE);
	VirtualFree(sOP.cSerialNumber,0,MEM_RELEASE);
	VirtualFree(sOP.cMiddleCode,0,MEM_RELEASE);
	VirtualFree(cMBShortcutName,0,MEM_RELEASE);
	VirtualFree(cCurrentPath,0,MEM_RELEASE);
	VirtualFree(cPath_QuickLaunch,0,MEM_RELEASE);
	VirtualFree(cWorkingDic,0,MEM_RELEASE);
	VirtualFree(cShortcutName,0,MEM_RELEASE);





	return 1;
}

int HD_GarbageRecycle()
{
	VirtualFree(cObject,0,MEM_RELEASE);
	VirtualFree(cHostName,0,MEM_RELEASE);
	VirtualFree(cValue,0,MEM_RELEASE);
	VirtualFree(cRecvBuff,0,MEM_RELEASE);
	VirtualFree(pVerBuff,0,MEM_RELEASE);

	return 1;
}


INT_PTR CALLBACK PasswordProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	char cPass[11],cVali[11];
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hEdit_Password = GetDlgItem(hwndDlg,IDC_EDIT_PASSWORD);
		hEdit_Validate = GetDlgItem(hwndDlg,IDC_EDIT_VALIDATE);
		hCheck_UsePassword = GetDlgItem(hwndDlg,IDC_CHECK_USEPASSWORD);
		SendMessage(hEdit_Password,EM_LIMITTEXT,(WPARAM)10,0);
		SendMessage(hEdit_Validate,EM_LIMITTEXT,(WPARAM)10,0);

		if(sOP.bUsePassword == 0)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_USEPASSWORD,BST_UNCHECKED);
			EnableWindow(hEdit_Password,FALSE);
			EnableWindow(hEdit_Validate,FALSE);		
		}
		else
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_USEPASSWORD,BST_CHECKED);
			EnableWindow(hEdit_Password,TRUE);
			EnableWindow(hEdit_Validate,TRUE);
			SetWindowText(hEdit_Password,sOP.cPassword);
			SetWindowText(hEdit_Validate,sOP.cPassword);
		}
		if (IsWindowEnabled(hRestAll) == 0)
		{
			EnableWindow(hCheck_UsePassword,FALSE);
			EnableWindow(hEdit_Password,FALSE);
			EnableWindow(hEdit_Validate,FALSE);
		}
		return FALSE;  
	case WM_NOTIFY:
		switch((LPNMHDR(lParam))->code )
		{
		case PSN_KILLACTIVE:
			memset(cPass,0,11);
			memset(cVali,0,11);
			GetWindowText(hEdit_Password,cPass,10);
			GetWindowText(hEdit_Validate,cVali,10);

			if(IsWindowEnabled(hEdit_Password) && ((strcmp(cPass,"")==0)||(strcmp(cVali,"")==0)))
			{
				MessageBox(hwndDlg,"如果启用密码保护，需要输入密码!","隐身专家",MB_OK);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;
			}

			if(strcmp(cPass,cVali))
			{
				MessageBox(hwndDlg,"密码输入不匹配,请重新输入!","隐身专家",MB_OK);
				SetWindowText(hEdit_Password,"");
				SetWindowText(hEdit_Validate,"");
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				
				return TRUE;
			}
			if(sOP.bUsePassword == 0 && IsWindow(hRadio_ShowAll)&&IsWindow(hDlg_Other))
			{
				EnableWindow(hRadio_ShowAll,TRUE);
				CheckRadioButton(hDlg_Other,IDC_RADIOKILL,IDC_RADIOSHOW,IDC_RADIOSHOW);
			}

			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_APPLY:
			if (IsDlgButtonChecked(hwndDlg,IDC_CHECK_USEPASSWORD) == BST_CHECKED)
			{
				sOP.bUsePassword = 1;
				memset(cPass,0,15);
				GetWindowText(hEdit_Password,cPass,11);
				strcpy(sOP.cPassword,cPass);
				sOP.iKorS = 'K';
				WriteOptionFile(sOP);
			}
			else if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_USEPASSWORD) == BST_UNCHECKED)
			{
				sOP.bUsePassword = 0;
				memset(sOP.cPassword,0,15);
				sOP.iKorS = 'S';
				WriteOptionFile(sOP);
			}
			if(sOP.bUsePassword == 0 && IsWindow(hRadio_ShowAll) && IsWindow(hDlg_Other))
			{
				EnableWindow(hRadio_ShowAll,TRUE);
				CheckRadioButton(hDlg_Other,IDC_RADIOKILL,IDC_RADIOSHOW,IDC_RADIOSHOW);
			}

			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
		}
		return FALSE;
	case WM_COMMAND:  
		switch(LOWORD(wParam))
		{   
		case IDC_CHECK_USEPASSWORD:
			if(IsWindowEnabled(hEdit_Password) != 0)
			{
				SetWindowText(hEdit_Password,"");
				SetWindowText(hEdit_Validate,"");
				
				EnableWindow(hEdit_Password,FALSE);
				EnableWindow(hEdit_Validate,FALSE);  
			}
			else
			{
				EnableWindow(hEdit_Password,TRUE);
				EnableWindow(hEdit_Validate,TRUE);
			}
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_EDIT_PASSWORD:
		case IDC_EDIT_VALIDATE:
			if(HIWORD(wParam) == EN_CHANGE)
			{
				PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);			
			}
			return FALSE;
		default:
			return FALSE;
		}
		return FALSE;
	}
	return 0;
}


HWND FindTrayToolbarWindow()
{
	HWND hWnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	if(hWnd)
	{
		hWnd = FindWindowEx(hWnd,NULL,_T("TrayNotifyWnd"), NULL);
		if(hWnd)
		{
			hWnd = FindWindowEx(hWnd,NULL,_T("SysPager"), NULL);
			if(hWnd)
			{				
				hWnd = FindWindowEx(hWnd, NULL,_T("ToolbarWindow32"), NULL);
			}
		}
	}
	return hWnd;
}


int IsHaveTrayIcon(WndAndHandle * pWH,TRAYDATA * pTD)
{
	DWORD lPID;

	GetWindowThreadProcessId(hTrayWnd,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t TipChar;
	wchar_t sTip[1024] = {0};
	wchar_t* pTip = NULL;		
	int x = 0,iRet = 0;


	int iBtnCount = SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);
	for(i=0; i < iBtnCount;i++)
	{
		SendMessage(hTrayWnd,TB_GETBUTTON,i,(LPARAM)data.GetData());
		data.ReadData(&tb);
		data.ReadData(&tray,(void * )tb.dwData);
		if (i <= 30)
		{
			arrTray[i] = tray;
		}
		else
		{
			MessageBox(hMainDialog,"arrTray array is not enough","隐身专家",MB_OK);
		}
		
		
	}
	
		
	for(i = 0;i<iBtnCount;i++)
	{
		GetWindowThreadProcessId(arrTray[i].hwnd,&lPID);

		if(pWH->lPID == lPID )
		{
			SendMessage(hTrayWnd,TB_GETBUTTON,i,(LPARAM)data.GetData());
			data.ReadData(&tb);
			data.ReadData(&tray,(void * )tb.dwData);

			pTD = arrTray + i;
			if(!(tb.fsState&TBSTATE_HIDDEN))
			{			
				memset(sTip,0,1024);
				x = 0;
				pTip = (wchar_t*)tb.iString;
				do 
				{	
					if(x >= 1023)
					{
						wcscpy(sTip,L"[ToolTip was either too long or not set]");	
						break;
					}
					
					data.ReadData_Tip((wchar_t*)&TipChar, (void *)pTip++);																			
				}while(sTip[x++] = TipChar);
				
			}
			else
				wcscpy(sTip,L"[Hidden Icon]");	
			if(pWH->cIconTip != NULL)
			{
				//strcpy((char *)cTip,(LPTSTR)(sTip));
				iRet = 0;
				iRet = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
					(unsigned short*)sTip,-1,pWH->cIconTip,1022,NULL,NULL);
			}
			if(pWH->hWnd == tray.hwnd)
			{
				
			}

			return i;
		}
		
	}
	//pTD = NULL;
	return -1;
}


int OpenBKWizard()
{
	PROPSHEETPAGE psp = {0};
	HPROPSHEETPAGE ahpsp [3] = {0};
	PROPSHEETHEADER psh = {0};


	
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_DEFAULT|PSP_HIDEHEADER;
	psp.hInstance = hInst;
	psp.pfnDlgProc = BeginBKProc;
	psp.pszTemplate = MAKEINTRESOURCE(IDD_WIZARD_BEGINBK);

	ahpsp [0] = CreatePropertySheetPage(&psp);


	//Select hide window and icon page////////////////////////////////////////////////
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle = "隐身专家后台程序指定向导";
	psp.pszHeaderSubTitle = "选取想要隐藏的程序或托盘图标";
	psp.hInstance = hInst;
	psp.pfnDlgProc = SelWndIconProc;
	psp.pszTemplate = MAKEINTRESOURCE(IDD_WIZARD_SELWINDOWANDICON);

	ahpsp [1] = CreatePropertySheetPage(&psp);
	
	//Validate and finish  page//////////////////////////////////////////////
	psp.dwFlags = PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	psp.pszHeaderTitle = "h1";
	psp.pszHeaderSubTitle ="hs1";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_WIZARD_FINISH);
	psp.pfnDlgProc = FinishProc;

	ahpsp[2] = CreatePropertySheetPage(&psp);
	
	psh.dwSize = sizeof(psh);
	psh.hInstance = hInst;
	psh.hwndParent = hMainDialog;
	psh.phpage = ahpsp;
	psh.dwFlags = PSH_WIZARD97|PSH_HEADER|PSH_WATERMARK;
	psh.pszbmWatermark  = MAKEINTRESOURCE(IDB_WATERMARK);
	psh.pszbmHeader = MAKEINTRESOURCE(IDB_HEADER);
	psh.nStartPage = 0;
	psh.nPages = 3;

	PropertySheet(&psh);
	
	return 1;
}

INT_PTR CALLBACK SelWndIconProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int i = 0,iItemNum = 0;
	LVITEM wiItem;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hList_SelWI = GetDlgItem(hwndDlg,IDC_LIST_ICONANDWINDOW);
		hImage_Sel = ImageList_Create(16,16,ILC_COLOR24,30,10);
		ListView_SetImageList(hList_SelWI,hImage_Sel,LVSIL_SMALL);
		AddSelListHeader();
		return FALSE;
	case WM_NOTIFY:
		switch(((LPPSHNOTIFY)lParam)->hdr.code)
		{
		case PSN_SETACTIVE:
			ImageList_RemoveAll(hImage_Sel);
			ImageList_AddIcon(hImage_Sel,hIcon_NoIcon);
			PropSheet_SetWizButtons(GetParent(hwndDlg),PSWIZB_BACK|PSWIZB_NEXT);
			UpdateSelList();
			break;
		case PSN_WIZNEXT:
			iItemNum = ListView_GetItemCount(hList_SelWI);
			for(i = 0;i<iItemNum;i++)
			{
				if(ListView_GetCheckState(hList_SelWI,i))
				{
					wiItem.iItem = i;
					ListView_GetItem(hList_SelWI,&wiItem);
					sBKWI[iTotalBK] = (WndAndHandle*)wiItem.lParam;
					iTotalBK ++;
				}
			}
			if(iTotalBK <= 0)
			{
				MessageBox(hwndDlg,"请选择一个或多个要隐藏的图标或窗口!","隐身专家",MB_OK|MB_ICONINFORMATION);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;
			}
			break;
		}
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			
		case IDCANCEL:
			EndDialog(hwndDlg,0);
		}
	default:
		return FALSE;
			
	}
	return FALSE;
}



INT_PTR CALLBACK FinishProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hList_Confirm = GetDlgItem(hwndDlg,IDC_LIST_VALIDATE);
		ListView_SetExtendedListViewStyle(hList_Confirm,LVS_EX_FULLROWSELECT);
		hImage_Confirm = ImageList_Create(16,16,ILC_COLOR24,iTotalBK,10);
		ListView_SetImageList(hList_Confirm,hImage_Confirm,LVSIL_SMALL);
		AddConfirmListHeader();
		return FALSE;
	case WM_NOTIFY:
		switch(((LPPSHNOTIFY)lParam)->hdr.code)
		{
		case PSN_SETACTIVE:
			ImageList_RemoveAll(hImage_Confirm);
			ImageList_AddIcon(hImage_Confirm,hIcon_NoIcon);
			UpdateConfirmList();
			PropSheet_SetWizButtons(GetParent(hwndDlg),PSWIZB_BACK|PSWIZB_FINISH);
			break;
		case PSN_WIZFINISH:
			HideSelected(sBKWI,iTotalBK);
			break;
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwndDlg,0);
		}
	default:
		return FALSE;

	}
	return FALSE;
}

INT_PTR CALLBACK BeginBKProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		return FALSE;
	case WM_NOTIFY:
		switch(((LPPSHNOTIFY)lParam)->hdr.code)
		{
		case PSN_SETACTIVE:
			PropSheet_SetWizButtons(GetParent(hwndDlg),PSWIZB_NEXT);
			break;
		}
		
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			
		case IDCANCEL:
			EndDialog(hwndDlg,0);
		}
	default:
		return FALSE;
			
	}
	return FALSE;

}


int AddSelListHeader()
{
	LVCOLUMN lvColumn;
	DWORD lvStyle;
	//lvColumn.pszText = new char(60);
	
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 70;
	lvColumn.pszText = "托盘图标";
	ListView_InsertColumn(hList_SelWI,0,&lvColumn);
	lvColumn.cx    = 260;
	lvColumn.pszText = "和托盘图标关联的窗口标题或工具提示";
	ListView_InsertColumn(hList_SelWI,1,&lvColumn);
	lvColumn.cx   = 80;
	lvColumn.pszText = "进程ID";
	ListView_InsertColumn(hList_SelWI,2,&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_SelWI);
	lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_SelWI,0,lvStyle);

	//delete []lvColumn.pszText;

	return 1;
}




int UpdateSelList()
{
	DWORD lPID;
	LVITEM lvItem;
	int iconIndex = 0,iconNum = 0;
	char cPID[20] = {0};
	iTotalBK = 0;
	int i = 0,j = 0;
	WndAndHandle *pNext = NULL,*pTemp = NULL,*pBKSel = NULL;
	ICONINFO iconInfo;

	for(i = 0;i<iW_Sel;i++)
	{
		sBKSel[i].iHasTrayIcon = 0;
		sBKSel[i].iIconIndex = 0;
		sBKSel[i].hWnd = NULL;
		sBKSel[i].trayicon.hIcon = NULL;
		sBKSel[i].lPID = 0;
		memset(sBKSel[i].cProcessName,20,0);
		//sBKSel[i].cWndTitle = NULL;

	}
	iW_Sel = 0;
	iNoTitle = 0;
	
	EnumWindows(EnumWindowsForBKProc,NULL);

	for (i = 0;i<iW_Sel-1;i++)
	{
		for(j = i+1;j <= iW_Sel-1;j++)
		{
			if(sBKSel[i].lPID == sBKSel[j].lPID && strcmp(sBKSel[i].cProcessName,"explorer.exe") != 0)
			{
				if(sBKSel[i].bReserved == 1)
				{
					strcpy(sBKSel[j].cWndTitle,sBKSel[i].cWndTitle);
					sBKSel[j].bReserved = 1;
				}
				pTemp = sBKSel + j;
				while(pTemp->Next!=NULL)
				{
					pNext = pTemp->Next;
					pTemp = pNext;
				}
				pTemp->Next = sBKSel+i;
				sBKSel[i].bExist = 1;
				//sBKSel[i].Next = NULL;
				pTemp->iLink ++;
			}
		}
	}

	

	GetWindowThreadProcessId(hTrayWnd,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t TipChar = 0;
	wchar_t sTip[1024] = {0};
	wchar_t* pTip = NULL;		
	int x = 0,iRet = 0;
	ListView_DeleteAllItems(hList_SelWI);



	
	


	int iBtnCount = SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);
	iconNum = 0;
	for(i=0; i < iBtnCount;i++)
	{
		if (i <= 30)
		{
		}
		else
		{
			MessageBox(hMainDialog,"arrTray array is not enough","隐身专家",MB_OK);
		}
		SendMessage(hTrayWnd,TB_GETBUTTON,i,(LPARAM)data.GetData());
		data.ReadData(&tb);
		data.ReadData(&tray,(void * )tb.dwData);
		
		if (!(tb.fsState & TBSTATE_HIDDEN))
		{
			GetWindowThreadProcessId(tray.hwnd,&lPID);

			if(lPID == GetCurrentProcessId())
			{
				continue;
			}
			if(GetIconInfo(tray.hIcon,&iconInfo) == 0)
			{
				tray.hIcon = hIcon_Unknown;
			}
			iconIndex = ImageList_AddIcon(hImage_Sel,tray.hIcon);
			lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM;
			lvItem.pszText = pszText;
			lvItem.iImage = iconIndex;
			strcpy(lvItem.pszText,"");
			lvItem.iSubItem = 0;
			pWH = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
			pWH->cWndTitle = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
			pWH->cIconTip = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
			pWH->trayicon = tray;
			pWH->lPID = lPID;
			lvItem.lParam = (LPARAM)pWH;
			lvItem.iItem = ListView_GetItemCount(hList_SelWI);
			ListView_InsertItem(hList_SelWI,&lvItem);

			memset(sTip,0,1024);
			x = 0;
			pTip = (wchar_t*)tb.iString;
			do 
			{	
				if(x >= 1023)
				{
					wcscpy(sTip,L"[ToolTip was either too long or not set]");	
					break;
				}
				
				data.ReadData_Tip((wchar_t*)&TipChar, (void *)pTip++);																			
			}while(sTip[x++] = TipChar);

			iRet = 0;
			iRet = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
				(unsigned short*)sTip,-1,pWH->cIconTip,1022,NULL,NULL);

			lvItem.pszText = pWH->cIconTip;

			for(int j = 0;j < iW_Sel;j++)
			{
				if (lPID == sBKSel[j].lPID && strcmp(sBKSel[j].cProcessName,"explorer.exe")!=0 )
				{					
					if (sBKSel[j].bExist == 1)
					{
						sBKSel[j].iHasTrayIcon = 1;
						continue;
					}

					sBKSel[j].trayicon = tray;
					lvItem.lParam = (LPARAM)(sBKSel +j);
					iRet = ListView_SetItem(hList_SelWI,&lvItem);
					lvItem.mask = LVIF_TEXT;
					lvItem.iSubItem = 1;
					//GetWindowText(sBKSel[j].hWnd,lvItem.pszText,1022);
					lvItem.pszText = sBKSel[j].cWndTitle;
					iRet = ListView_SetItem(hList_SelWI,&lvItem);
					sBKSel[j].iHasTrayIcon = 1;
				}
				else if(lPID == sBKSel[j].lPID && strcmp(sBKSel[j].cProcessName,"explorer.exe")==0)
				{
					break;
				}
			}
			
			lvItem.mask = LVIF_TEXT|LVIF_IMAGE;
			lvItem.lParam = NULL;
			sprintf(cPID,"%X",lPID);
			lvItem.iSubItem = 2;
			lvItem.pszText = cPID;
			iRet = ListView_SetItem(hList_SelWI,&lvItem);
			if(iRet != TRUE)
			{
				MessageBox(hMainDialog,"listview_setitem error","隐身专家",MB_OK);
			}
			
			arrTray[i] = tray;
			iconNum ++;
		}
	}
	for(j = 0;j<iW_Sel ;j++)
	{
		if( sBKSel[j].iHasTrayIcon != 1 && sBKSel[j].bExist != 1)
		{
			lvItem.mask = LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE;
			lvItem.lParam = (LPARAM)(sBKSel + j);
			lvItem.iItem = ListView_GetItemCount(hList_SelWI);
			lvItem.iSubItem = 0;
			lvItem.iImage = 0;
			lvItem.pszText = "";
			ListView_InsertItem(hList_SelWI,&lvItem);


			lvItem.mask = LVIF_TEXT;
			lvItem.iSubItem = 1;
			lvItem.pszText = sBKSel[j].cWndTitle;
			ListView_SetItem(hList_SelWI,&lvItem);

			lvItem.iSubItem = 2;
			sprintf(cPID,"%X",sBKSel[j].lPID);
			lvItem.pszText = cPID;
			ListView_SetItem(hList_SelWI,&lvItem);
		}
		

		
	}

	

	
	return 1;
}





BOOL CALLBACK EnumWindowsForBKProc(HWND hwnd,LPARAM lParam)
{
	//HWND hOnw;
	DWORD lPID;

	
	if (::GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{	
		GetWindowText(hwnd,cWndText,1024);
		GetWindowThreadProcessId(hwnd,&lPID);
		if (/*!strcmp(cWndText,"")||*/!strcmp(cWndText,"Program Manager"))
			return 1;
		if (hwnd == hMainDialog||::GetParent(hwnd) == hMainDialog)
		{
			return 1;
		}


		hProcessSnap2 = (unsigned long)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		pe322.dwSize = sizeof(PROCESSENTRY32);
		Process32First((void*)hProcessSnap2,&pe322);
		while(Process32Next((void*)hProcessSnap2,&pe322))
		{
			if(pe322.th32ProcessID == lPID)
			{
				if(strcmp(strlwr(pe322.szExeFile),"explorer.exe")==0)
				{
					if (strcmp(cWndText,"") != 0)
					{
						strcpy(sBKSel[iW_Sel].cProcessName,pe322.szExeFile);
						sBKSel[iW_Sel].hWnd = hwnd;
						sBKSel[iW_Sel].lPID = lPID;
						sBKSel[iW_Sel].Next = NULL;
						GetWindowText(hwnd,sBKSel[iW_Sel].cWndTitle,1022);
						iW_Sel ++;
					}
				}
				else
				{
					strcpy(sBKSel[iW_Sel].cProcessName,pe322.szExeFile);
					sBKSel[iW_Sel].hWnd = hwnd;
					sBKSel[iW_Sel].lPID = lPID;
					sBKSel[iW_Sel].Next = NULL;
					if (strcmp(cWndText,"") == 0)
					{
						strcpy(sBKSel[iW_Sel].cWndTitle,pe322.szExeFile);
						sBKSel[iW_Sel].bReserved = 1;
					}
					else
					{
						GetWindowText(hwnd,sBKSel[iW_Sel].cWndTitle,1022);
					}
					if (IsWindow(GetParent(hwnd)))
					{
						sBKSel[iW_Sel].iWindowType = 1;
					}
					iW_Sel ++;
				}
				return 1;
			}
		}
	}
	return 1;
}


int AddConfirmListHeader()
{
	LVCOLUMN lvColumn;
	DWORD lvStyle;
	
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 70;
	lvColumn.pszText = "托盘图标";
	ListView_InsertColumn(hList_Confirm,0,&lvColumn);
	lvColumn.cx    = 260;
	lvColumn.pszText = "和托盘图标关联的窗口标题或工具提示";
	ListView_InsertColumn(hList_Confirm,1,&lvColumn);
	lvColumn.cx   = 80;
	lvColumn.pszText = "进程ID";
	ListView_InsertColumn(hList_Confirm,2,&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_SelWI);
	lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_SelWI,0,lvStyle);



	return 1;
}



int UpdateConfirmList()
{
	LVITEM conItem;
	int i = 0,iIconIndex = 0,iRet = 0;
	char cPID[20];
	char * pszTemp = NULL;



	for( i = 0;i<iTotalBK;i++)
	{
		conItem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
		conItem.iItem = ListView_GetItemCount(hList_Confirm);
		conItem.lParam = (LPARAM)sBKWI[i];
		if(sBKWI[i]->trayicon.hIcon != NULL)
		{
			iIconIndex = ImageList_AddIcon(hImage_Confirm,sBKWI[i]->trayicon.hIcon);
			conItem.pszText = "";
		}
		else
		{
			iIconIndex = 0;
			conItem.pszText = "";
		}
		conItem.iImage = iIconIndex;
		conItem.iSubItem = 0;
		iRet = ListView_InsertItem(hList_Confirm,&conItem);

		conItem.mask = LVIF_TEXT;
		conItem.iSubItem = 1;
		if(IsWindow(sBKWI[i]->hWnd))
		{
			//GetWindowText(sBKWI[i]->hWnd,sBKWI[i]->cWndTitle,2048);
			pszTemp = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
			strcpy(pszTemp,sBKWI[i]->cWndTitle);
		}
		conItem.pszText = sBKWI[i]->cWndTitle;//pszTemp;
		iRet = ListView_SetItem(hList_Confirm,&conItem);


		conItem.iSubItem = 2;
		sprintf(cPID,"%X",sBKWI[i]->lPID);
		conItem.pszText= cPID;
		iRet = ListView_SetItem(hList_Confirm,&conItem);


		
	}
		
	

	return 1;
}



int HideSelected(WndAndHandle **pWH,int iSize)
{
	LVITEM bkItem;
	//bkItem.pszText = bkText;
	int i = 0,iIconIndex = 0;
	NOTIFYICONDATA nid;
	char cPID[20];

	WndAndHandle * pNext = NULL, * pFirst = NULL,*pBKSel = NULL,*pTemp = NULL;
	
	for(i = 0;i<iSize;i++)
	{
		//sBK_All[iBK_All] = pWH[i];
		//
		sBK_All = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sBK_All->hWnd = pWH[i]->hWnd;
		sBK_All->iHasTrayIcon = pWH[i]->iHasTrayIcon;
		sBK_All->lPID = pWH[i]->lPID;
		sBK_All->trayicon = pWH[i]->trayicon;
		sBK_All->cWndTitle = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		sBK_All->cIconTip = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		
		strcpy(sBK_All->cWndTitle,pWH[i]->cWndTitle);
		strcpy(sBK_All->cProcessName,pWH[i]->cProcessName);
		//strcpy(sBK_All[iBK_All].cIconTip,pWH[i].cIconTip);
		

		if(IsWindow(pWH[i]->hWnd))
		{
			ShowWindow(pWH[i]->hWnd,SW_HIDE);
		}
		if(pWH[i]->trayicon.hIcon != NULL )
		{
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = pWH[i]->trayicon.hwnd;
			nid.uID = pWH[i]->trayicon.uID;
			Shell_NotifyIcon(NIM_DELETE,&nid);
		}
		pFirst = pWH[i];
		while(pWH[i]->Next)
		{
			pNext = pWH[i]->Next;
			pWH[i] = pNext;
			ShowWindow(pWH[i]->hWnd,SW_HIDE);
		}
		pWH[i] = pFirst;
		
		bkItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM;
		if(pWH[i]->trayicon.hIcon == NULL)
		{
			iIconIndex = 0;
		}
		else
		{
			iIconIndex = ImageList_AddIcon(hImage_BK,pWH[i]->trayicon.hIcon);
		}
		bkItem.iImage = iIconIndex;
		if(pWH[i]->trayicon.hIcon != NULL)
		{
			//strcpy(bkItem.pszText,"");
			bkItem.pszText = "";
		}
		else
		{
			bkItem.pszText ="";
			
		}
		//pTemp = ((WndAndHandle*)lvItem.lParam);
		pTemp = sBK_All;
		pBKSel = pWH[i];
		do
		{
			pTemp->lPID =pBKSel->lPID;
			strcpy(pTemp->cWndTitle,pBKSel->cWndTitle);
			//strcpy(pTemp->cIconTip,pBKSel->cIconTip);
			pTemp->trayicon = pBKSel->trayicon;
			pTemp->bReserved = pBKSel->bReserved;
			pTemp->hWnd = pBKSel->hWnd;
			pTemp->bExist = pBKSel->bExist;
			strcpy(pTemp->cProcessName,pBKSel->cProcessName);
			pTemp->iHasTrayIcon = pBKSel->iHasTrayIcon;
			pTemp->iWindowType = pBKSel->iWindowType;
			pTemp->iLink = pBKSel->iLink;
			if(pBKSel->Next != NULL)
			{						
				pNext = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				pNext->cWndTitle = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				pNext->cIconTip = (char*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				pNext->Next = NULL;
				pTemp->Next = pNext;
				pTemp = pNext;	
				pNext = pBKSel->Next;
				pBKSel = pNext;
			}
			else
			{
				break;
			}
		}while(1);
		pTemp = sBK_All;
		bkItem.lParam = (LPARAM)pTemp;//(LPARAM)pWH[i];

		bkItem.iSubItem = 0;
		bkItem.iItem = ListView_GetItemCount(hList_BK);
		ListView_InsertItem(hList_BK,&bkItem);

		bkItem.mask = LVIF_TEXT;
		bkItem.iSubItem = 1;
		if(IsWindow(pWH[i]->hWnd))
		{
			//GetWindowText(pWH[i]->hWnd,pWH[i]->cWndTitle,2022);
			bkItem.pszText=pWH[i]->cWndTitle;
		}
		else
		{
			bkItem.pszText="";
		}
		ListView_SetItem(hList_BK,&bkItem);

		bkItem.iSubItem = 2;
		sprintf(cPID,"%X",pWH[i]->lPID);
		bkItem.pszText=cPID;
		ListView_SetItem(hList_BK,&bkItem);

		
		iBK_All++;
	}
	
	EnableWindow(hButton_ShowBK,TRUE);
	return 1;
}


int ShowSelected(WndAndHandle * pWH,int iSize)
{
	int i = 0,j = 0;
	NOTIFYICONDATA nid;
	WndAndHandle * pTemp = NULL, * pFirst = NULL, * pNext = NULL;

	if(IsWindow(pWH->hWnd))
	{
		ShowWindow(pWH->hWnd,SW_SHOW);
	}
	if(pWH->trayicon.uID != NULL)
	{
		nid.cbSize           = sizeof(NOTIFYICONDATA);
		nid.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nid.uID              = pWH->trayicon.uID;
		nid.hIcon            = pWH->trayicon.hIcon;
		nid.hWnd             = pWH->trayicon.hwnd;
		//strcpy(nid.szTip,(const char *)sW_dll[i].cIconTip);
		nid.uCallbackMessage = pWH->trayicon.uCallbackMessage;
		Shell_NotifyIcon(NIM_ADD,&nid);
	}
	pFirst = pWH;
	while(pWH->Next)
	{
		pNext = pWH->Next;
		pWH = pNext;
		ShowWindow(pWH->hWnd,SW_SHOW);
	}
	pWH = pFirst;
	
	iBK_All--;
	if(iBK_All == 0)
	{
		EnableWindow(hButton_ShowBK,FALSE);
		ImageList_RemoveAll(hImage_BK);
		ImageList_AddIcon(hImage_BK,hIcon_NoIcon);
	}


	return 1;
}








int AddBKListHeader()
{
	LVCOLUMN lvColumn;
	DWORD lvStyle = 0;

	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 80;
	lvColumn.pszText = "托盘图标";
	ListView_InsertColumn(hList_BK,0,&lvColumn);
	lvColumn.cx    = 360;
	lvColumn.pszText = "和托盘图标相关联的窗口标题或工具提示";
	ListView_InsertColumn(hList_BK,1,&lvColumn);
	lvColumn.cx   = 80;
	lvColumn.pszText = "进程ID";
	ListView_InsertColumn(hList_BK,2,&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_BK);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_BK,0,lvStyle);

	hImage_BK = ImageList_Create(16,16,ILC_COLOR24,20,10);
	ImageList_AddIcon(hImage_BK,hIcon_NoIcon);
	ListView_SetImageList(hList_BK,hImage_BK,LVSIL_SMALL);

	
	return 1;
}



























