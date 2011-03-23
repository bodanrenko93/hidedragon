/*************************************************************************

  DongDapeng All rights reserved.
  http://www.hidedragon.com
  mailto:dongdapeng8@gmail.com
  Since 2003.4.6 or 2002.12

**************************************************************************/
#define _WIN32_WINNT 0x0500

#include "stdafx.h"
//Add for support ATL then use webbrowser control

#include <atlbase.h>

// Required ATL module
CComModule _Module;

#include <stdlib.h>

#include <atlcom.h>
#include <atlwin.h>
#include <atlhost.h>
#include <afxres.h>
#include <oledlg.h>
#include <crtdbg.h>
#include "process.h"

#include "winnls.h"
//#include "windows.h"
#include "resource.h"
#include "shellapi.h"
#include "string.h"
//#include "../hooklib.h"
#define _WIN32_IE 0x0501
#include "hd.h"
#include "update.h"
#include "prsht.h"
#include "windowsx.h"  
#include "richedit.h"
#include "mmsystem.h"
#include "stdio.h"
#include "shtypes.h"
#include "shlobj.h"
#include "shobjidl.h" 

#include "objidl.h"  
#include "tlhelp32.h"
#include "winsock2.h"
#include "wininet.h"
#include "objbase.h"
#include "ProcessData.h"
#include "mmsystem.h"
#include "DBClass.h"
#include "ButtonClass/WINButton.h"







#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>















//using namespace std;



//////////////////define variables///////////////////////////////////////////////////////////////////
HINSTANCE hInst;
HWND hMainDialog,hAboutDialog,hOptionDialog,hToolTip,hBalTip,hUsageTip,
hListView,hWizardDialog,hHK_HideAll,hHK_ShowSelf;
HWND hRestAll,hAds,hMouse,hStatic_Update,hButton_Update,hRadio_ShowAll,hDlg_Other,hBtnExpandInfo;
HWND Array_QH[20];

HWND hToolTip_reg;

int g_UseIcon;


//TTTOOLINFOA ti;
MSG Msg;
NOTIFYICONDATA nID;
NOTIFYICONDATAW nIDW;
HMENU hMainMenu ,hTrayMenu,hMainWndMenu,hFileMenu,hHelpMenu,hWLMenu1,hWLMenu,
hJobMenu,hAlphaMenu,hJobTipMenu,hDisguiseMenu,hBKMenu;
POINT CurMouPos;
int TrayFlag,iTotalSelected,bReserveOperation = 0 ,iFlag_ThroughIcon,iFlag_Size,iFlag_EndInit,iFlag_Restore;
ATOM aHideWindowsHotKey,aShowSelf,aResHK;
wchar_t cItemString[80],* cBKWndText;
int  iSelectedItems [100],iA1[100],iSelectedItem,iUseSkin,iExtWindow,g_iExit;
wchar_t cStrings[30][5],cOrgCode[20];
wchar_t cCode[4][5] = {L"dddd", L"dddd", L"dddd" ,L"dddd"};
int iTemp,iResTemp ,i,bFir,bRef = 1,iForSW,iForSWC,iBaSW,iBaSWC;
wchar_t * d,cHT,*cOptionFileName;
HANDLE hOptionFile,hProcessSnap;
DWORD FileSizeHigh,FileSizeLow;
PROCESSENTRY32 pe32,pe322;
DWORD lWritten,lReaded;char lpBuff [10];
WndAndHandle * sW;
OptionFile sOP;
HFONT hFont_New;
LOGFONT lf;

//Expand Button Variable//////////////
//WNDPROC wpOrgExpandProc;
//CWINButton * wb_Expand;
//HBITMAP back, DlgBack, ok, hove, press, disable;
/*CWINButton *win;*/
int iEvaluateTime = 30;

int g_HidedProcess;

//Sync variable/////////////////////////////////////////////////////////////
CRITICAL_SECTION csDelListView,csFillList;


//option dialog varialbes///////////////////////////////////////////////////
int iBuff[10];
DWORD dwPathSize=0,dwExPID = 0;
int bUseMouse,iRefInterval,iKorS,iForState;
UINT  iHideWindows ,iShowSelf ;
HHOOK hGM;
LVITEM lvItem ;
IShellLinkW * pShellLink;
IPersistFile * ppf;
HKEY hRegAutoRun;
LPITEMIDLIST lpItemIdList;
HRESULT hr;
HWND hEdit_Password = NULL ,hEdit_Validate,hCheck_UsePassword,hHK_HA,hHK_SS,hRightFirst;

//Register dialog////////////////////////////////////////////////////////////////
HWND hEdit_MiddleCode,hEdit_RC[5];
GUID hd_guid,hd_guid2;
HWND hRegDialog;

char cGUID1[] = {"3E4A5C23-B2E9-48d7-9521-D5F907F76B70"};
char cGUID2[] = {"CA21C244-8B20-4de2-ACD8-85FD57D64147"};
char cGUID3[] = {"48759383-89EA-45f4-A743-9A3FB99D31A8"};
char cGUID4[] = {"0EA01698-74CA-4751-8DCB-9F7159F4DC5C"};
char cGUID5[] = {"9C522ACF-9495-4236-AFE9-8131594661CC"};
char cGUID6[] = {"9C522AC9-9499-4239-AFE9-9131594666C9"};


int iTryTime = 5,iTryTime_re = 50;




/////////////////////////////////////////////////////
HWND hEmail,hHttp,hEditCre;
RECT rMail,rHttp,rAds;
long xP,yP;
POINT pMouse;
HCURSOR hHand,hNorm;
HICON hI1,hI2;
WndParam sWP;
HDC hHttpDC,hEmailDC;

//NetworkThread variables////////////////////////////////////////////////////////////////
HANDLE hNetworkThread,g_hUpdateHandle,g_hUpdateLangHandle;
SOCKET sock_Scan;
char * cRequestHeader,*cResponseHeader;
WSADATA wsd;
char * cObject = NULL,*cHostName =NULL,*cValue = NULL;
wchar_t * pVerBuff = NULL;
char * cRecvBuff = NULL;
long lHeaderLength = 0;
VersionInfo viRF;


//Tray icon operation///////////////////////////////////////////////////////////////////////
HWND hTrayWnd;
TRAYDATA arrTray[60];
HIMAGELIST hIconList;
UINT g_HD_Msg_GetIcon = 0;//Message send to explorer tray window.
HWND g_TrayWnd;
HANDLE g_ChangeIconTitleHandle;
//BK Operation////////////////////////////////////////////////////////////////////////////////
HWND hList_SelWI,hList_Confirm,hList_BK,hStatic_BKG,hButton_ShowBK,hHideMain,hHideIcon;
WndAndHandle **sBKWI = NULL,*sBK_All, *pWH, *pNoTitle,* sBKSel = NULL;
int iW_Sel,iTotalBK,iBK_All,iNoTitle,iBKItemCount,iTakeToBk;
HIMAGELIST hImage_BK,hImage_Sel,hImage_Confirm;
HICON hIcon_Unknown,hIcon_NoIcon,hIcon_Default;

//Draw Title
HDC hWinDC,hWinDC2,hWinDC3,hWinDC_LD;


//QuickHideWidnow Proc//////////////////////////////////////////////
HWND hEdit_PPath,hBtn_Path;

//AutoUpdate Proc///////////////////////////////////////////////////
HWND hRadioAuto,hRadioTip;


//For refresh performance///////////////////////////////////////////
SYSTEMTIME g_StartTime;
int g_iStarted,g_iAutoRun;

int g_iTip,g_iTipCount;
wchar_t * g_pTipText[10][10];

//Language variables//////////////////////////////////////////////////
int g_iUseLanguage,g_iLangCount;
HMENU g_hLangMenu;
UINT g_CurrLangID,g_OldLangID;


//AutoHide Tab
int g_AutoHide = 0;
HANDLE g_AutoHideHandle = NULL;




//Tab Variables///////////////////////////////////////////////////////HWND g_Tab = NULL;
HWND g_Tab = NULL,g_hInfoPanel = NULL;
HWND g_TabWnd[10];
int g_iTabCount = 8;
WndAndHandle * g_QuickHideAllTray = NULL;
HWND g_TrayList,g_ProcessList;

HWND g_hGenWnd;
HWND g_hAdvWnd;



HWND g_IPList;

HWND g_hCleanStatus;





extern WORD wBKWnd;
extern UINT uModifier_BK, uMF_BK, uVK_BK;
extern HWND hBKDialog,hTitleDialog,hPropertyDialog;
extern int iShowFileDialog;
extern WndAndHandle * pTempBK;
extern BKHList g_BKHList;
extern WndAndHandle * sBKSel_con;

///define contans
#define WM_MY_TRAY_NOTIFICATION (WM_USER + 101)
#define INFO_DISPLAY WM_USER + 108



//define functions

INT_PTR CALLBACK MainProc1(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK AboutProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK HotKeyProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK PasswordProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK BKProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK QuickHideProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK MouseMotionProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK OtherProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK MultiLanguageProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK AutoUpdateProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK ReadMeProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK HistoryProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK RegisterProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK SelWndIconProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK FinishProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK BeginBKProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
unsigned __stdcall RefreshThread(void *);







BOOL CALLBACK EnumWindowsProc(HWND,LPARAM);
BOOL CALLBACK EnumWindowsForBKProc(HWND,LPARAM);
VOID CALLBACK ScanWebProc(HWND,UINT,UINT_PTR,DWORD);
int DeleteAll();
VOID CALLBACK TimerProc(HWND,UINT,UINT_PTR,DWORD);
int CreateOptionProps();
int CreateAboutProps();
int ReadOptionFile(OptionFile*);
int GarbageRecycle();
int OpenBKWizard();
int AddSelListHeader();
int UpdateSelList();
int AddConfirmListHeader();
int UpdateConfirmList();
int AddBKListHeader();
int ShowSelected(WndAndHandle * pWH,int iSize);
int RestoreAllBK(int iKorS);
int ShowWindowNormal(HWND hwndDlg);
int ExtendWindow();
int ModifyAutoReserveMenu(HMENU hMenu,int iSel);
VOID CALLBACK SetTipTextProc(HWND,UINT,UINT_PTR,DWORD);

 



DWORD WINAPI NetworkThread(LPVOID lp);

int HD_Connect(SOCKET sock_Scan,char *cHostName,int iPort);
int HD_FormatRequestHeader(char * cObject,char * cHostName,long * lHeaderLength,char * cRequestHeader);
int HD_SendRequest(SOCKET sock_Scan,char * cRequestHeader);
int HD_RecvResponseHeader(SOCKET sock_Scan,char * cResponseHeader,long * lHeaderLength);
int HD_GetHeaderField(char * cResponseHeader,char * cField,char * cValue);
int HD_GetState(char * cResponseHeader);
int HD_GarbageRecycle();
int GetFileFromServer(char * cFileName);





DWORD CALLBACK EditStreamCallback(DWORD_PTR,LPBYTE,LONG,LONG*);

LRESULT CALLBACK GetMsgProc(int ,WPARAM,LPARAM);

ExeFunc pExeFunc;

 

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








	Compatible25();






	sOP.cSerialNumber = new wchar_t[100];
	sOP.cMiddleCode = new wchar_t[100];
	sOP.cTrailRun = new wchar_t[MAX_PATH];
	//Register operation variables///////////////////////////////////////////////////////////////
	HRESULT hResult;
	int iReturn = 0,i2=0;
	wchar_t cTemp[80],*cTemp2 = NULL,*cTemp3 = NULL,cTemp4[50];
	DWORD dwError = 0,dwVolumeSerialNumber=0,dwF=0,dwL=0;

	//File operation
	cOptionFileName =  new wchar_t[1024];
	wchar_t * cOptionFileName2 =  new wchar_t[1024];
	OutputDebugString(L"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHAAAAAAAAAAAAAAAAAAAAAAA");
	memset(cOptionFileName,0,1024 * sizeof(wchar_t));
	memset(cOptionFileName2,0,1024 * sizeof(wchar_t));
 	GetModuleFileName(hInst,cOptionFileName2,1024);
 	PathRemoveFileSpec(cOptionFileName2);

	SHGetSpecialFolderPath(NULL, cOptionFileName, CSIDL_PERSONAL, FALSE);
	wcscat(cOptionFileName,L"\\HideDragon");
	CreateDirectory(cOptionFileName,NULL);

	wcscat(cOptionFileName,L"\\misc34.hdo");
	wcscat(cOptionFileName2,L"\\misc34.hdo");
	g_iUseLanguage = SetCurrentLanguage();
// 	CopyFile(cOptionFileName2,cOptionFileName,FALSE);

#ifdef DEBUG
	hOptionFile = CreateFile(cOptionFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		/*OPEN_EXISTING*/OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
		if (hOptionFile == INVALID_HANDLE_VALUE)
		{
// 			MessageBox(NULL,"文件不完整，需要重新安装隐身专家!","隐身专家",MB_OK);
// 			dwError = GetLastError();
			CopyFile(cOptionFileName2,cOptionFileName,FALSE);

			//exit(0);
		}
#else
	hOptionFile = CreateFile(cOptionFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
		if (hOptionFile == INVALID_HANDLE_VALUE)
		{
			//HDMessageBoxW(NULL,"文件不完整，需要重新安装隐身专家!","隐身专家",MB_OK);
			//dwError = GetLastError();
			//HDMessageBoxW(L"IDS_MESSAGEBOX_FILENOTFULL",L"File not full, please re install HideDragon");
			CopyFile(cOptionFileName2,cOptionFileName,FALSE);

			//exit(0);
		}

#endif
	delete [] cOptionFileName;
	delete [] cOptionFileName2;

 _CrtDumpMemoryLeaks();
	FileSizeLow = GetFileSize(hOptionFile,&FileSizeHigh);
	CloseHandle(hOptionFile);
 _CrtDumpMemoryLeaks();
	if (FileSizeLow == 0)
	{
		sOP.bUseMouse = 1;
		sOP.iRefInterval = 1;
		sOP.iKorS = 'S';
		sOP.bDisableSound = 0;
		sOP.bAutoRun = 1;
		sOP.bAddQuickLaunch = 1;
		sOP.bOnTop = 0;
		sOP.iUsedTimes = 1;
		sOP.bRegistered = IsRegister();
		sOP.bUsePassword = 0;
		memset(sOP.cPassword,0,15 * sizeof(wchar_t));
		sOP.bUseFunctionKey = 0;
		sOP.iFunctionKey = 1;
		sOP.bUseBKMouse = 0;
		sOP.bAutoHide = 0;
		sOP.bHideMain = 0;
		sOP.bHideIcon = 0;
		sOP.bReserveSel = 1;
		sOP.bNeedStart = 0;
		sOP.bAutoHide_QH = 0;
		sOP.bReserveHabit = 1;
		sOP.bShowTip = 1;
		sOP.bShowBKTip = 1;
		sOP.iAutoUpdate = ID_AUTOUPDATE_AUTODONTTIP;
		sOP.bRightFirst = 0;
		sOP.bUseMiddle = 1;
		sOP.bUseResHK = 0;
		sOP.sResHK.wHK_Res = 0;
		sOP.sResHK.uMF = 0;
		sOP.sResHK.uVK = 0;
		sOP.sHaHK.wHK_Res = 593;
		sOP.sHaHK.uMF = 2;
		sOP.sHaHK.uVK = 81;
		sOP.sSsHK.wHK_Res = 599;
		sOP.sSsHK.uVK = 87;
		sOP.sSsHK.uMF = 2;
		sOP.bHideAllTray = 1;
		sOP.bCloseInfoPanel = 1;
		sOP.bAutoChangeIconTitle = 0;
		sOP.bBeginnerModel = 1;
		sOP.bHideProcess = 0;
		sOP.bNotShowProcess = 0;
		sOP.bCleanAfterHide = 0;
		sOP.bAutoHideProgram = 0;
		sOP.doubleClick = 0;
		WriteOptionFile(sOP);
 	}
	else
	{
		ReadOptionFile(&sOP);

		ReadTimes(&sOP.iUsedTimes);


//		if(sOP.iUsedTimes > 5)
//		{
//			sOP.bBeginnerModel = 0;
//		}
		
		if(sOP.iUsedTimes)
		{
			hResult = CoCreateGuid(&hd_guid);
			GetVolumeInformation(L"C:\\",NULL,0,&dwVolumeSerialNumber,&dwL,&dwF,NULL,0);
			memset(cOrgCode,0,20 * sizeof(wchar_t));
			
			dwVolumeSerialNumber ^= 0x98989898;				
			swprintf(cOrgCode,L"%X",dwVolumeSerialNumber);
			for(i = 0;i<2;i++)
			{
				for(i2=0;i2<4;i2++)
				{
					cCode[i][i2] = cOrgCode[i*4+i2];
				}
			}
			dwVolumeSerialNumber ^= 0x68686868;
			swprintf(cOrgCode,L"%X",dwVolumeSerialNumber);
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
			memset(sOP.cSerialNumber,0,100 * sizeof(wchar_t));
			memset(cTemp,0,80 * sizeof(wchar_t));
			StringFromGUID2(hd_guid,(unsigned short*)cTemp,80);
//			iReturn = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
//				(unsigned short*)cTemp,72,sOP.cSerialNumber,80,NULL,NULL);
			wcscpy(sOP.cSerialNumber,cTemp);
			iReturn = wcslen(sOP.cSerialNumber);
			cTemp2 = wcschr(sOP.cSerialNumber,'{');
			cTemp2++;
			cTemp3 = wcschr(sOP.cSerialNumber,'}');
			memset(cTemp4,0,50 * sizeof(wchar_t));

			memcpy(cTemp4,cTemp2,cTemp3-cTemp2);
			memset(sOP.cSerialNumber,0,100 * sizeof(wchar_t));
			wcscpy(sOP.cSerialNumber,cTemp4);
			iReturn = wcslen(sOP.cSerialNumber);
			sOP.iUsedTimes ++;
			WriteTimes(sOP.iUsedTimes);
			WriteOptionFile(sOP);
		}
		lf.lfHeight = 15;
		lf.lfWidth = 0;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfWeight = 800;
		lf.lfItalic = FALSE;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = FALSE;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH;

		wcscpy(lf.lfFaceName,L"MS Sans Serif");
		hFont_New = CreateFontIndirect(&lf);
		hInst = hInstance;

		hHand = LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
		hNorm = LoadCursor(NULL,IDC_ARROW);
		hGM = SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,NULL,GetCurrentThreadId());			
		{
			// sOP.iUsedTimes ++;
			WriteOptionFile(sOP);
#ifdef DEBUG
			iEvaluateTime = 30;
#else
			iEvaluateTime = 30;
#endif


#ifdef _NOADS
			iEvaluateTime = 0;
#endif


#ifdef _NOADS
			iTryTime = 50,iTryTime_re = 50; 
#endif
			if(sOP.iUsedTimes >= iEvaluateTime &&!(sOP.bRegistered = IsRegister()))
			{
				iReturn = DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_REGISTER),NULL,RegisterProc);

				if (iReturn == 0)
				{
#ifdef DEBUG
					if(sOP.iUsedTimes > iTryTime)
					{
#ifdef _NOADS
						exit(0);
#endif
					}
#else
					if(sOP.iUsedTimes > iTryTime_re)
					{
#ifdef _NOADS
						exit(0);
#endif
					}					
#endif
				}
			}
			DeleteObject(hFont_New);

			if(IsRegister())
			{
				sOP.bRegistered = 1;
				WriteOptionFile(sOP);
			}
			else
			{
				sOP.bRegistered = 0;
				WriteOptionFile(sOP);
			}
		}
	} 


	wchar_t * pCmdLine = new wchar_t[512];
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,lpCmdLine,strlen("-D"),pCmdLine,512);
	if(_wcsnicmp(pCmdLine,L"-D",wcslen(L"-D")) != 0)
	{
   		IsHaveUpdatedFile();
		IsHaveTempDirectory();
	}







	
	//Added for support ATL 
	
	  // Initialize COM
	  CoInitialize(0);
//
//	  // Initialize the ATL module
	  //_Module.Init(0, hInstance);
//
//	  // Initialize support for control containment
	  //AtlAxWinInit();









	INITCOMMONCONTROLSEX icc;
	icc.dwSize   =  sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC    =  ICC_BAR_CLASSES|ICC_LISTVIEW_CLASSES|ICC_TAB_CLASSES;

	InitCommonControlsEx(&icc);
 _CrtDumpMemoryLeaks();

	sW = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(sW,0,sizeof(WndAndHandle) * 100);
	//Original file operation

	
	for(int i = 0;i< 100;i++)
	{
		sW[i].cWndTitle = (wchar_t *) VirtualAlloc(NULL,2048 * sizeof(wchar_t),
			MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		memset(sW[i].cWndTitle,0,2048 * sizeof(wchar_t));
		sW[i].cIconTip = new wchar_t [1024];
	}
	
//	g_iUseLanguage = SetCurrentLanguage();

	//AutoRun and Add shortcut to Quick launch area////////////////////////////////////////////////////////////
	wchar_t * cCurrentPath = NULL;

	HANDLE hWriteReg = (HANDLE)_beginthreadex(NULL,0,
		(unsigned int(__stdcall*)(void*))WriteRegThread,NULL,0,NULL);


	
	 _CrtDumpMemoryLeaks();

#ifdef _QUICK_LAUNCH
	//Quick launch area///////////////////////////////////////////////////////////////////
	SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&lpItemIdList);
	wchar_t * cPath_QuickLaunch = (wchar_t*)new wchar_t[1024];
	SHGetPathFromIDListW(lpItemIdList,cPath_QuickLaunch);
	if(CreateFileW(cPath_QuickLaunch,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL) == NULL)
	{
		//MessageBox(hwndDlg,"path not exist","隐身专家",MB_OK);
	}
	CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&pShellLink);
	pShellLink->QueryInterface(IID_IPersistFile,(void**)&ppf);	
	cCurrentPath = new wchar_t [1024];
	memset(cCurrentPath,0,1024 * sizeof(wchar_t));
	GetModuleFileNameW(NULL,cCurrentPath,1024);
	
	if (pShellLink->SetPath(cCurrentPath) == NOERROR)
	{
		//MessageBox(hwndDlg,"OK","OK",MB_OK);
	}
	wchar_t * cWorkingDic = (wchar_t *)new wchar_t [1024];
	memset(cWorkingDic,0,1024 * sizeof(wchar_t));
	wcscpy(cWorkingDic,cCurrentPath);
	PathRemoveFileSpecW(cWorkingDic);
	if(pShellLink->SetWorkingDirectory(cWorkingDic)==NOERROR)
	{
		//MessageBox(NULL,"SETOK","OK",MB_OK);
	}
	SetCurrentDirectory(cWorkingDic);
	delete [] cCurrentPath;
	delete [] cWorkingDic;
	wchar_t * cShortcutName = (wchar_t*)new wchar_t[1024];
	memset(cShortcutName,0,dwPathSize * sizeof(wchar_t));
	wcscpy(cShortcutName,cPath_QuickLaunch);
	wcscat(cShortcutName,L"\\Microsoft\\Internet Explorer\\Quick Launch\\");
	if(g_iUseLanguage == 0)
	{
		wcscat(cShortcutName,L"HideDragon 3.4");//隐身专家 2.9");
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
 		wcscat(cShortcutName,L"HideDragon 3.4"/*LoadItemStringW(L"IDS_TRAYICON_BALLTIPTITLE",pText,
 			L"HideDragon 3.21")*/);
		delete [] pText;
	}
	wcscat(cShortcutName,L".lnk");
	if(sOP.bAddQuickLaunch == 1)
	{
		hr = ppf->Save( (unsigned short *)cShortcutName,FALSE);
	}
	delete [] cShortcutName;
	delete [] cPath_QuickLaunch;

#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	hInst = hInstance;

	hHand = LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
	hNorm = LoadCursor(NULL,IDC_ARROW);
	hIcon_Unknown = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_UNKNOWN));
	hIcon_NoIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_NOICON));
	hIcon_Default = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON_DEFAULT));
 _CrtDumpMemoryLeaks();
	int ret = _wcsnicmp(pCmdLine,L"-A",wcslen(L"-A"));
	if(ret == 0)
	{
		g_iAutoRun = 1;
	}
	delete [] pCmdLine;

	iUseSkin = IsUseSkin();

	//ReadWndInfo(sW,&iBaSW);
	g_TrayWnd = FindTrayToolbarWindow();
	g_HD_Msg_GetIcon = RegisterWindowMessage(L"HD_Msg_GetIcon");	
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAINDIALOG),NULL,(DLGPROC)MainProc1);
	//AtlAxDialogBoxW(hInstance,MAKEINTRESOURCE(IDD_MAINDIALOG), NULL,(DLGPROC)MainProc1,NULL);


	
	// Terminate the ATL module
	//_Module.Term();
	////Uninitialize COM
	CoUninitialize();



	return (Msg.wParam);

}



HBRUSH hBKBrush;




INT_PTR WINAPI MainProc1(HWND hwndDlg,UINT uMsg, WPARAM wParam ,LPARAM lParam)
{
	//HMENU hSysMenu;
	NMITEMACTIVATE * pIA;
	LPTOOLTIPTEXTW lpttt;
	int idCtrl,iItem,iSelIndex=0,ret = 0;
	LVCOLUMN lvColumn;
	LVCOLUMNW lvColumnW;
	NMLISTVIEW nmListView;
	DWORD lvStyle;
	HANDLE hCurSelWnd; 
	LVITEM bkItem;
	DWORD dwE;
	//RECT rc;
	wchar_t cSubItem[20],cPID_local [10],cPID_Current[10], * cSelWndTitle;
	HFONT hOldFont;
	BOOL bSuc = TRUE;
	wchar_t cSelPID[10],cCurPID[10];
	wchar_t pTipText[1024],* pTitle = NULL;
	DWORD lSelPID = 0;
	sFileItem * pItem = NULL;
	int iAR = 0,iAlpha = 0,iSelMark = 0;
	sHMF_Item * pHMItem = NULL;
	long lStyle = 0;
	TOOLINFOW Ti_Bal;
//	wchar_t pWindowTitle[100],pProcessID[100],pTrayIcon[100];
	wchar_t pWindowTitleW[100],pProcessIDW[100],pTrayIconW[100];
	RECT rtUsage;
	HRGN hUsageRgn;
	LOGFONT lf;
	sBKWHotkey * pBHTemp = NULL;

	BKHList::iterator cNode;

	HMENU hJobTipMenu_bk = NULL;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hMainDialog = hwndDlg;

		iFlag_Size = 1;
		hStatic_Update = ::GetDlgItem(hwndDlg,IDC_STATIC_UPDATE);
		hButton_Update = ::GetDlgItem(hwndDlg,IDC_UPDATE);
		hTrayWnd = FindTrayToolbarWindow();
		DoCreateStatusBar();
		InitTab();
		SetWindowSizePos();
		
		hMouse = GetDlgItem(g_hInfoPanel,IDC_INFO_MOUSE);
		hHK_HideAll = GetDlgItem(g_hInfoPanel,IDC_HOTKEY_INFO_HA);
		hHK_ShowSelf = GetDlgItem(g_hInfoPanel,IDC_HOTKEY_INFO_SS);
		hUsageTip = GetDlgItem(hwndDlg,IDC_STATIC_USEAGETIP);
		SetTrayHook(1);

		hBKBrush = CreateSolidBrush(RGB(250,209,99));

		if(sOP.bUseMouse || sOP.bUseMiddle)
		{
			if(g_iUseLanguage == 0)
			{
				::SetWindowTextW(hMouse,L"Enable");
			}
			else
			{
				wchar_t * pText = new wchar_t[1024];
				::SetWindowTextW(hMouse,LoadItemStringW(L"IDS_MOUSE_STATUS_ENABLED",pText,L"Enable"));
				delete [] pText;
			}
		}
		else if(sOP.bUseMouse == 0 && sOP.bUseMiddle == 0)
		{
			if(g_iUseLanguage == 0)
			{
				::SetWindowTextW(hMouse,L"Disable");
			}
			else
			{
				wchar_t * pText = new wchar_t[1024];
				::SetWindowTextW(hMouse,LoadItemStringW(L"IDS_MOUSE_STATUS_DISABLED",pText,L"Disable"));
				delete [] pText;
			}
		}
		hI2 = (struct HICON__*)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,32,32,NULL);
		SendMessage(hwndDlg,WM_SETICON,ICON_BIG,(LPARAM)hI2);




		///填充ListView列
		hListView = ::GetDlgItem(hwndDlg,IDC_LISTVIEW);
		{//Test 
			HWND hTabAllWnd = g_TabWnd[0];
			hListView = ::GetDlgItem(hTabAllWnd,IDC_TAB_LIST_ALLWND);
		}
		
		{
			HWND hTabAllWnd = g_TabWnd[2];

			hList_BK = GetDlgItem(hTabAllWnd,IDC_TAB_BKLIST);


		}
		AddBKListHeader();
		
		lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
		lvColumn.fmt  = LVCFMT_LEFT;
		lvColumn.cx   = 400;
		lvColumnW.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
		lvColumnW.fmt  = LVCFMT_LEFT;
		lvColumnW.cx   = 400;
		//lvColumn.pszText = new wchar_t[100];
		//memset(lvColumn.pszText,0,100);
		//wcscpy(lvColumn.pszText,"窗口标题");
		hMainMenu = LoadMenu(hInst,MAKEINTRESOURCE(IDR_MENU1));
		hTrayMenu = GetSubMenu(hMainMenu,0);
		hWLMenu1 = LoadMenu(hInst,MAKEINTRESOURCE(IDR_MENU3));
		hWLMenu   = GetSubMenu(hWLMenu1,0);

		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_MAINDIALOG);
			SetMenuString();
		}

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
		//ListView_InsertColumn(hListView,0,&lvColumn);
		SendMessageW(hListView,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumnW);
		lvColumn.cx  = 80;
		lvColumnW.cx  = 80;
		//wcscpy(lvColumn.pszText,"进程ID");
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
		//ListView_InsertColumn(hListView,1,&lvColumn);
		SendMessageW(hListView,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumnW);
		lvColumn.cx   = 80;
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
		//ListView_InsertColumn(hListView,2,&lvColumn);
		SendMessageW(hListView,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumnW);
		//delete [] lvColumn.pszText;
	_CrtDumpMemoryLeaks();	
		lvStyle = ListView_GetExtendedListViewStyle(hListView);
		lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyleEx(hListView,0,lvStyle);
		
		//hList_BK = GetDlgItem(hwndDlg,IDC_LIST_BK);

		hTrayWnd_dll = hTrayWnd;
		if(hTrayWnd == NULL || !IsWindow(hTrayWnd))
		{
			OutputDebugString(L"Find traytoolbar window error");
		}

		hRestAll = ::GetDlgItem(hwndDlg,IDC_RESTORE);
		//HMENU hSysMenu;
		//hSysMenu = ::GetSystemMenu(hwndDlg,FALSE);
//		DeleteMenu(hSysMenu,4,MF_BYPOSITION);
//		DeleteMenu(hSysMenu,0,MF_BYPOSITION);
//		DeleteMenu(hSysMenu,1,MF_BYPOSITION);
		//Balloon tooltip add

		hBalTip = CreateWindowExW(NULL,TOOLTIPS_CLASSW,NULL,
			WS_POPUP|TTS_ALWAYSTIP|TTS_BALLOON|TTS_NOPREFIX,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwndDlg,NULL,hInst,NULL);

		memset(&Ti_Bal,0,sizeof(TOOLINFOW));
		Ti_Bal.cbSize   = sizeof(TOOLINFOW);
		Ti_Bal.hinst    = hInst;
		Ti_Bal.uFlags   = TTF_IDISHWND;
		Ti_Bal.hwnd     = hwndDlg;
		Ti_Bal.uId      = (UINT_PTR)hListView;
		Ti_Bal.lpszText = LPSTR_TEXTCALLBACKW;
		GetClientRect(hListView,&Ti_Bal.rect);
		if(SendMessageW(hBalTip,TTM_ADDTOOLW,0,(LPARAM)&Ti_Bal) == FALSE)
		{
			dwE = GetLastError();
		}
		Ti_Bal.uId      = (UINT_PTR)hMouse;
		GetClientRect(hMouse,&Ti_Bal.rect);
		if(SendMessageW(hBalTip,TTM_ADDTOOLW,0,(LPARAM)&Ti_Bal) == FALSE)
		{
			dwE = GetLastError();
		}

		SendMessageW(hBalTip,TTM_SETDELAYTIME,TTDT_INITIAL,MAKELONG(600,0));
		SendMessageW(hBalTip,TTM_SETDELAYTIME,TTDT_AUTOPOP,MAKELONG(10000,0));



 
 
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

		InitializeCriticalSection(&csDelListView);
		InitializeCriticalSection(&csFillList);


		hStatic_BKG = GetDlgItem(hwndDlg,IDC_STATIC_BKGROUP);
//		hButton_ShowBK = GetDlgItem(hwndDlg,IDC_SHOWBK);
		//ShowWindowNormal(hwndDlg);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		hMainWindow = hMainDialog;



		hCurWnd     = hMainDialog;
		//hLV = ::GetDlgItem(hCurWnd,IDC_LISTVIEW);
		hLV = hListView;
		hAll = ::GetDlgItem(hCurWnd,IDC_RESTORE);


		//iShowSelf = 87;
		aHideWindowsHotKey = GlobalAddAtom(L"HideWindowsHotKey");
		bSuc = RegisterHotKey(hMainDialog,aHideWindowsHotKey,sOP.sHaHK.uMF,sOP.sHaHK.uVK);
		if(bSuc == 0)
		{
			HDMessageBoxW(L"IDS_MESSAGEBOX_REGHIDEALLWINDOWFAILURE",L"Register Hide Window Hotkey Failure.");
		}
		SendMessage(hHK_HideAll,HKM_SETHOTKEY,sOP.sHaHK.wHK_Res,0);
		EnableWindow(hHK_HideAll,FALSE);

		aShowSelf = GlobalAddAtom(L"ShowSelf");
		bSuc = RegisterHotKey(hMainDialog,aShowSelf,sOP.sSsHK.uMF,sOP.sSsHK.uVK);
		if(bSuc == 0)
		{
			HDMessageBoxW(L"IDS_MESSAGEBOX_REGSHOWHIDEDRAGONFAILURE",L"Register Show HideDragon Failure.");
		}
		SendMessage(hHK_ShowSelf,HKM_SETHOTKEY,sOP.sSsHK.wHK_Res,0);
		EnableWindow(hHK_ShowSelf,FALSE);


		hI1 = (struct HICON__*)LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON2),IMAGE_ICON,16,16,NULL);
		

		nID.cbSize           = sizeof(NOTIFYICONDATA);
		nID.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nID.uID              = IDI_ICON2;
		nID.hIcon            = hI1;
		nID.hWnd             = hMainWindow;
		nID.uTimeout         = 15000;

		nIDW.cbSize           = sizeof(NOTIFYICONDATAW);
		nIDW.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nIDW.uID              = IDI_ICON2;
		nIDW.hIcon            = hI1;
		nIDW.hWnd             = hMainWindow;
		nIDW.uTimeout         = 15000;

		if(g_iUseLanguage == 1)
		{
			wchar_t * pTrayIconText = new wchar_t[1024];
			wcscpy(nIDW.szTip,LoadItemStringW(L"IDS_TRAYICON_TOOLTIP",pTrayIconText));
			delete [] pTrayIconText;
		}
		else
		{
			wcscpy(nIDW.szTip,L"HideDragon");
		}
		nID.uCallbackMessage = WM_MY_TRAY_NOTIFICATION;
		nIDW.uCallbackMessage = WM_MY_TRAY_NOTIFICATION;
//		if(sOP.bHideIcon == 0)
//		{
//			Shell_NotifyIconW(NIM_ADD,&nIDW);
//		}
//		else
//		{
//			//iFlag_ThroughIcon = 1;
//			TrayFlag = 1;
//			SetWindowTextW(::GetDlgItem(hwndDlg,IDC_HIDEICON),L"Hide \&Icon");
//		}
		
		
		//SetCbtHook(1);
		CreateAutoReserveFile();
		HMF_Create();

		
		
		//ChangeWindowStyle(hwndDlg,0);
		
		if(iUseSkin == 1)
		{
			ChangeWindowStyle(hwndDlg,0);
		}
		//_beginthreadex(NULL,0,RefreshThread,NULL,0,NULL);
		if (sOP.bUseMouse || sOP.bUseMiddle)
		{
			SetMouseHook(1);
			GetSystemTime(&g_StartTime);
		}

		lf.lfHeight = 15;
		lf.lfWidth = 0;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfWeight = 800;
		lf.lfItalic = FALSE;
		lf.lfUnderline = FALSE;
		lf.lfStrikeOut = FALSE;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH;
		wcscpy(lf.lfFaceName,L"MS Sans Serif");
		hFont_New = CreateFontIndirect(&lf);
		SendMessage(hUsageTip,WM_SETFONT,(WPARAM)hFont_New,TRUE);
		MakeLanguageMenu();
		g_iTipCount = 0;
		SetTimer(hwndDlg,IDT_REFRESH,1*1000/*1000*60*30*/,(TIMERPROC)TimerProc);

		if(sOP.bUseMiddle)
		{
			AppendTipText(L"IDS_USAGETIP_CLICKMIDDLEHIDEWND",
			L"Click mouse Middle button can hide all windows.");
		}
		else if(sOP.bUseMouse)
		{
			AppendTipText(L"IDS_USAGETIP_CLICKLEFTRIGHTHIDEWND",
			L"Click mouse right + left button can hide all windows.");
		}
		else
		{
			AppendTipText(L"IDS_USAGETIP_PRESSHOTKEYTOHIDEWINDOW",
			L"Press Hide Window Hotkey can hide all Windows.");	
		}
		if(sOP.bReserveSel)
		{
			AppendTipText(L"IDS_USAGETIP_SELWNDLISTCANRESERVEWND",
				
			L"You also can select unwill hide window in Window list");
		}
		else
		{
			AppendTipText(L"IDS_USAGETIP_HIDESELITEM",
			L"The selected item in the Window List will hide.");
		}

		if(sOP.bUseMiddle)
		{
			SetTipText(L"IDS_USAGETIP_CLICKMIDDLEHIDEWND",
			L"Click mouse Middle button can hide all windows.");
		}
		else if(sOP.bUseMouse)
		{
			SetTipText(L"IDS_USAGETIP_CLICKLEFTRIGHTHIDEWND",
			L"Click mouse right + left button can hide all windows.");
		}
		else
		{
			SetTipText(L"IDS_USAGETIP_PRESSHOTKEYTOHIDEWINDOW",
			L"Press Hide Window Hotkey can hide all Windows.");
		}
//		DoCreateTabControl(hMainDialog);//Temp
//		CreateInfoPanel();

		//SetTimer(hwndDlg,IDT_SCANWEB,1000 * 60 *5/*60*10*/,(TIMERPROC)ScanWebProc);
//		hNetworkThread = (HANDLE)_beginthreadex(NULL,0,
//			(unsigned int(__stdcall*)(void*))NetworkThread,NULL,0,NULL);
		SendMessage(hwndDlg,WM_NCACTIVATE,NULL,NULL);
		if(iUseSkin == 0)
		{
			FillList();
		}
		if(iTakeToBk == 1)
		{
			FillList();
			BKHided();
		}
		hBtnExpandInfo = GetDlgItem(hwndDlg,IDC_STATIC_EXPANDINFO);





		if(sOP.bShowTip == 1)
		{
//			wchar_t * pText = NULL;
//			pText = new wchar_t[1024];
//
//			wb_Expand->SetToolTip(LoadItemStringW(L"IDS_TIP_CLICKTOEXPANDINFO",pText,
//				L"Click to expand info panel"));
/*			delete [] pText;*/
		}



//		wpOrgExpandProc =(WNDPROC) SetWindowLong(hBtnExpandInfo,GWL_WNDPROC,
//			(LONG)ExpandBtnSubclassProc);

		SetClassLong(GetDlgItem(hwndDlg,IDC_STATIC_EXPANDINFO),    // window handle 
			GCL_HCURSOR,      // change cursor 
			(LONG) NULL);   // new cursor 
		SetClassLong(GetDlgItem(hwndDlg,IDC_HIDEICON),    // window handle 
			GCL_HCURSOR,      // change cursor 
			(LONG) NULL);   // new cursor 

		if(sOP.bNotShowProcess == 1)
		{
			DWORD dwPID = 0;
			GetWindowThreadProcessId(hwndDlg,&dwPID);
			HideProcess(dwPID);
			g_HidedProcess = 1;
		}

		SendMessage(hMainDialog,WM_COMMAND,IDC_STATIC_USEAGETIP,0);

		
		
		if(sOP.bHideIcon == 0)
		{
			Shell_NotifyIconW(NIM_ADD,&nIDW);
		}
		else
		{
			//iFlag_ThroughIcon = 1;
			TrayFlag = 1;
			if(g_iUseLanguage == 0)
			{
				SetWindowTextW(::GetDlgItem(g_hInfoPanel,
					IDC_INFO_HIDEICON),L"Show Icon");
			}
			else
			{
				SetDlgItemStringW(GetDlgItem(g_hInfoPanel,IDC_INFO_HIDEICON),
					IDD_MAINDIALOG,IDC_SHOWICON);
			}
		}
		
		
		if(sOP.bAutoChangeIconTitle == 1)
		{
			CreateAutoChangeThread();
		}


#ifndef _NOADS
		CreateGetAdsLinkThread();
#endif



		if(sOP.iUsedTimes == 0)
		{

			wchar_t * cUrl = new wchar_t[1024];
			memset(cUrl,0,1024 * sizeof(wchar_t));
			
			ShellExecute(NULL,L"open",L"IEXPLORE",
				LoadItemStringW(L"IDS_HELPLINK_FIRSTRUN",cUrl,L"http://www.hidedragon.com/cn/firstrun.html"),NULL,SW_SHOW);
			delete [] cUrl;
			cUrl = NULL;

		}
		

		
		if(sOP.bAutoHideProgram && g_AutoHideHandle ==  NULL)
		{
			CreateAutoHideThread();
		}

		g_UseIcon = 0;


		//SetWebControlPos();
		iFlag_EndInit = 1;
 _CrtDumpMemoryLeaks();
		return FALSE;
//	case WM_SETCURSOR:
//		{
//			HWND hExpandBtn = GetDlgItem(hwndDlg,IDC_STATIC_EXPANDINFO);
//			HWND hTipBtn = GetDlgItem(hwndDlg,IDC_HIDEICON);
//			RECT rtExpand,rtTip;
//			int iForOut = 0;
//			int iForLink = 0;
//			int iForIn = 0;
//
//			memset(&rtExpand,0,sizeof(RECT));
//			memset(&rtTip,0,sizeof(RECT));
//			GetWindowRect(hExpandBtn,&rtExpand);
//			GetWindowRect(hExpandBtn,&rtTip);
//			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtExpand,2);
//			MapWindowPoints(GetDesktopWindow(),hwndDlg,(tagPOINT*)&rtTip,2);
//			POINT pMouse;
//			GetCursorPos(&pMouse);
//			ScreenToClient(hwndDlg,&pMouse);
//			if (PtInRect(&rtExpand,pMouse)||PtInRect(&rtTip,pMouse))
//			{
////				iForOut = 0;
//
//
//				
//				SetCursor(hHand);
//
////				MessageBoxW(NULL,L"DDDD",L"DDD",MB_OK);
////				iForLink = 0;
//				
////				if ((++iForIn)  <= 1)
////				{
////					if(PtInRect(&rtExpand,pMouse))
////						InvalidateRect(hExpandBtn,NULL,FALSE);
////					else
////						InvalidateRect(hExpandBtn,NULL,FALSE);
////				}
//			}
//			else
//			{
////				iForIn = 0;
////				iForLink = 1;
//				SetCursor(hNorm);
//				//SendMessage(hwndDlg,WM_CTLCOLORSTATIC,(WPARAM)hHttpDC,(LPARAM)hHttp);
////				if((++iForOut) <= 1)
////				{
////					InvalidateRect(hExpandBtn,NULL,FALSE);
////					InvalidateRect(hExpandBtn,NULL,FALSE);
////				}
//
//			}
//		return TRUE;
//		}
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hUsageTip)
		{
//			hBKBrush = CreateSolidBrush(RGB(250,209,99));
			SetBkColor((HDC)(wParam),GetSysColor(COLOR_3DFACE));
			GetClientRect(hUsageTip,&rtUsage);
			hUsageRgn = CreateRoundRectRgn(rtUsage.left,rtUsage.top,rtUsage.right,
				rtUsage.bottom,6,6);
			SelectClipRgn((HDC)(wParam),hUsageRgn);
			DeleteObject(hUsageRgn);
			SetBkColor((HDC)(wParam),RGB(250,209,99));
			return (LRESULT)hBKBrush;
		}
		else
		{
			SelectClipRgn((HDC)(wParam),NULL);
			return FALSE;
		}
		return FALSE;
	case WM_PAINT:
		return FALSE;
	case WM_NOTIFY:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC = GetWindowDC(hwndDlg);
			if(hWinDC)
			{
				if(iUseSkin == 1)
				{
					DrawTitleBar(hWinDC);
				}
			}
			hOldFont = (HFONT)::SelectObject(hWinDC,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
			if(hOldFont)
			{
				DeleteObject(hOldFont);
			}
			else
			{ 
				OutputDebugString(L"The hOldFont Is NULL.");
			}

			ReleaseDC(hwndDlg,hWinDC);
		}
	

//		if(((LPNMHDR)lParam)->hwndFrom == hList_BK)
//		{
//			switch(((LPNMHDR)lParam)->code)
//			{
//			case TTN_GETDISPINFO:
//				break;
//			default:
//				break;
//			}
//		}
		switch((((LPNMHDR)lParam)->code ) )
		{
		case  TTN_GETDISPINFO:
			idCtrl = ::GetDlgCtrlID((HWND)((LPNMHDR)lParam)->idFrom);
			lpttt  = (LPTOOLTIPTEXTW)lParam;
			switch(idCtrl)
			{
			case IDC_LISTVIEW:
				if(sOP.bShowTip == 1)
				{
					if(g_iUseLanguage == 1)
					{
						
						//pTipText = new wchar_t[1024];

						//wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_LISTVIEW",pTipText));
						lpttt->lpszText = LoadItemStringW(L"IDS_TIP_LISTVIEW",pTipText);
						//delete [] pTipText;
					}
					else
					{
						lpttt->lpszText = L"Select unhide or to hide window in here,or "
							L"click right mouse button to access more functions.";
						//wcscpy(lpttt->lpszText,L"Select unhide or to hide window in here,or "
						//	L"click right mouse button to access more functions.");
					}
				}
				break;
			case IDC_MOUSE:
				if(sOP.bShowTip == 1)
				{
					lpttt->lpszText = L"If enable mouse ,press left + right can hide window.";
				}
				break;
			default:
				break;
			}
			break;
		case NM_SETFOCUS:
			bRef = 0;
			break;
		case TCN_SELCHANGING:
		case TCN_SELCHANGE:
		  {
				int iPage = TabCtrl_GetCurSel(g_Tab);
				OnTabSelChange(iPage);
				break;
		  }

		case NM_KILLFOCUS:
			bRef = 1;
			break;
		case NM_CLICK:			
			if(sOP.bBeginnerModel== 0)
			{
				break;
			}
		case NM_RCLICK:
			pIA = (NMITEMACTIVATE*)lParam;
			if(/*pIA->iItem != -1 && */pIA->hdr.idFrom == IDC_TAB_LIST_ALLWND)
			{
				pTitle = (wchar_t *)VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				memset(pTitle,0,2048 * sizeof(wchar_t));
				iSelectedItem = pIA->iItem;
				ListView_GetItemText(hListView,pIA->iItem,0,pTitle,2046 * sizeof(wchar_t));
//				LVITEMW lvItemW;
//				memset(&lvItemW,0,sizeof(lvItemW));
//				lvItemW.iSubItem = 0;
//				lvItemW.iItem = pIA->iItem;
//				lvItemW.pszText = pTitle;
//				lvItemW.cchTextMax = 2048;
//				SendMessageW(hListView,LVM_GETITEMTEXTW,0,(LPARAM)&lvItemW);
				GetCursorPos(&CurMouPos);
				for(i = 0;i < iBaSW;i++)
				{
					if(wcscmp(sW[i].cWndTitle,pTitle) == 0)
					{
						pItem = new sFileItem;
						pItem->cFileName = sW[i].cProcessName;
						pItem->lSize = wcslen(sW[i].cProcessName) * 2;
						
						{
							DBClass arDB;

							if(arDB.IsInARFL(pItem))
							{
								if(g_iUseLanguage == 0)
								{
									ModifyMenuW(hWLMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
										IDM_AUTORESERVE,L"Cancel &Auto Selected");
								}
								else if(g_iUseLanguage != 0)
								{
									wchar_t * pAutoReserve = new wchar_t[1024];
									ModifyMenuW(hWLMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
									IDM_AUTORESERVE,LoadItemStringW(L"IDM_CANCELAUTORESERVE",pAutoReserve,
									L"Cancel &Auto Selected"));
									delete [] pAutoReserve;
								}
							}
							else
							{
								if(g_iUseLanguage == 0 )
								{
									ModifyMenuW(hWLMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
									IDM_AUTORESERVE,L"&Auto select this item in next");
								}
								else if(g_iUseLanguage != 0)
								{
									wchar_t * pAutoReserve = new wchar_t[1024];
									ModifyMenuW(hWLMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
									IDM_AUTORESERVE,LoadItemStringW(L"IDM_AUTORESERVE",pAutoReserve,
									L"&Auto select this item in next"));
									delete [] pAutoReserve;
								}
							}
						}
						delete pItem;
					}
				}
				if(ListView_GetCheckState(hListView,iSelectedItem) == FALSE)
				{
					if(g_iUseLanguage == 0)
					{
						ModifyMenuW(hWLMenu,IDM_AUTORESERVE,MF_BYCOMMAND,IDM_RESERVE,L"&Check this item.");
					}
					else
					{
						wchar_t * pReserve = new wchar_t[1024];
						ModifyMenuW(hWLMenu,IDM_RESERVE,MF_BYCOMMAND,
						IDM_RESERVE,LoadItemStringW(L"IDM_RESERVE",pReserve,
						L"&Check this item"));
						delete [] pReserve;
					}
				}
				else
				{
					if(g_iUseLanguage == 0)
					{
						ModifyMenuW(hWLMenu,IDM_RESERVE,MF_BYCOMMAND,IDM_RESERVE,
							L"&Uncheck this item");
					}
					else
					{
						wchar_t * pReserve = new wchar_t[1024];
						ModifyMenuW(hWLMenu,IDM_RESERVE,MF_BYCOMMAND,
						IDM_RESERVE,LoadItemStringW(L"IDM_CANCELRESERVE",pReserve,
						L"&Uncheck this item"));
						delete [] pReserve;
					}
				}

				if(pIA->iItem <= -1)
				{
					EnableMenuItem(hWLMenu,IDM_TOBK,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_BKWNDHOTKEY,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_UNREGISTERHK,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_RESERVE,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_AUTORESERVE,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_CHANGEICON,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_RESTOREICONTITLE,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_CHANGETITLE,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_PROPERTY,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_SETTOTOP,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,IDM_ADDAUTOHIDE,MF_DISABLED|MF_GRAYED);
					EnableMenuItem(hWLMenu,15,MF_DISABLED|MF_GRAYED|MF_BYPOSITION);

				}
				else
				{
					EnableMenuItem(hWLMenu,IDM_TOBK,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_BKWNDHOTKEY,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_UNREGISTERHK,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_RESERVE,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_AUTORESERVE,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_CHANGEICON,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_RESTOREICONTITLE,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_CHANGETITLE,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_PROPERTY,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_SETTOTOP,MF_ENABLED);
					EnableMenuItem(hWLMenu,IDM_ADDAUTOHIDE,MF_ENABLED);
					EnableMenuItem(hWLMenu,15,MF_ENABLED|MF_BYPOSITION);

				}

				TrackPopupMenu(hWLMenu,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
				VirtualFree(pTitle,0,MEM_RELEASE);
			}
			break;
		case NM_DBLCLK:
			pIA = (NMITEMACTIVATE*)lParam;
			if(pIA->hdr.idFrom == IDC_TAB_LIST_ALLWND)
			{
				if(sOP.doubleClick == 2)
				{
					if(ListView_GetCheckState(hListView,pIA->iItem) == FALSE)
					{
						ListView_SetCheckState(hListView,pIA->iItem,TRUE);
					}
					else
					{
						ListView_SetCheckState(hListView,pIA->iItem,FALSE);
					}
				}
				else if(sOP.doubleClick == 1)
				{
					SendMessage(hMainDialog,WM_COMMAND,IDM_SETTOTOP,lParam);
				}
				else if(sOP.doubleClick == 0)
				{
					SendMessage(hMainDialog,WM_COMMAND,IDM_TOBK,lParam);
				}
			}
			break;
		case LVN_ITEMCHANGED:
			if (iForState == 1)
			{
				break;
			}
			nmListView = *((LPNMLISTVIEW)lParam);
			iItem = ((LPNMLISTVIEW)lParam) ->iItem;
			cSelWndTitle = (wchar_t*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
			if ( iItem >= 0)
			{
				if(nmListView.uNewState == 8192 && nmListView.uOldState != 0)
				{
					//EnterCriticalSection(&csFillList);
					//MessageBox(hwndDlg,"ok","ok",MB_OK);
					ListView_GetItemText(hListView,iItem,0,sWP.cT[iTotalSelected],78);
					ListView_GetItemText(hListView,iItem,1,sWP.cP[iTotalSelected],78);
					iTotalSelected ++;

					//2.2 method ////////
					ListView_GetItemText(hListView,iItem,1,cPID_Current,10);
					ListView_GetItemText(hListView,iItem,0,cSelWndTitle,1024);
					for(int i2 = 0;i2 < iBaSW;i2++)
					{
						swprintf(cPID_local,L"%X",sW[i2].lPID);
						if (wcscmp(cPID_local,cPID_Current) == 0)
						{
							if(wcscmp(wcslwr(sW[i2].cProcessName),L"iexplore.exe")==0 &&
								wcscmp(sW[i2].cWndTitle,cSelWndTitle)==0)
							{
								sW[i2].bReserved = 1;
								hCurSelWnd = sW[i2].hWnd;
							}
							else if(wcscmp(wcslwr(sW[i2].cProcessName),L"explorer.exe")==0&&
								wcscmp(sW[i2].cWndTitle,cSelWndTitle)== 0)
							{
								sW[i2].bReserved = 1;
								hCurSelWnd = sW[i2].hWnd;
							}
							else if(wcscmp(wcslwr(sW[i2].cProcessName),L"iexplore.exe")!=0 &&
								wcscmp(wcslwr(sW[i2].cProcessName),L"explorer.exe")!=0)
							{
								sW[i2].bReserved = 1;
								hCurSelWnd = sW[i2].hWnd;
							}
							if(sOP.bReserveHabit == 1)
							{
								pHMItem = new sHMF_Item;
								pHMItem->cName = sW[i2].cProcessName;
								pHMItem->lSize = wcslen(sW[i2].cProcessName) * 2;
								pHMItem->iTimes = 1;
								{
									DBClass dbHM;
									
									if(dbHM.IsInHM(pHMItem))//HMF_Modify(pHMItem,HMF_ISHAVEITEM))
									{
										dbHM.AddHMTime(pHMItem);
									}
									else
									{
										dbHM.InsertHMItem(pHMItem);
									}
								}
								pHMItem->cName = NULL;
								delete [] pHMItem;
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

					ExeData * pED = new ExeData;
					pED->sWP = sW;
					pED->iCount = iBaSW;
					pED->pArr_QH = Array_QH;
					pED->iUseLanguage = g_iUseLanguage;
					pED->pTipCount = &g_iTipCount;
					pED->pTipText = (wchar_t ***)g_pTipText;
					ReserveWnds(pED);
					delete pED;

					//LeaveCriticalSection(&csFillList);	
				}
				if(nmListView.uNewState == 4096 && nmListView.uOldState != 0)
				{
					//EnterCriticalSection(&csFillList);
					ListView_GetItemText(hListView,iItem,1,cSubItem,18);


					//2.2 method//////////////////////////////////////////////////////////
					ListView_GetItemText(hListView,iItem,1,cPID_Current,10);
					ListView_GetItemText(hListView,iItem,0,cSelWndTitle,1024);
					

					for (int i2 = 0;i2<iBaSW;i2++)
					{
						swprintf(cPID_local,L"%X",sW[i2].lPID);

						if(wcscmp(cPID_local,cPID_Current)== 0 && wcscmp(cSelWndTitle,sW[i2].cWndTitle)==0 &&
							sW[i2].iWindowType == 0)
						{
							hCurSelWnd = sW[i2].hWnd;
						}
					}
					for(i2 = 0;i2 < iBaSW;i2++)
					{
						swprintf(cPID_local,L"%X",sW[i2].lPID);

						if (::GetParent(sW[i2].hWnd) == hCurSelWnd)
						{
							sW[i2].bReserved = 0;
						}
						else if (wcscmp(cPID_local,cPID_Current)==0 && wcscmp(cSelWndTitle,sW[i2].cWndTitle)==0 /*&& 
							(wcscmp(wcslwr(sW[i2].cProcessName),L"iexplore.exe") == 0||
							wcscmp(wcslwr(sW[i2].cProcessName),L"explorer.exe") == 0)*/
							&& sW[i2].bReserved == 1 )
						{
							pItem = new sFileItem;
							pItem->lSize = wcslen(sW[i2].cProcessName) * 2;
							pItem->cFileName = sW[i2].cProcessName;

							pHMItem = new sHMF_Item;
							pHMItem->cName = sW[i2].cProcessName;
							pHMItem->lSize = wcslen(sW[i2].cProcessName) * 2;
							pHMItem->iTimes = 1;
							{
								DBClass dbAR_HM;
								dbAR_HM.DelARItem(pItem);
								
								if(dbAR_HM.IsInHM(pHMItem))// HMF_Modify(pHMItem,HMF_ISHAVEITEM))
								{
									dbAR_HM.DelHMItem(pHMItem);
								}
								pHMItem->cName = NULL;
								delete [] pHMItem;
								pItem->cFileName = NULL;
								delete pItem;
								sW[i2].bReserved = 0;
								break;
							}
						}
						else if (wcscmp(cPID_local,cPID_Current) == 0)
						{
							sW[i2].bReserved = 0;
						}
						if(sOP.bReserveHabit == 1 )
						{
							continue;
						}
					}

					iTotalSelected = iTemp;

					ExeData * pED = new ExeData;
					pED->sWP = sW;
					pED->iCount = iBaSW;
					pED->pArr_QH = Array_QH;
					pED->iUseLanguage = g_iUseLanguage;
					pED->pTipCount = &g_iTipCount;
					pED->pTipText = (wchar_t ***) g_pTipText;
					ReserveWnds(pED);
					delete pED;

					//LeaveCriticalSection(&csFillList);				
				}
			}
			VirtualFree(cSelWndTitle,0,MEM_RELEASE);

			break;
		/*case LVN_ODSTATECHANGED:
			MessageBox(hwndDlg,"od","od",MB_OK);
			break;*/

		}
		return 0;
		
	case WM_MOVE:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC = GetWindowDC(hwndDlg);
			if(hWinDC)
			{
				if(iUseSkin == 1)
				{
					DrawTitleBar(hWinDC);
				}
				hOldFont = (HFONT)::SelectObject(hWinDC,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC);
			}
			else if (hWinDC == NULL)
			{
				//MessageBox(hwndDlg,"DDDDDDDDDDDDDDDDDDD","HD",MB_OK);
			}
		}
		return FALSE;

	case WM_NCPAINT:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC2 = GetWindowDC(hwndDlg);
			if(hWinDC2)
			{
				if(iUseSkin == 1)
				{
					DrawTitleBar(hWinDC2);
				}
 				hOldFont = (HFONT)::SelectObject(hWinDC2,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC2);
			}
		}

		return FALSE;
	case WM_NCACTIVATE:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC = GetWindowDC(hwndDlg);
//			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			if(hWinDC)
			{
				if(iUseSkin == 1)
				{
					DrawTitleBar(hWinDC);
				}
				hOldFont = (HFONT)::SelectObject(hWinDC,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC);
			}
			else
			{
				OutputDebugString(L"WM_NCACTIVATE HDC Is Null.");
			}
		}
		return FALSE;
	case WM_ACTIVATE:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC = GetWindowDC(hwndDlg);
//			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			if(hWinDC)
			{
				if(iUseSkin == 1)
				{
					DrawTitleBar(hWinDC);
				}
 				hOldFont = (HFONT)::SelectObject(hWinDC,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC);
			}
			else if (hWinDC == NULL)
			{
				//MessageBox(hwndDlg,"DDDDDDDDDDDDDDDDDDD","HD",MB_OK);
			}
			SYSTEMTIME CurTime;
			memset(&CurTime,0,sizeof(CurTime));
			if(g_iAutoRun == 1 && g_iStarted == 0 && sOP.bUseMouse)
			{
				if(g_StartTime.wYear == 0 && sOP.bUseMouse == 1)
				{
					return FALSE;
				}
				GetSystemTime(&CurTime);
				if(CurTime.wSecond - g_StartTime.wSecond <= 5)
				{
					return FALSE;
				}
				else
				{
					g_iStarted = 1;
				}
			}
			if(IsWindowVisible(hwndDlg))
			{
				SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
				SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
				FillTrayList();
			}
		}

		return FALSE;
	case WM_NCMOUSEMOVE:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC3 = GetWindowDC(hwndDlg);
			if(hWinDC3)
			{
				if(iUseSkin == 1)
				{
					OnNCMouseMove(hWinDC3,lParam);
					DrawTitleBar(hWinDC3);
				}
 				hOldFont = (HFONT)::SelectObject(hWinDC3,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC3);
			}
		}
		return FALSE;
	case WM_NCLBUTTONDOWN:
		if(IsWindow(hwndDlg))
		{
			hMainDialog = hwndDlg;
			hWinDC_LD = GetWindowDC(hwndDlg);
			if(hWinDC)
			{
				if(iUseSkin == 1)
				{
					OnNCLBDown(hWinDC_LD,lParam);
				}
				hOldFont = (HFONT)::SelectObject(hWinDC_LD,
					(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC_LD);
			}
		}
		return 0;
	case WM_NCLBUTTONUP:
//		if(IsWindow(hwndDlg))
//		{
//			hMainDialog = hwndDlg;
//			hWinDC = GetWindowDC(hwndDlg);
//			if(hWinDC)
//			{
//				OnNCLBUp(hWinDC,lParam);
//			}
//			ReleaseDC(hwndDlg,hWinDC);
//		}
		return FALSE;


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
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO pMI = (LPMINMAXINFO)lParam;
			pMI->ptMinTrackSize.x=550;   
			pMI->ptMinTrackSize.y=550;   
			RECT rtDesktop;
			HWND hDesktop;
			hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop,&rtDesktop);
			
			pMI->ptMaxTrackSize.x= rtDesktop.right - rtDesktop.left;   
			pMI->ptMaxTrackSize.y= rtDesktop.bottom - rtDesktop.top;
			return 0;
		}
		break;
	case WM_SIZING:
		break;
	case WM_SIZE:
		{
			ResizeInfoPanel();
			ResizeTabWnd();
		}
		if(IsWindow(hwndDlg))
		{
			if(iFlag_Size == 1 && iFlag_EndInit == 1 && sOP.bHideMain == 1)
			{
				ShowWindow(hwndDlg,SW_HIDE);
				iFlag_Size = 0;
				iFlag_EndInit = 0;
				iFlag_ThroughIcon = 1;
			}
			hMainDialog = hwndDlg;
			hWinDC = GetWindowDC(hwndDlg);
			if(hWinDC)
			{
				if(iUseSkin == 1)
				{
					DrawTitleBar(hWinDC);
				}
 				hOldFont = (HFONT)::SelectObject(hWinDC,(HGDIOBJ)GetStockObject(SYSTEM_FONT));
				if(hOldFont)
				{
					DeleteObject(hOldFont);
				}
				else
				{
					OutputDebugString(L"The hOldFont Is NULL.");
				}
				ReleaseDC(hwndDlg,hWinDC);
			}
			else if (hWinDC == NULL)
			{
#ifdef _DEBUG
				MessageBox(hwndDlg,L"DDDDDDDDDDDDDDDDDDD",L"HD",MB_OK);
#endif
			}
		}

		if (wParam == SIZE_MINIMIZED)
		{
			ShowWindow(hwndDlg,SW_HIDE);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,NULL);
			if(TrayFlag == 0)
			{
				if(iFlag_ThroughIcon == 0)
				{
					nIDW.cbSize           = sizeof(NOTIFYICONDATA);
					nIDW.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE|NIF_INFO;
					nIDW.uID              = IDI_ICON2;
					nIDW.hIcon            = hI1;
					nIDW.hWnd             = hMainWindow;
					nIDW.uTimeout         = 15000;
					if(g_iUseLanguage == 1)
					{
						wchar_t * pBallTipText = new wchar_t[1024];
						wcscpy(nIDW.szInfoTitle,LoadItemStringW(L"IDS_TRAYICON_BALLTIPTITLE",
							pBallTipText));
						delete [] pBallTipText;
					}
					else
					{
						wcscpy(nIDW.szInfoTitle,L"HideDragon 3.31");
					}
					if(sOP.bShowTip == 1)
					{
						if(g_iUseLanguage == 1)
						{
							wchar_t * pBallTipText= new wchar_t[1024];
							wcscpy(nIDW.szInfo,LoadItemStringW(L"IDS_TRAYICON_BALLTIP",pBallTipText));
							delete [] pBallTipText;
						}
						else
						{
							wcscpy(nIDW.szInfo,L"HideDragon has minimized, "
								L"you can right click this icon to access more function.");
						}
					}
					else
					{
						wcscpy(nIDW.szInfo,L"");
					}
					nIDW.dwInfoFlags = NIIF_INFO;
					if(g_iUseLanguage == 1)
					{
						wchar_t * pTrayIconText = new wchar_t[1024];
						wcscpy(nIDW.szTip,LoadItemStringW(L"IDS_TRAYICON_TOOLTIP",pTrayIconText));
						delete [] pTrayIconText;
					}
					else
					{
						wcscpy(nIDW.szTip,L"HideDragon");
					}
					nIDW.uCallbackMessage = WM_MY_TRAY_NOTIFICATION;
					Shell_NotifyIconW(NIM_MODIFY,&nIDW);
				}
			}
			else
			{
				if(iFlag_Restore == 0 && sOP.bShowBKTip == 1)
				{
					ShowBackTipDialog();
				}
			}
			iFlag_ThroughIcon = 0;

			return 0;
		}
		return 0;
	case WM_HOTKEY:
		if ((HIWORD(lParam) == sOP.sHaHK.uVK && LOWORD(lParam) == sOP.sHaHK.uMF)
			||HIWORD(lParam) == sOP.iFunctionKey)
		{
			FillList();
			MyHideWindows();
			FillList();
			return 1;
		}
		if (HIWORD(lParam) == sOP.sSsHK.uVK && LOWORD(lParam) == sOP.sSsHK.uMF)
		{
			SendMessage(hMainDialog,WM_COMMAND,IDM_SHOWWINDOW,1);
			return 1;
		}

		if(IsWindow(hWizardDialog))//BKREGISTERED用的内存和WIZARD对话框用的内存是同一块内存
		{
			return 0;
		}
		for(cNode = g_BKHList.begin();cNode != g_BKHList.end(); cNode ++)
		{
			sBKWHotkey * pC =  *cNode;
			if(HIWORD(lParam) == pC->uVK && LOWORD(lParam) == pC->uMF)
			{
				if(wcscmp(pC->cPID,L"") && wcscmp(pC->cTitle,L"") && wcscmp(pC->cProcessName,L""))
				{
					FillList();
					BKRegistered(pC);//pC->cTitle,pC->cPID,pC->cProcessName);
					break;
				}
			}
		} 
		
		return 1;
	case WM_MY_TRAY_NOTIFICATION:
		switch(lParam)
		{
		case WM_LBUTTONDOWN:
			if(sOP.bBeginnerModel == 0)
			{
				if(GetActiveWindow() != hwndDlg && IsWindowVisible(hMainDialog))
				{
					SetForegroundWindow(hwndDlg);
				}
				else
				{
					SendMessage(hwndDlg,WM_COMMAND,IDM_SHOWWINDOW,0);
				}
				break;
			}
		case WM_RBUTTONDOWN:
			if(sOP.bUsePassword == 1&& (IsWindowVisible(hMainDialog) == FALSE))
			{
				return FALSE;
			}
			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			SetMenuDefaultItem(hTrayMenu,IDM_SHOWWINDOW,FALSE);
			SetForegroundWindow(hwndDlg);
			GetCursorPos(&CurMouPos);
			if (::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE)
			{
				if(g_iUseLanguage == 0)
				{
					ModifyMenuW(hTrayMenu,IDM_SHOWWINDOW,
						MF_BYCOMMAND,IDM_SHOWWINDOW,L"Hide &HideDragon");
				}
				else
				{
					wchar_t * pMenuText = new wchar_t[1024];
					ModifyMenuW(hTrayMenu,IDM_SHOWWINDOW,MF_BYCOMMAND,IDM_SHOWWINDOW,
						LoadItemStringW(L"IDM_HIDEWINDOW",pMenuText));
					delete [] pMenuText;
				}
			}
			else if(!(::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE))
			{
				if(g_iUseLanguage == 0)
				{
					ModifyMenuW(hTrayMenu,IDM_SHOWWINDOW,MF_BYCOMMAND,
						IDM_SHOWWINDOW,L"Show &HideDragon");
				}
				else
				{
					wchar_t * pMenuText = new wchar_t[1024];
					ModifyMenuW(hTrayMenu,IDM_SHOWWINDOW,MF_BYCOMMAND,IDM_SHOWWINDOW,
						LoadItemStringW(L"IDM_SHOWWINDOW",pMenuText));
					delete [] pMenuText;
				}
			}
			if(sOP.bShowTip == 1)
			{
				CheckMenuItem(hTrayMenu,IDM_HIDETIP,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hTrayMenu,IDM_HIDETIP,MF_BYCOMMAND|MF_UNCHECKED);
			}

			if (::IsWindow(hAboutDialog)|| ::IsWindow(hOptionDialog)|| ::IsWindow(hRegDialog) ||
				IsWindow(hWizardDialog) || IsWindow(hBKDialog) || IsWindow(hTitleDialog)
				|| IsWindow(hPropertyDialog) ||iShowFileDialog == 1)
			{
				EnableMenuItem(hTrayMenu,IDM_ABOUT,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_OPTION,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_SHOWWINDOW,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_REGISTER,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hTrayMenu,IDM_SPECBK,MF_DISABLED|MF_GRAYED);

			}
			else if (!::IsWindow(hAboutDialog)||!::IsWindow(hOptionDialog) || !IsWindow(hRegDialog)||
				!IsWindow(hWizardDialog)|| !IsWindow(hBKDialog) || !IsWindow(hTitleDialog)
				|| !IsWindow(hPropertyDialog) || iShowFileDialog == 0)
			{
				EnableMenuItem(hTrayMenu,IDM_ABOUT,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_OPTION,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_SHOWWINDOW,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_REGISTER,MF_ENABLED);
				EnableMenuItem(hTrayMenu,IDM_SPECBK,MF_ENABLED);
			}

			if (sOP.bRegistered == 1)
			{
				EnableMenuItem(hTrayMenu,IDM_REGISTER,MF_DISABLED|MF_GRAYED);
			}
			{
				int iMenuCount = GetMenuItemCount(hTrayMenu);
				if(iMenuCount >= 16)
				{
					DeleteMenu(hTrayMenu,1,MF_BYPOSITION);
					DeleteMenu(hTrayMenu,IDM_REGISTER,MF_BYCOMMAND);

				}
			}
			//DeleteMenu(hTrayMenu,IDM_BLOG,MF_BYCOMMAND);
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_ONTOP) == BST_CHECKED)
			{
				CheckMenuItem(hTrayMenu,IDM_TRAYONTOP,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hTrayMenu,IDM_TRAYONTOP,MF_BYCOMMAND|MF_UNCHECKED);
			}
			{
				int iMenuCount = GetMenuItemCount(hTrayMenu);
				if(iMenuCount > 7)
				{
					DeleteMenu(hTrayMenu,IDM_BLOG,MF_BYCOMMAND);
					DeleteMenu(hTrayMenu,IDM_VIDEOTURORIAL,MF_BYCOMMAND);
					DeleteMenu(hTrayMenu,IDM_ABOUT,MF_BYCOMMAND);
					DeleteMenu(hTrayMenu,IDM_SPECBK,MF_BYCOMMAND);
					DeleteMenu(hTrayMenu,IDM_HIDETIP,MF_BYCOMMAND);
					//DeleteMenu(hTrayMenu,IDM_EXIT,MF_BYCOMMAND);
					DeleteMenu(hTrayMenu,1,MF_BYPOSITION);
					DeleteMenu(hTrayMenu,3,MF_BYPOSITION);
					DeleteMenu(hTrayMenu,6,MF_BYPOSITION);
				}
			}


			TrackPopupMenu(hTrayMenu,0,CurMouPos.x,CurMouPos.y,0,hwndDlg,NULL);
			break;
		default:
			return FALSE;
		}
		return FALSE;
	case WM_INITMENUPOPUP:
		if(IsWindow(hwndDlg) == 0)
			return 0;
		hMainWndMenu = GetMenu(hwndDlg);
		hFileMenu = GetSubMenu(hMainWndMenu,0);
		hJobMenu = GetSubMenu(hMainWndMenu,1);
		hAlphaMenu = GetSubMenu(hJobMenu,1);
		hDisguiseMenu = GetSubMenu(hMainWndMenu,2);
		hBKMenu = GetSubMenu(hMainWndMenu,3);
		hHelpMenu = GetSubMenu(hMainWndMenu,5);
		
		hJobTipMenu = LoadMenuW(hInst,MAKEINTRESOURCE(IDR_MENU_JOBTIP));
		hJobTipMenu_bk = hJobTipMenu;
		hJobTipMenu = GetSubMenu(hJobTipMenu,0);
		{
			MENUITEMINFOW MII;
			MII.cbSize = sizeof(MII);
			MII.fMask = MIIM_FTYPE;
			MII.fType = MFT_RADIOCHECK;
			BOOL bRet = SetMenuItemInfoW(g_hLangMenu,g_CurrLangID,FALSE,&MII);
			DWORD dwError = GetLastError();
		}
		iSelMark = ListView_GetSelectionMark(hListView);
		if(iSelMark == -1 || IsWindowEnabled(hListView) == 0)
		{
			EnableMenuItem(hJobMenu,IDM_RESERVE,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hJobMenu,IDM_AUTORESERVE,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hDisguiseMenu,0,MF_DISABLED|MF_GRAYED|MF_BYPOSITION);
			EnableMenuItem(hBKMenu,IDM_TOBK,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hBKMenu,IDM_BKWNDHOTKEY,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hBKMenu,IDM_UNREGISTERHK,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hDisguiseMenu,IDM_CHANGEICON,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hDisguiseMenu,IDM_CHANGETITLE,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hDisguiseMenu,IDM_RESTOREICONTITLE,MF_DISABLED|MF_GRAYED);
			EnableMenuItem(hJobMenu,IDM_PROPERTY,MF_DISABLED|MF_GRAYED);

			int iMenuCount = GetMenuItemCount(hJobMenu);
			if(0)//iMenuCount < 8)
			{
				AppendMenu(hJobMenu,MF_SEPARATOR,NULL,NULL);
				AppendMenu(hDisguiseMenu,MF_SEPARATOR,NULL,NULL);
				if(g_iUseLanguage == 0)
				{
					AppendMenuW(hJobMenu,MF_BYCOMMAND|MF_STRING,
						IDM_JOB_TIP3,L"Use Upper Functions Pleease Select Window List");
					AppendMenuW(hDisguiseMenu,MF_BYCOMMAND|MF_STRING,
						IDM_JOB_TIP3,L"Use Upper Functions Pleease Select Window List");

				}
				else
				{
					wchar_t * pText = new wchar_t[1024];
					AppendMenuW(hJobMenu,MF_BYCOMMAND|MF_STRING,
						IDM_JOB_TIP3,LoadItemStringW(L"IDM_JOB_TIP3",pText,NULL));
					AppendMenuW(hDisguiseMenu,MF_BYCOMMAND|MF_STRING,
						IDM_JOB_TIP3,LoadItemStringW(L"IDM_JOB_TIP3",pText,NULL));


					delete [] pText;
				}
			}
		}
		else
		{
			EnableMenuItem(hJobMenu,IDM_RESERVE,MF_ENABLED);
			EnableMenuItem(hJobMenu,IDM_AUTORESERVE,MF_ENABLED);			
			EnableMenuItem(hDisguiseMenu,0,MF_BYPOSITION|MF_ENABLED);
			EnableMenuItem(hBKMenu,IDM_TOBK,MF_ENABLED);
			EnableMenuItem(hBKMenu,IDM_BKWNDHOTKEY,MF_ENABLED);
			EnableMenuItem(hBKMenu,IDM_UNREGISTERHK,MF_ENABLED);
			EnableMenuItem(hDisguiseMenu,IDM_CHANGEICON,MF_ENABLED);			
			EnableMenuItem(hDisguiseMenu,IDM_CHANGETITLE,MF_ENABLED);			
			EnableMenuItem(hDisguiseMenu,IDM_RESTOREICONTITLE,MF_ENABLED);			
			EnableMenuItem(hJobMenu,IDM_PROPERTY,MF_ENABLED);
			ModifyAutoReserveMenu(hJobMenu,0);
			DeleteMenu(hJobMenu,7,MF_BYPOSITION);
			DeleteMenu(hJobMenu,7,MF_BYPOSITION);

			DeleteMenu(hDisguiseMenu,8,MF_BYPOSITION);
			DeleteMenu(hDisguiseMenu,8,MF_BYPOSITION);

		}
		if(1)//LOWORD(lParam) == 0)
		{
			if(TrayFlag == 1)
			{
				if(g_iUseLanguage == 0)
				{
					ModifyMenuW(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND,IDM_HIDEICON,L"Show &Icon");

				}
				else
				{
					wchar_t * pText = new wchar_t[1024];
					ModifyMenuW(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND,IDM_HIDEICON,
						LoadItemStringW(L"IDM_SHOWICON",pText,L"Show &Icon"));
					delete [] pText;
				}
			}
			else
			{
				if(g_iUseLanguage == 0)
				{
					ModifyMenuW(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND,IDM_HIDEICON,L"Hide &Icon");
				}
				else
				{
					wchar_t * pText = new wchar_t[1024];
					ModifyMenuW(hFileMenu,IDM_HIDEICON,MF_BYCOMMAND,IDM_HIDEICON,
						LoadItemStringW(L"IDM_HIDEICON",pText,L"Hide &Icon"));
					delete [] pText;
				}
			}

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_ONTOP) == BST_CHECKED)
			{
				CheckMenuItem(hFileMenu,IDM_ONTOP,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hFileMenu,IDM_ONTOP,MF_BYCOMMAND|MF_UNCHECKED);
			}

			if(IsWindowVisible(g_hInfoPanel) == TRUE)
			{
				CheckMenuItem(hFileMenu,IDM_EXPANDINFOPANEL,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hFileMenu,IDM_EXPANDINFOPANEL,MF_BYCOMMAND|MF_UNCHECKED);
			}
			
			{
				MENUITEMINFOW MII;
				MII.cbSize = sizeof(MII);
				MII.fMask = MIIM_FTYPE;
				MII.fType = MFT_RADIOCHECK;
				BOOL bRet = SetMenuItemInfoW(hJobMenu,IDM_HIDESEL,FALSE,&MII);
				bRet = SetMenuItemInfoW(hJobMenu,IDM_RESERVESEL,FALSE,&MII);
				DWORD dwError = GetLastError();
			}

			if(sOP.bReserveSel == 1)
			{
				CheckMenuItem(hJobMenu,IDM_HIDESEL,MF_BYCOMMAND|MF_UNCHECKED);
				CheckMenuItem(hJobMenu,IDM_RESERVESEL,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hJobMenu,IDM_RESERVESEL,MF_BYCOMMAND|MF_UNCHECKED);
				CheckMenuItem(hJobMenu,IDM_HIDESEL,MF_BYCOMMAND|MF_CHECKED);
			}
			if(IsWindowEnabled(hListView) == 0)
			{
				EnableMenuItem(hJobMenu,IDM_HIDESEL,MF_DISABLED|MF_GRAYED);
				EnableMenuItem(hJobMenu,IDM_RESERVESEL,MF_DISABLED|MF_GRAYED);
			}
			else
			{
				EnableMenuItem(hJobMenu,IDM_HIDESEL,MF_ENABLED);
				EnableMenuItem(hJobMenu,IDM_RESERVESEL,MF_ENABLED);
			}

			if (sOP.bRegistered == 1)
			{
				EnableMenuItem(hHelpMenu,IDM_REGISTER,MF_DISABLED|MF_GRAYED);
			}
			int iMenuCount = GetMenuItemCount(hHelpMenu);
			if(iMenuCount > 9)
			{
				//DeleteMenu(hHelpMenu,4,MF_BYPOSITION);
			}
			//DeleteMenu(hHelpMenu,IDM_BUYONLINE,MF_BYCOMMAND);
			//DeleteMenu(hHelpMenu,IDM_REGISTER,MF_BYCOMMAND);
			DeleteMenu(hHelpMenu,IDM_BLOG,MF_BYCOMMAND);
			if(sOP.bShowTip == 1)
			{
				CheckMenuItem(hFileMenu,IDM_HIDETIP,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hFileMenu,IDM_HIDETIP,MF_BYCOMMAND|MF_UNCHECKED);
			}
			if(sOP.bAutoChangeIconTitle == 1)
			{
				CheckMenuItem(hDisguiseMenu,IDM_REMEMBER,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hDisguiseMenu,IDM_REMEMBER,MF_BYCOMMAND|MF_UNCHECKED);
			}

			if(sOP.bBeginnerModel == 1)
			{
				CheckMenuItem(hFileMenu,IDM_NEWCOMMERMODEL,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hFileMenu,IDM_NEWCOMMERMODEL,MF_BYCOMMAND|MF_UNCHECKED);
			}

			if(g_HidedProcess == 1)
			{
				CheckMenuItem(hFileMenu,IDM_HIDEHDPROCESS,MF_BYCOMMAND|MF_CHECKED);
			}
			else
			{
				CheckMenuItem(hFileMenu,IDM_HIDEHDPROCESS,MF_BYCOMMAND|MF_UNCHECKED);
			}
#ifndef _HIDEPROCESS
			DeleteMenu(hFileMenu,IDM_HIDEHDPROCESS,MF_BYCOMMAND);
#endif

			DestroyMenu(hJobTipMenu_bk);
			hJobTipMenu = NULL;

			return 0;
		}
		
		return 0;
	case WM_SYSCOMMAND:
		if(wParam == SC_MAXIMIZE)
		{
			SendMessage(hwndDlg,WM_SIZE,0,0);
			ResizeInfoPanel();
			ResizeTabWnd();
		}
		return 0;

		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_HIDEHDPROCESS:
			{
				DWORD dwID = 0;
				GetWindowThreadProcessId(hwndDlg,&dwID);
				if(g_HidedProcess == 0)
				{
					HideProcess(dwID);
					g_HidedProcess = 1;
				}
				else
				{
					ShowProcess(dwID);
					g_HidedProcess = 0;
				}
			}
			break;
		case IDM_NEWCOMMERMODEL:
			sOP.bBeginnerModel = sOP.bBeginnerModel ? 0:1;
			WriteOptionFile(sOP);
			break;
		case IDC_STATIC_USEAGETIP:
			{
				HANDLE hControl = (HANDLE)lParam;
				if(hControl == NULL)
				{
					break;
				}
				UINT uCode = HIWORD(wParam);
			}
			
		case IDC_STATIC_EXPANDINFO:
			{
				ExpendInfoPanel();
				
				break;
			}
		case IDM_SHOWWINDOW:
			if((::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE) == 0)
			{
				if(ShowPasswordDlg() == 0)
				{
					return FALSE;
				}
				iFlag_Restore = 1;
				ShowWindow(hwndDlg,SW_MINIMIZE);
				//ShowWindow(hwndDlg,SW_RESTORE);
				SetForegroundWindow(hwndDlg);
				CloseBKTipDialog();
				SendMessage(hwndDlg,WM_SYSCOMMAND,SC_RESTORE,0);
				if(iBKItemCount > 0)
				{
					//ExtendWindow();
				}
				else
				{
					//ShowWindowNormal(hMainDialog);
				}
				{
					ResizeInfoPanel();
					ResizeTabWnd();
				}
				{
					if(sOP.iUsedTimes >= iEvaluateTime &&!(sOP.bRegistered = IsRegister()))
					{
						int iReturn = DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_REGISTER),
							hwndDlg,RegisterProc); 
					}
				}
			}
			else if (::GetWindowLong(hwndDlg,GWL_STYLE) & WS_VISIBLE)
			{
				iFlag_Restore = 0;

				if(lParam == 0)
				{
					iFlag_ThroughIcon = 1;
				}
				SendMessage(hwndDlg,WM_SYSCOMMAND,SC_MINIMIZE,0);
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
				if(IsWindowVisible(hwndDlg) == 0 && sOP.bShowBKTip == 1)
				{
					ShowBackTipDialog();
				}
				Shell_NotifyIcon(NIM_DELETE,&nID);
				if(g_iUseLanguage == 0)
				{
					::SetWindowTextW(::GetDlgItem(g_hInfoPanel,IDC_INFO_HIDEICON),L"Show &Icon");
				}
				else
				{
					SetDlgItemStringW(GetDlgItem(g_hInfoPanel,IDC_INFO_HIDEICON),
						IDD_MAINDIALOG,IDC_SHOWICON);
				}
				TrayFlag = 1;
			}
			else if (TrayFlag == 1)
			{
				TrayFlag = 0;
				nIDW.cbSize           = sizeof(NOTIFYICONDATAW);
				nIDW.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
				nIDW.uID              = IDI_ICON2;
				nIDW.hIcon            = hI1;
				nIDW.hWnd             = hMainWindow;
				nIDW.uTimeout         = 15000;
				if(g_iUseLanguage == 1)
				{
					wchar_t * pTrayIconText = new wchar_t[1024];
					wcscpy(nIDW.szTip,LoadItemStringW(L"IDS_TRAYICON_TOOLTIP",pTrayIconText));
					delete [] pTrayIconText;
				}
				else
				{
					wcscpy(nIDW.szTip,L"HideDragon");
				}

				Shell_NotifyIconW(NIM_ADD,&nIDW);
				if(g_iUseLanguage == 0)
				{
					::SetWindowTextW(::GetDlgItem(g_hInfoPanel,IDC_INFO_HIDEICON),L"Hide &Icon");
				}
				else
				{
					SetDlgItemStringW(GetDlgItem(g_hInfoPanel,IDC_INFO_HIDEICON),IDD_PANEL_ALLINFO,IDC_INFO_HIDEICON);
				}
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
			ShellExecute(NULL,L"open",L"HDUpdate.exe",NULL,NULL,SW_SHOWNORMAL);
			break;
		case IDM_EXIT:
//			if(TipIfExit() == 0)
//			{
//				return 0;
//			}
			g_iExit = 1;
			RestoreAllWindows(sOP.iKorS);
			

			RestoreAllBK(sOP.iKorS);
			RestoreAllIconTitle();
			Shell_NotifyIcon(NIM_DELETE,&nID);
			if(sOP.bUseMouse)
			{
				SetMouseHook(0);
				SetTrayHook(0);
			}
			SavePosSize();
			CloseHandle(hOptionFile);

			GarbageRecycle();
			 _CrtDumpMemoryLeaks();
			ReleaseResourceForTrayList();
			//RestoreIP();
			PostQuitMessage(1);
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
			//DeleteAll();

			FillList();

			break;
		case IDC_RESTORE:
			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			//EnterCriticalSection(&csFillList);
			for(i = 0;i<iBaSW;i++)
			{
				sW[i].bReserved = 0;
			}
			SendMessage(hwndDlg,WM_COMMAND,IDC_REFRESH,0);
			break;

		case IDCANCEL:
			//SendMessage(hwndDlg,WM_SYSCOMMAND,SC_MINIMIZE,0);
			SendMessage(hMainDialog,WM_COMMAND,IDM_SHOWWINDOW,1);
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
		case IDM_SPECBK:
			//MessageBox(hMainDialog,"You click the bkwizard button","hidedragon",MB_OK);
			OpenBKWizard();
			break;
		case IDM_ACQUIRELANGUAGE:
			UpdateLanguagePack();
			break;
		case IDC_SHOWBK:
			if (ListView_GetSelectedCount(hList_BK) == 0)
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_PLEASESELECTAITEM",L"Please select a item.");
				break;
			}
			else
			{
				iSelIndex = ListView_GetSelectionMark(hList_BK);
				bkItem.iItem = iSelIndex;
				bkItem.mask = LVIF_PARAM;
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
		case IDM_XPBK:
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_XPBK",cUrl,L"http://www.hidedragon.com/cn/sub/eXPerience%20Running.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			break;
		case IDM_XPQH:
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_XPQUICKHIDE",cUrl,L"http://www.hidedragon.com/cn/sub/eXPerience%20quick%20hide.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			
			break;
		case IDM_HELP_CLEAN:
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_XPCLEAN",cUrl,L"http://www.hidedragon.com/cn/sub/CleanHistory.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}

			
			break;
		case IDM_HELP_HIDEIP:

			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_XPHIDE",cUrl,L"http://www.hidedragon.com/cn/sub/HideIP.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}

			
			break;
		case IDM_XPDISGUISE:
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
			
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_XPDISGUISE",cUrl,L"http://www.hidedragon.com/cn/sub/eXPerience%20disguise.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}


			
			break;

		case IDM_BLOG:
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_BLOG",cUrl,L"http://hi.baidu.com/hidedragon"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}

			
			break;
		case IDM_SUGGEST:
			{
// 				wchar_t * cUrl = new wchar_t[1024];
// 				memset(cUrl,0,1024 * sizeof(wchar_t));
// 				
// 				ShellExecute(NULL,L"open",L"IEXPLORE",
// 					LoadItemStringW(L"IDS_HELPLINK_SUGGEST",cUrl,L"http://www.hidedragon.com/cnbbs/index.php"),NULL,SW_SHOW);
// 				delete [] cUrl;
// 				cUrl = NULL;

				ShellExecute(hwndDlg,NULL,L"mailto:support@hidedragon.com",NULL,NULL,SW_SHOWMAXIMIZED);
			}

			
			break;
		case IDM_USERMANUAL:
			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_MANUAL",cUrl,L"http://www.hidedragon.com/cn/Manual_set.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			
			break;
		case IDM_BUYONLINE:
			{
// 				wchar_t * cUrl = new wchar_t[1024];
// 				memset(cUrl,0,1024 * sizeof(wchar_t));
// 				
// 				ShellExecute(NULL,L"open",L"IEXPLORE",
// 					LoadItemStringW(L"IDS_HELPLINK_PURCHASE",cUrl,L"http://www.hidedragon.com/cn/purchase.html"),NULL,SW_SHOW);
// 				delete [] cUrl;
// 				cUrl = NULL;

				BuyHideDragon();
			}
			
			break;
		case IDM_REGISTER:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_REGISTER),hMainDialog,RegisterProc);
			break;
		case IDM_RESERVESEL:
			sOP.bReserveSel = 1;
			WriteOptionFile(sOP);

			if(g_iTipCount >= 1)
			{
				wcscpy(g_pTipText[1][0],L"IDS_USAGETIP_SELWNDLISTCANRESERVEWND");
				wcscpy(g_pTipText[1][1],L"Check below window list item can to reserve or hide:");
				if(IsWindowEnabled(hListView) == TRUE)
				{
					SetTipText(L"IDS_USAGETIP_SELWNDLISTCANRESERVEWND",
						L"Check below window list item can to reserve or hide:");
				}
			}
			
			break;
		case IDM_HIDESEL:
			sOP.bReserveSel = 0;
			WriteOptionFile(sOP);

			if(g_iTipCount >= 1)
			{
				wcscpy(g_pTipText[1][0],L"IDS_USAGETIP_HIDESELITEM");
				wcscpy(g_pTipText[1][1],L"In Window List selected item will hide.");
				if(IsWindowEnabled(hListView) == TRUE)
				{
					SetTipText(L"IDS_USAGETIP_HIDESELITEM",
						L"In Window List selected item will hide.");
				}
			}
			break;
		case IDM_RESERVE:
			iSelMark = ListView_GetSelectionMark(hListView);
			if(ListView_GetCheckState(hListView,iSelMark) == FALSE)
			{
				ListView_SetCheckState(hListView,iSelMark,TRUE);
			}
			else
			{
				ListView_SetCheckState(hListView,iSelMark,FALSE);
			}
			break;
		case IDM_AUTORESERVE:
			iSelMark = ListView_GetSelectionMark(hListView);
			iSelectedItem = iSelMark;
			ListView_GetItemText(hListView,iSelectedItem,1,cSelPID,10);
			for(iAR = 0;iAR < iBaSW;iAR++)
			{
				swprintf(cCurPID,L"%X",sW[iAR].lPID);
				if(wcscmp(cCurPID,cSelPID) == 0)
				{
					break;
				}
			}
			pItem = new sFileItem;
			pItem->lSize = wcslen(sW[iAR].cProcessName) * 2;
			pItem->cFileName = sW[iAR].cProcessName;

			pHMItem = new sHMF_Item;
			pHMItem->cName = sW[iAR].cProcessName;
			pHMItem->lSize = wcslen(sW[iAR].cProcessName) * 2;
			pHMItem->iTimes = 1;

			{
				DBClass arDB;

				if(!arDB.IsInARFL(pItem))
				{
					arDB.InsertARItem(pItem);
					
					ListView_SetCheckState(hListView,iSelectedItem,TRUE);
				}
				else
				{
					arDB.DelARItem(pItem);
					
					arDB.DelHMItem(pHMItem);//HMF_Modify(pHMItem,HMF_DELITEM);
					ListView_SetCheckState(hListView,iSelectedItem,FALSE);
				}
			}
			pItem->cFileName = NULL;
			delete pItem;
			delete pHMItem;

			break;
		case IDM_EXPANDINFOPANEL:
			SendMessage(hwndDlg,WM_COMMAND,IDC_STATIC_EXPANDINFO,NULL);
			break;
		case IDM_HIDETIP:
			sOP.bShowTip = (sOP.bShowTip == 0) ? 1:0;
			WriteOptionFile(sOP);
			break;

SetAlpha:	
			iSelectedItem = iSelMark;
			ListView_GetItemText(hListView,iSelectedItem,1,cSelPID,10);
			for(iAR = 0;iAR < iBaSW;iAR++)
			{
				swprintf(cCurPID,L"%X",sW[iAR].lPID);
				if(wcscmp(cCurPID,cSelPID) == 0)
				{
					SetWindowTransparent(sW[iAR].hWnd,iAlpha);
				}
			}
			break;
		case IDM_TRANSPARENT_MIDDLE:
			iSelMark = ListView_GetSelectionMark(hListView);
			iAlpha = ID_WNDALPHA_MIDDLE;
			goto SetAlpha;
		case IDM_TRANSPARENT_RESTORE:
			iSelMark = ListView_GetSelectionMark(hListView);
			iAlpha = ID_WNDALPHA_RESTORE;
			goto SetAlpha;
		case IDM_TRANSPARENT_LOW:
			iSelMark = ListView_GetSelectionMark(hListView);
			iAlpha = ID_WNDALPHA_LOW;
			goto SetAlpha;
		case IDM_TRANSPARENT_MOSTHIGH:
			iSelMark = ListView_GetSelectionMark(hListView);
			iAlpha = ID_WNDALPHA_MOSTHIGH;
			goto SetAlpha;
		case IDM_TRANSPARENT_MIDDLEHIGH:
			iSelMark = ListView_GetSelectionMark(hListView);
			iAlpha = ID_WNDALPHA_MIDDLEHIGH;
			goto SetAlpha;
		case IDM_TRANSPARENT_MIDDLELOW:
			iSelMark = ListView_GetSelectionMark(hListView);
			iAlpha = ID_WNDALPHA_MIDDLELOW;
			goto SetAlpha;
		case IDM_TOBK:
			iSelMark = ListView_GetSelectionMark(hListView);
			BKCurrentWindow(iSelMark);
			break;
		case IDM_UNREGISTERHK:
			iSelMark = ListView_GetSelectionMark(hListView);
			UnRegisterHK(iSelMark);
			break;

		case IDM_BKWNDHOTKEY:
			iSelMark = ListView_GetSelectionMark(hListView);
			SetBkWndHotkey(iSelMark);
			break;
		case IDM_CHANGEICON:
			iSelMark = ListView_GetSelectionMark(hListView);
			ChangeWindowIcon(iSelMark);
			FillList();
			break;
		case IDM_CHANGETITLE:
			iSelMark = ListView_GetSelectionMark(hListView);
			ChangeWndTitle(iSelMark);
			FillList();
			break;
		case IDM_RESTOREICONTITLE:
			iSelMark = ListView_GetSelectionMark(hListView);
			RestoreTitle(iSelMark);
			RestoreIcon(iSelMark);
			FillList();
			break;
		case IDM_REMEMBER:
			SaveAutoChangeIconTitle();
			break;
		case IDM_VIDEOTURORIAL:

			{
				wchar_t * cUrl = new wchar_t[1024];
				memset(cUrl,0,1024 * sizeof(wchar_t));
				
				ShellExecute(NULL,L"open",L"IEXPLORE",
					LoadItemStringW(L"IDS_HELPLINK_VIDEOHELP",cUrl,L"http://www.hidedragon.com/cn/videotutorial.html"),NULL,SW_SHOW);
				delete [] cUrl;
				cUrl = NULL;
			}
			
// 			ShellExecute(NULL,L"open",L"IEXPLORE",
// 				L"http://www.hidedragon.com/cn/videotutorial.html",NULL,SW_SHOW);

			
			break;
		case IDM_PROPERTY:
			iSelMark = ListView_GetSelectionMark(hListView);
			ShowPropertyDialog(iSelMark);
			break;

		case IDM_ADDAUTOHIDE:
			iSelMark = ListView_GetSelectionMark(hListView);
			Save2AutoHideList(iSelMark);
			break;

		case IDM_SETTOTOP:
			iSelMark = ListView_GetSelectionMark(hListView);
			SetWindowToTop(iSelMark);

			break;


		default:
			if(LOWORD(wParam) >= 40300 && LOWORD(wParam) <= 40300 + g_iLangCount)
			{
				wchar_t * lpMenuString = new wchar_t[1024];
				memset(lpMenuString,0,1024 * sizeof(wchar_t));
				GetMenuStringW(g_hLangMenu,LOWORD(wParam),lpMenuString,1024,MF_BYCOMMAND);
	 			int iRet = LoadLanguage(lpMenuString,NULL);
				if(iRet == 1)
				{
					ChangeLanguage(LOWORD(wParam),lpMenuString);
					break;

#ifdef __use_change
					g_iUseLanguage = 1;
					SetDialogTextW(&hwndDlg,IDD_MAINDIALOG);
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
					SetMenuString();
					ResetPriTree();


					ModifyMenuW(g_hLangMenu,LOWORD(wParam),MF_CHECKED|MF_STRING,
						LOWORD(wParam),lpMenuString);

					g_CurrLangID = LOWORD(wParam);
					
					//ModifyMenu(g_hLangMenu,g_OldLangID,MF_UNCHECKED|MF_,g_OldLangID,g_OldString);
					CheckMenuItem(g_hLangMenu,g_OldLangID,MF_UNCHECKED);
					g_OldLangID = g_CurrLangID;
					FillList();
#endif //__use_change
					/*MENUITEMINFO MII;
					MII.cbSize = sizeof(MII);
					MII.fMask = MIIM_FTYPE;
					MII.fType = MFT_RADIOCHECK;
					BOOL bRet = SetMenuItemInfo(g_hLangMenu,LOWORD(wParam),FALSE,&MII);
					DWORD dwError = GetLastError();*/
				}
				else
				{
					HDMessageBoxW(L"IDS_MESSAGEBOX_NOTFOUNDLANGPACK",
						L"Not found language pack,or load language failure.");
					g_iUseLanguage = 0;
				}

				delete [] lpMenuString;

			}
			return FALSE;
		}

	default:
		return FALSE;
	}
	return FALSE;
}

HBRUSH g_bmenu = NULL,g_wbr = NULL;


HWND hToolTip_about;

INT_PTR CALLBACK AboutProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	wchar_t sContentCredit [] = L"Please send us emails for any advice or comment,and we will improve our software in next version.\r\n\r\n"
							
							L"Welcome to register, In this way you will obtain a registration code which allows you to continue using this software and receive technical support.";
//	static HBRUSH bmenu = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	HBRUSH bmenu = NULL,wbr = NULL;
	static int iForLink = 1,iForIn = 0,iForOut = 0;
	TOOLINFO ti;
	LPTOOLTIPTEXT lpttt;
	int idCtrl;
	HWND hEditQQ = NULL;
	HDC hScreenDC;
	int iX = 0,iY = 0;
	wchar_t * pTipText = NULL, * pTipText2 = NULL;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hAboutDialog = ::GetParent(hwndDlg);
//		ChangeWindowStyle(hAboutDialog,0);
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

		hToolTip_about = CreateWindowEx(NULL,TOOLTIPS_CLASS,NULL,WS_POPUP|TTS_ALWAYSTIP,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwndDlg,NULL,hInst,NULL);
		ti.cbSize     = sizeof (TOOLINFO);
		ti.hinst      = hInst;
		ti.uFlags     = TTF_IDISHWND;
		ti.hwnd       = hwndDlg;
		ti.uId        = (UINT_PTR)hHttp;
		ti.lpszText   = /*"Welcome !";*/LPSTR_TEXTCALLBACK;
		SendMessage(hToolTip_about,TTM_ADDTOOL,0,(LPARAM)&ti);
		ti.uId        = (UINT_PTR)hEmail;
		SendMessage(hToolTip_about,TTM_ADDTOOL,0,(LPARAM)&ti);
		SetWindowText(hEditQQ,L"hdsupport@gmail.com");
//		hIconAbout = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON3));
//		SendMessage(hAboutDialog,WM_SETICON,ICON_SMALL,(LPARAM)hIconAbout);
//		CloseHandle(hIconAbout);
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOGABOUT);
			wchar_t * pText = new wchar_t[1024],* cRights = new wchar_t[1024];
			memset(cRights,0,1024 * sizeof(wchar_t));
			wcscat(cRights,LoadItemStringW(L"IDD106_1028",pText,NULL));
			wcscat(cRights,L"\r\n\r\n");
			wcscat(cRights,LoadItemStringW(L"IDS_PARA2",pText,NULL));
			SendMessageW(hEditCre,WM_SETTEXT,0,(LPARAM)cRights);
			delete [] pText;
			delete [] cRights;
		}

		g_bmenu = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		g_wbr = CreateSolidBrush(RGB(255,255,255));


		{
			wchar_t * cUserName = new wchar_t[1024];
			memset(cUserName,0,1024*sizeof(wchar_t));
			DBClass db;
			db.ReadUserName(cUserName);
			if(wcscmp(cUserName,L"") == 0)
			{
				LoadItemStringW(L"IDS_UNREGISTER",cUserName,NULL);
			}
			SetDlgItemText(hwndDlg,IDC_ABOUT_STATIC_LICENSENAME,cUserName);
			
			delete [] cUserName;
		}


		
		return FALSE;
	case WM_CTLCOLORSTATIC:
//		g_bmenu = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
//		g_wbr = CreateSolidBrush(RGB(255,255,255));

		if ((HWND)lParam == hEditCre)
		{
			return (LRESULT)g_wbr;
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
			
			return (LRESULT)g_bmenu;
		}

		SetBkColor((HDC)(wParam),GetSysColor(COLOR_3DFACE));
		return (LRESULT)g_bmenu;
	case WM_DESTROY:
		DeleteObject((HGDIOBJ)g_bmenu);
		DeleteObject((HGDIOBJ)g_wbr);
		ReleaseDC(hHttp,hHttpDC);
		ReleaseDC(hEmail,hEmailDC);
		
		return FALSE;
	case WM_CTLCOLORDLG:
//		g_bmenu = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		return (LRESULT)g_bmenu;
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
			lpttt  = (LPTOOLTIPTEXTW)lParam;
			switch(idCtrl)
			{
			case IDC_STATICHTTP:
				if(g_iUseLanguage == 1)
				{
					pTipText2 = new wchar_t[1024];
					wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_HTTP",pTipText2));
					delete [] pTipText2;
				}
				else
				{
					lpttt ->lpszText = L"Get the newest information about HideDragon.";
				}
				
				break;
			case IDC_STATICEMAIL:
				if(g_iUseLanguage == 1)
				{
					pTipText = new wchar_t[1024];
					wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_MAIL",pTipText));
					delete [] pTipText;
				}
				else
				{
					lpttt ->lpszText = L"Feedback and Suggestion.";
				}
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
					ShellExecute(hwndDlg,NULL,L"mailto:support@hidedragon.com",NULL,NULL,SW_SHOWMAXIMIZED);
				}
				break;
			case IDC_STATICHTTP:
				if(HIWORD(wParam) == STN_CLICKED)
				{
					//MessageBox(hwndDlg,"ok","ok",MB_OK);
					ShellExecute(hwndDlg,L"Open",L"IEXPLORE",L"http://www.hidedragon.com",
						NULL,SW_SHOWMAXIMIZED);
				}
				break;
			case IDCANCEL:
				DeleteObject((HGDIOBJ)g_bmenu);
				DeleteObject((HGDIOBJ)g_wbr);
				ReleaseDC(hHttp,hHttpDC);
				ReleaseDC(hEmail,hEmailDC);
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
	int iSel = ListView_GetSelectionMark(hListView);
	

	if(g_iExit == 1)
	{
		return 0;
	}
	//EnterCriticalSection(&csFillList);
	ListView_DeleteAllItems(hListView);
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

	FillNotExistWnd();

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
				wcscpy(sW[j].cWndTitle,sW[j+1].cWndTitle);
				sW[j].bHide = sW[j+1].bHide;
				wcscpy(sW[j].cProcessName,sW[j+1].cProcessName);
				sW[j+1].bExist = 0;
				sW[j].iHasTrayIcon = sW[j+1].iHasTrayIcon;
				sW[j].iIconIndex = sW[j+1].iIconIndex;
				sW[j].trayicon = sW[j+1].trayicon;
				sW[j].bToBk = sW[j+1].bToBk;
				sW[j].iCommandId = sW[j+1].iCommandId;
				memset(sW[j].cIconTip,0,1024 * sizeof(wchar_t));
				wcscpy(sW[j].cIconTip,sW[j+1].cIconTip);
			}
		}
	}


	int i2 = 0;
	for(i2 = 0;sW[i2].bExist != 0||sW[i2].bHide == 1;i2++)
	{
		
	}
	iBaSW = iForSW= i2;

	DelTitleNode();
 	DelIconNode();
	DelBKHotKeyNode();



	ExeData * pED = new ExeData;
	pED->sWP = sW;
	pED->iCount = iBaSW;
	pED->pArr_QH = Array_QH;
	pED->iUseLanguage = g_iUseLanguage;
	pED->pTipCount = &g_iTipCount;
	pED->pTipText = (wchar_t ***) g_pTipText;
	ReserveWnds(pED);
	delete pED;

	//SaveWindowInfo(sW,iBaSW);

	//LeaveCriticalSection(&csFillList);
	if(iSel <= -1)
	{
		iSel = 0;
	}

	int iTabSel = TabCtrl_GetCurSel(g_Tab);
	if(iTabSel == 0)
	{
		SetFocus(hListView);
		ListView_SetSelectionMark(hListView,iSel);
		ListView_SetItemState(hListView,iSel,LVIS_FOCUSED,0);
		ListView_SetItemState(hListView,iSel,LVIS_SELECTED,LVIS_SELECTED);
	}

	return 1;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
	wchar_t * cWndText,cPID[10] = L"", * pszText = NULL;
	HWND hOnw;
	DWORD lPID = 0;
	int i1 = 0,iFlag = 0,iconIndex = 0,imageindex = 0,iItemCount = 0,iRec = 0,i3 = 0;
	TRAYDATA * pTrayinfo = NULL;
	WndAndHandle  temp_SW,*popWnd = NULL,*pTemp = NULL,*pNext = NULL;
	HICON hMainIcon;
	LVITEMW bkItem;
	sFileItem * pItem = NULL;
	if (::GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{	
		cWndText = (wchar_t *)VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		pszText = (wchar_t *)VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		if(cWndText == NULL || pszText == NULL)
		{
			//MessageBox(hMainDialog,"Memory allocate failure in EnumWindowProc","HideDragon",MB_OK);
			return 1;
		}
		memset(cWndText,0,2048 * sizeof(wchar_t));
		memset(pszText,0,2048 * sizeof(wchar_t));
		GetWindowTextW(hwnd,cWndText,1024);
		GetWindowThreadProcessId(hwnd,&lPID);
		if(lPID == 2336)
		{
			lPID = lPID;
		}
		if(lPID == GetCurrentProcessId())
		{
			VirtualFree(pszText,0,MEM_RELEASE);
			VirtualFree(cWndText,0,MEM_RELEASE);
			return 1;
		}
		if (/*!wcscmp(cWndText,"")||*/!wcscmp(cWndText,L"Program Manager"))
		{
			VirtualFree(pszText,0,MEM_RELEASE);
			VirtualFree(cWndText,0,MEM_RELEASE);
			return 1;
		}
		if (hwnd == hMainDialog||::GetParent(hwnd) == hMainDialog)
		{
			VirtualFree(pszText,0,MEM_RELEASE);
			VirtualFree(cWndText,0,MEM_RELEASE);
			return 1;
		}
		
		
		//Auto hide background window's popup window
		if (sOP.bAutoHide == 1)
		{
			iItemCount = ListView_GetItemCount(hList_BK);
			for (i = 0;i<iItemCount;i++)
			{
				bkItem.iItem = i;
				bkItem.mask = LVIF_PARAM;
				bkItem.iSubItem = 0;
				ListView_GetItem(hList_BK,&bkItem);
				pTemp = (WndAndHandle*)bkItem.lParam;
				if(pTemp->lPID == lPID && wcscmp(wcslwr(pTemp->cProcessName),L"explorer.exe") != 0)
				{
					popWnd = new WndAndHandle;
					memset(popWnd,0,sizeof(WndAndHandle));
					popWnd->cWndTitle  = (wchar_t*) VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
					memset(popWnd->cWndTitle,0,2048 * sizeof(wchar_t));
					popWnd->cIconTip = new wchar_t[1024];
					memset(popWnd->cIconTip,0,1024 * sizeof(wchar_t));
					ShowWindow(hwnd,SW_HIDE);
					bkItem.mask = LVIF_PARAM;
					bkItem.iSubItem = 0;
					popWnd->hWnd = hwnd;
					popWnd->lPID = lPID;
					popWnd->Next = NULL;
					pTemp = (WndAndHandle*)bkItem.lParam;
					while(pTemp->Next != NULL)
					{
						pNext = pTemp->Next;
						pTemp = pNext;
					}
					pTemp->Next = popWnd;
					ListView_SetItem(hList_BK,&bkItem);
				}
			}
		}

		if(sOP.bAutoHide_QH == 1)
		{
			for(i = 0;i < iBaSW;i++)
			{
				if(sW[i].bHide == 1 && sW[i].lPID == lPID && wcscmp(sW[i].cProcessName,L"explorer.exe"))
				{
					LRESULT lRet_QH = SendMessageTimeout(hwnd,WM_GETICON,
						ICON_SMALL,NULL,SMTO_NORMAL,100,(PDWORD_PTR)&hMainIcon);



					if(lRet_QH == 0)
					{
						dbview(L"EnumWindowsProc enter return 1;........................................");
						return 1;
// 						DWORD dwRet_QH = GetLastError();
// 						if(dwRet_QH == 0)
// 						{
// 							dbview(L"EnumWindowsProc enter return 1;........................................");
// 							
// 						}
					}

					ShowWindow(hwnd,SW_HIDE);
					if(iBaSW_QH > 19)
						return 1;
					Array_QH[iBaSW_QH] = hwnd;
					iBaSW_QH ++;
					VirtualFree(pszText,0,MEM_RELEASE);
					VirtualFree(cWndText,0,MEM_RELEASE);
					return 1;
				}
			}
		}
		




		SendMessageTimeout(hwnd,WM_GETICON,ICON_SMALL,NULL,SMTO_NORMAL,100,(PDWORD_PTR)&hMainIcon);
		if(hMainIcon == NULL)
		{
			hMainIcon = (HICON)GetClassLongPtr(hwnd,GCLP_HICONSM);
		}
		if(hMainIcon == NULL)
		{
			hMainIcon = hIcon_Default;
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		
		lvItem.mask = LVIF_TEXT|LVIF_IMAGE/*|LVIF_PARAM*/;
		lvItem.pszText = pszText;
		
		wcscpy(lvItem.pszText,cWndText);
		lvItem.iItem = SendMessage(hListView,LVM_GETITEMCOUNT,0,0);
		lvItem.iSubItem = 0;

		//2.2 method////////
		for(i1 = 0;i1<iBaSW;i1++)
		{
			if( lPID == sW[i1].lPID &&sW[i1].bReserved == 1&&
				(wcscmp(wcslwr(sW[i1].cProcessName),L"iexplore.exe")==0 
				|| wcscmp(wcslwr(sW[i1].cProcessName),L"explorer.exe") == 0)
				&& hwnd != sW[i1].hWnd/*wcscmp(sW[i1].cWndTitle,cWndText)*/
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
				wcscpy(sW[iForSW].cWndTitle,cWndText);
				sW[iForSW].lPID = lPID;
				sW[iForSW].hWnd = hwnd;
				sW[iForSW].bExist = 1;
				sW[iForSW].iWindowType = 0;
				sW[iForSW].bReserved = 1;
				sW[iForSW].bHide = 0;
				wcscpy(sW[iForSW].cProcessName,sW[i1].cProcessName);
				

				if (::GetParent(hwnd) != NULL )
				{
					sW[iForSW].iWindowType = 1;
					iForSW ++;
					iBaSW = iForSW;
					VirtualFree(pszText,0,MEM_RELEASE);					
					VirtualFree(cWndText,0,MEM_RELEASE);
					return 1;
				}
				/*if(wcscmp(cWndText,"")==0)
				{
					wcscpy(lvItem.pszText,sW[i1].cProcessName);
				}*/

				imageindex = ImageList_AddIcon(hIconList,hMainIcon);
				lvItem.iImage = imageindex;
				//lvItem.pszText = pszText; 又赋了一次，上面已经赋过一次了
//				lvItem.lParam = (LPARAM)(sW + iForSW);
				wcscpy(lvItem.pszText,sW[iForSW].cWndTitle);
				if(IsInListView(sW[iForSW].lPID,sW[iForSW].cWndTitle) == 0)
				{
					ListView_InsertItem(hListView,&lvItem);
					swprintf(cPID,L"%X",sW[iForSW].lPID);

					wcscpy(lvItem.pszText ,cPID);
					lvItem.iSubItem = 1;
					ListView_SetItem(hListView,&lvItem);
					memset(cPID,0,10*sizeof(wchar_t));

					pTrayinfo = NULL;
					if(wcscmp(wcslwr(sW[iForSW].cProcessName),L"explorer.exe")!=0)
					{
						iconIndex = IsHaveTrayIcon(&sW[iForSW],pTrayinfo);
						if (iconIndex > 0 || pTrayinfo != NULL)
						{
							sW[iForSW].iIconIndex =  iconIndex;
							sW[iForSW].trayicon =  arrTray[iconIndex];
							sW[iForSW].iHasTrayIcon = 1;

							lvItem.iSubItem = 2;
							if(g_iUseLanguage == 0)
							{
									wcscpy(lvItem.pszText,L"Have");
							}
							else
							{
								wchar_t * pText = new wchar_t[1024];
								wcscpy(lvItem.pszText,LoadItemStringW(L"IDS_HAVETRAYICON",pText,L"Have"));
								delete [] pText;
							}
							//wcscpy(lvItem.pszText,L"有");///////////////////////////////////////////Need International////////////////////////////////////////////////////
							ListView_SetItem(hListView,&lvItem);
						}
					}

					iForState = 1;
					ListView_SetCheckState(hListView,lvItem.iItem,TRUE);
					iForState = 0;

					iForSW ++;
					iBaSW = iForSW;
				}
				VirtualFree(pszText,0,MEM_RELEASE);
				VirtualFree(cWndText,0,MEM_RELEASE);
				return 1;
			}


			if (hwnd == sW[i1].hWnd || sW[i1].bHide == 1)
			{
				sW[i1].bExist = 1;

				if(hwnd == sW[i1].hWnd && sW[i1].bReserved == 0)// && ::GetParent(hwnd)==NULL)
				{
					wcscpy(sW[i1].cWndTitle,cWndText);//For window titile change,ues the new title///////////////////////
					break;

				}
				else if (hwnd == sW[i1].hWnd && sW[i1].bReserved == 1)
				{
					wcscpy(sW[i1].cWndTitle,cWndText);
					HWND hOnw = ::GetParent(sW[i1].hWnd);
					if (sW[i1].iWindowType == 1 && IsWindowVisible(hOnw) == TRUE )//&& IsEuqalExe(hOnw,L"qq.exe",iBaSW) == 0)//For QQ
					{
						VirtualFree(pszText,0,MEM_RELEASE);
						VirtualFree(cWndText,0,MEM_RELEASE);
						return 1;
					}
					/*if(lPID == sW[i1].lPID && wcscmp(cWndText,"")==0)
					{
						return 1;
					}*/
					for(i3 = 0;i3<iBaSW;i3++)
					{
						if(lPID == sW[i3].lPID && wcscmp(cWndText,L"")==0 && hwnd != sW[i3].hWnd
							&& hwnd != GetParent(sW[i3].hWnd))
						{
							VirtualFree(pszText,0,MEM_RELEASE);
							VirtualFree(cWndText,0,MEM_RELEASE);
							return 1;
						}
					}
					if(i3>=iBaSW)
					{
						if(wcscmp(cWndText,L"") == 0)
						{
							wcscpy(sW[i1].cWndTitle,sW[i1].cProcessName);
						}
					}

					imageindex = ImageList_AddIcon(hIconList,hMainIcon);
					lvItem.iImage = imageindex;
					//lvItem.pszText = pszText; 又赋了一次
//					lvItem.lParam = (LPARAM)(sW + i1);
					wcscpy(lvItem.pszText,sW[i1].cWndTitle);
					if(IsInListView(sW[i1].lPID,sW[i1].cWndTitle) == 0)
					{
						ListView_InsertItem(hListView,&lvItem);
						swprintf(cPID,L"%X",sW[i1].lPID);
						memset(lvItem.pszText,0,2048 * sizeof(wchar_t));
						wcscpy(lvItem.pszText,cPID);
						lvItem.iSubItem = 1;
						ListView_SetItem(hListView,&lvItem);
						memset(cPID,0,10*sizeof(wchar_t));

						
						pTrayinfo = NULL;
						if(wcscmp(wcslwr(sW[i1].cProcessName),L"explorer.exe")!=0)
						{
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
								if(g_iUseLanguage == 0)
								{
										wcscpy(lvItem.pszText,L"Have");
								}
								else
								{
									wchar_t * pText = new wchar_t[1024];
									wcscpy(lvItem.pszText,LoadItemStringW(L"IDS_HAVETRAYICON",pText,L"Have"));								
									delete [] pText;
								}
								//wcscpy(lvItem.pszText,L"有");/////////////////////////////////////////////////////////////Need International///////////////////////////////
								ListView_SetItem(hListView,&lvItem);
							}
						}
						

						iForState = 1;
						ListView_SetCheckState(hListView,lvItem.iItem,TRUE);
						iForState = 0;
					}
					VirtualFree(pszText,0,MEM_RELEASE);
					VirtualFree(cWndText,0,MEM_RELEASE);
					
					return 1;
				}
			}
		}//end for

		if(i1>=iBaSW)
		{
			wcscpy(sW[iForSW].cWndTitle,cWndText);
			sW[iForSW].lPID = lPID;


			hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
			pe32.dwSize = sizeof(PROCESSENTRY32);
			Process32First((void*)hProcessSnap,&pe32);
			int iFoundFlag = 0;
			while(Process32Next((void*)hProcessSnap,&pe32))
			{
				if(pe32.th32ProcessID == lPID)
				{
					iFoundFlag = 1;
					if(wcscmp(wcslwr(pe32.szExeFile),L"iexplore.exe")==0)
					{
						wcscpy(sW[iForSW].cProcessName,pe32.szExeFile);
						//MessageBox(NULL,"IE","HD",MB_OK);
					}
					else if(wcscmp(wcslwr(pe32.szExeFile),L"explorer.exe") == 0)
					{
						dwExPID = lPID;
						wcscpy(sW[iForSW].cProcessName,pe32.szExeFile);
					}
					else
					{
						wcscpy(sW[iForSW].cProcessName,pe32.szExeFile);
					}
				}
			}
			CloseHandle((HANDLE)hProcessSnap);

			if(iFoundFlag == 0)
			{
				//ShowProcess(sW[iForSW].lPID);
				GetHidedProcessName(lPID,sW[iForSW].cProcessName);
			}
			
			//2.5/////////////////////////////////////////////////////////////
			if (wcscmp(sW[iForSW].cProcessName,L"explorer.exe") == 0 && wcscmp(sW[iForSW].cWndTitle,L"")==0)
			{
				VirtualFree(pszText,0,MEM_RELEASE);
				VirtualFree(cWndText,0,MEM_RELEASE);
				return 1;
			}
			if(wcscmp(sW[iForSW].cWndTitle,L"")==0)
			{
				wcscpy(sW[iForSW].cWndTitle,sW[iForSW].cProcessName);
				wcscpy(lvItem.pszText,sW[iForSW].cWndTitle);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////

			sW[iForSW].hWnd = hwnd;
			sW[iForSW].bExist = 1;
			sW[iForSW].iWindowType = 0;
			sW[iForSW].bReserved = 0;
			sW[iForSW].bHide = 0;

			//For AutoCheck//////////////////////////////////////////////////////////////////
			pItem = new sFileItem;
			pItem->lSize = wcslen(sW[iForSW].cProcessName) * 2;
			pItem->cFileName = sW[iForSW].cProcessName;
			{
				DBClass arDB;

				if(arDB.IsInARFL(pItem))
				{
					sW[iForSW].bReserved = 1;
				}
			}
			pItem->cFileName = NULL;
			delete pItem;

			//For BK Hotkey///////////////////////////////////////////////////////////////
			{
				sBKWHotkey * pItem = new sBKWHotkey;
				memset(pItem,0,sizeof(sBKWHotkey));
				wcscpy(pItem->cProcessName,sW[iForSW].cProcessName);
				DBClass db_HK;
				int iHK = db_HK.IsInBKHK(pItem);
				delete pItem;
				wchar_t cHKPid[10];
				memset(cHKPid,0,10);
				
				swprintf(cHKPid,L"%X",sW[iForSW].lPID);
				
				int iInHKList = IsInRegBKHK(sW + iForSW);

				if(iHK && iInHKList == 0)
				{
					RegisterBKHotkey(sW + iForSW);
				}
			}


			pTrayinfo = NULL;
			if(wcscmp(wcslwr(sW[iForSW].cProcessName),L"explorer.exe")!=0)
			{
				iconIndex = IsHaveTrayIcon(&sW[iForSW],pTrayinfo);
				if (iconIndex >= 0 )
				{
					sW[iForSW].iIconIndex =  iconIndex;
					sW[iForSW].trayicon = arrTray[iconIndex];
					sW[iForSW].iHasTrayIcon = 1;
				}
				else
				{
					sW[iForSW].iHasTrayIcon = 0;
				}
			}
			iForSW ++;
			iBaSW = iForSW;
			hOnw = ::GetParent(hwnd);
			{
				BOOL bVisile = IsWindowVisible(hOnw);
			}
			if(hOnw !=NULL  && IsWindowVisible(hOnw) == TRUE )// && IsEuqalExe(hwnd,L"qq.exe",iForSW) == 0)
			{
				//2.2 method//////
				sW[iForSW-1].iWindowType = 1;//Temp comment
				VirtualFree(pszText,0,MEM_RELEASE);
				VirtualFree(cWndText,0,MEM_RELEASE);
				return 1;
			}
		}
		else
		{
			if(wcscmp(wcslwr(sW[i1].cProcessName),L"explorer.exe")!=0)
			{
				iconIndex = IsHaveTrayIcon(&sW[i1],pTrayinfo);
				if (iconIndex >= 0 )
				{
					sW[i1].iIconIndex =  iconIndex;
					sW[i1].trayicon = arrTray[iconIndex];
					sW[i1].iHasTrayIcon = 1;
				}
				else
				{
					sW[i1].iHasTrayIcon = 0;
				}
			}
			
		}

		hOnw = ::GetParent(hwnd);
		if(hOnw !=NULL  && IsWindowVisible(hOnw) == TRUE)// && IsEuqalExe(hwnd,L"qq.exe",iForSW) == 0)//For QQ 
		{
			VirtualFree(pszText,0,MEM_RELEASE);
			VirtualFree(cWndText,0,MEM_RELEASE);
			return 1;
		}
		for (i3 = 0;i3<iBaSW;i3++)
		{
			if(lPID == sW[i3].lPID && wcscmp(cWndText,L"")==0 && hwnd != sW[i3].hWnd &&
				/*IsEuqalExe(hwnd,L"qq.exe",iForSW) == 0 && */   hwnd != GetParent(sW[i3].hWnd))
			{
				VirtualFree(pszText,0,MEM_RELEASE);
				VirtualFree(cWndText,0,MEM_RELEASE);
				return 1;
			}
			else if(lPID == sW[i3].lPID)
			{
				iRec = i3;
			}
		}
		if(i3>=iBaSW)
		{
			if(wcscmp(cWndText,L"")==0)
			{
				wcscpy(lvItem.pszText,sW[iRec].cProcessName);
			}
		}
		
		
		imageindex = ImageList_AddIcon(hIconList,hMainIcon);
		lvItem.iImage = imageindex;
		//lvItem.stateMask = LVIS_SELECTED;
		//lvItem.lParam = (LPARAM)(sW + iRec);
		if(lPID == 1520)
		{
			lPID = lPID;
		}
		if((IsBadReadPtr(lvItem.pszText,1024) == 0) && IsInListView(lPID,lvItem.pszText) == 0)
		{
			ListView_InsertItem(hListView,&lvItem);

			memset(cPID,0,10 * sizeof(wchar_t));
			swprintf(cPID,L"%X",lPID);
			wcscpy(lvItem.pszText,cPID);
			lvItem.iSubItem = 1;
			ListView_SetItem(hListView,&lvItem);
			ZeroMemory(cPID,10);
			
			pTrayinfo = NULL;
			memset(&temp_SW,0,sizeof(WndAndHandle));
			temp_SW.lPID = lPID;
			temp_SW.cIconTip = NULL;
			if(lPID != dwExPID)
			{
				iconIndex = IsHaveTrayIcon(&temp_SW,pTrayinfo);
				if (iconIndex >= 0 )
				{
					lvItem.iSubItem = 2;
					if(g_iUseLanguage == 0)
					{
							wcscpy(lvItem.pszText,L"Have");
					}
					else
					{
						wchar_t * pText = new wchar_t[1024];
						memset(pText,0,1024 * sizeof(wchar_t));
						wcscpy(lvItem.pszText,LoadItemStringW(L"IDS_HAVETRAYICON",pText,L"Have"));
						delete [] pText;
					}
					//wcscpy(lvItem.pszText,L"有");//////////////////////////Need International///////////////////////////////
					ListView_SetItem(hListView,&lvItem);
				}
				else
				{
					
				}
			}
		}
		VirtualFree(pszText,0,MEM_RELEASE);
		VirtualFree(cWndText,0,MEM_RELEASE);
	}
	return 1;
}




int GetHideProcessName(DWORD lPID,wchar_t * cHidedName)
{
	wcscpy(cHidedName,L"flashget.exe");

	return 1;
}




int IsInListView(DWORD lPID,wchar_t * cTitle)
{
	int iCount = ListView_GetItemCount(hListView);
	
	wchar_t * cText = new wchar_t[1024];
	wchar_t cPID[100];
	
	int i = 0;
	for(i = 0;i < iCount;i ++)
	{
		memset(cText,0,1024 * sizeof(wchar_t));
		ListView_GetItemText(hListView,i,1,cText,1024 * sizeof(wchar_t));
		memset(cPID,0,100);
		swprintf(cPID,L"%X",lPID);
		if(wcscmp(cPID,cText) != 0)
		{
			continue;
		}
		else
		{
			memset(cText,0,1024 * sizeof(wchar_t) );
			ListView_GetItemText(hListView,i,0,cText,1024 * sizeof(wchar_t));
			if(wcscmp(cTitle,cText) != 0)
			{
				continue;
			}
			else
			{
				delete [] cText;

				return 1;
			}
		}
	}
	
	delete [] cText;

	return 0;
}

int IsEuqalExe(HWND hWnd,wchar_t * cExeName,int iCount)
{
	for(int i = 0;i < iCount;i ++)
	{
		if(sW[i].hWnd == hWnd)
		{
			if(wcscmp(sW[i].cProcessName,cExeName) == 0)
			{
				return 1;
			}
		}
	}



	return 0;
}




int DeleteAll()
{
	EnterCriticalSection(&csDelListView);
	ListView_DeleteAllItems(hListView);
	LeaveCriticalSection(&csDelListView);
	return 1;
}

INT_PTR CALLBACK HotKeyProc(HWND hwndDlg, UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	int iSelInd = 0,iCurSel = 0,iCurSel2 = 0,iCurSel3 = 0;
	char cHK = NULL;
	BOOL bSuc;
	HDC hScreenDC;
	int fX = 0,fY = 0;
	WORD wHK_Res = 0,wHK_HA = 0,wHK_SS = 0;
	LRESULT lRes = 0;
	UINT uModifier = 0,uVK = 0,uMK = 0;
	BYTE uMF = 0;
	DWORD dwErr = 0;
	HWND hStatic_bUseMouse,hStatic_HideWindow,hStatic_ShowHideDragon;
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
//		ChangeWindowStyle(hOptionDialog,0);
		hHK_HA = GetDlgItem(hwndDlg,IDC_HOTKEY_HIDEALL);
		hHK_SS = GetDlgItem(hwndDlg,IDC_HOTKEY_SHOWSELF);
		hRightFirst = GetDlgItem(hwndDlg,IDC_CHECK_RIGHTFIRST);
		hStatic_bUseMouse = GetDlgItem(hwndDlg,IDC_STATIC_ISUSEMOUSE);
		SendMessage(hStatic_bUseMouse,WM_SETFONT,(WPARAM)hFont_New,0);
		hStatic_HideWindow = GetDlgItem(hwndDlg,IDC_STATIC_HIDEALLWINDOWHOTKEY);
		SendMessage(hStatic_HideWindow,WM_SETFONT,(WPARAM)hFont_New,0);
		hStatic_ShowHideDragon = GetDlgItem(hwndDlg,IDC_STATIC_SHOWHIDEDRAGONHOTKEY);
		SendMessage(hStatic_ShowHideDragon,WM_SETFONT,(WPARAM)hFont_New,0);

		
		iSelInd = 16;

		if (sOP.bUseMouse)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECKMOUSE,BST_CHECKED);
			if(sOP.bRightFirst == 1)
			{
				CheckDlgButton(hwndDlg,IDC_CHECK_RIGHTFIRST,BST_CHECKED);
			}
		}
		if(sOP.bUseMiddle)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECK_MIDDBUTTON,BST_CHECKED);
		}
		
		if(sOP.bUseResHK == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECKRESHK,BST_CHECKED);
		}
		else if(sOP.bUseResHK == 0)
		{
			CheckDlgButton(hwndDlg,IDC_CHECKRESHK,BST_UNCHECKED);
		}
		SendMessage(hHK_HA,HKM_SETHOTKEY,sOP.sHaHK.wHK_Res,0);
		SendMessage(hHK_SS,HKM_SETHOTKEY,sOP.sSsHK.wHK_Res,0);
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOGHOTKEY);
		}
		if(sOP.bShowBKTip == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_SHOWBKTIP,BST_CHECKED);
		}
		
		return FALSE;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam) ->code)
		{
		case PSN_KILLACTIVE:


			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_APPLY:
			//MessageBox(hwndDlg,"apply","ok",MB_OK);
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKMOUSE) == BST_UNCHECKED)
			{
				lpBuff [0] = char (0);
				lpBuff [1] = char (10);
				sOP.bUseMouse = lpBuff[0];
			}
			else
			{
				lpBuff [0] = char (1);
				lpBuff [1] = char (10);
				sOP.bUseMouse = lpBuff[0];
				if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_RIGHTFIRST) == BST_UNCHECKED)
				{
					sOP.bRightFirst = 0;
				}
				else
				{
					sOP.bRightFirst = 1;
				}
			}

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_MIDDBUTTON) == BST_UNCHECKED)
			{
				sOP.bUseMiddle = 0;
			}
			else
			{
				sOP.bUseMiddle = 1;
			}




			if (hMouseHook == NULL && (sOP.bUseMouse || sOP.bUseMiddle))
			{
				SetMouseHook(1);
			}
			else if(hMouseHook != NULL && (sOP.bUseMouse == 0 && sOP.bUseMiddle == 0))
			{
				SetMouseHook(0);
			}

///////////////////////////////////////////////////////////////////////////////////////////////
			wHK_HA = (WORD)SendMessage(hHK_HA,HKM_GETHOTKEY,0,0);
			aHideWindowsHotKey = GlobalFindAtom(L"HideWindowsHotKey");
			UnregisterHotKey(hMainDialog,aHideWindowsHotKey);
			aHideWindowsHotKey = GlobalAddAtom(L"HideWindowsHotKey");
			uMF = HIBYTE(wHK_HA);
			uVK = LOBYTE(wHK_HA);
			uModifier = 0;
			if(uMF & HOTKEYF_ALT)
			{
				uModifier |= MOD_ALT;
			}
			if(uMF & HOTKEYF_CONTROL)
			{
				uModifier |= MOD_CONTROL;
			}
			if(uMF & HOTKEYF_SHIFT)
			{
				uModifier |= MOD_SHIFT;
			}
			bSuc = RegisterHotKey(hMainDialog,aHideWindowsHotKey,uModifier,uVK);
			SetLastError (ERROR_SUCCESS);
			GlobalDeleteAtom(aHideWindowsHotKey);
			if(GetLastError() != ERROR_SUCCESS)
			{
				OutputDebugString(L"Delete Atom Error.");
			}
			if (bSuc == 0)
			{
				//MessageBox(hwndDlg,"注册隐藏所有窗口热键失败，请更换再注册。","隐身专家",MB_OK|MB_ICONINFORMATION);
				HDMessageBoxW(L"IDS_MESSAGEBOX_REGHIDEWNDFAILURECHANGE",
					L"Register hide window hotkey failure,please change another.",hwndDlg);

				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;

			}
			sOP.sHaHK.wHK_Res = wHK_HA;
			sOP.sHaHK.uMF     = uModifier;
			sOP.sHaHK.uVK     = uVK;
			//SendDlgItemMessage(hMainDialog,IDC_HOTKEY_HA,HKM_SETHOTKEY,wHK_HA,0);
			SendDlgItemMessage(g_hInfoPanel,IDC_HOTKEY_INFO_HA,HKM_SETHOTKEY,wHK_HA,0);
			EnableWindow(GetDlgItem(hMainDialog,IDC_HOTKEY_HA),FALSE);
//////////////////////////////////////////////////////////////////////////////////////////
			wHK_SS = (WORD)SendMessage(hHK_SS,HKM_GETHOTKEY,0,0);
			aShowSelf = GlobalFindAtom(L"ShowSelf");
			UnregisterHotKey(hMainDialog,aShowSelf);
			aShowSelf = GlobalAddAtom(L"ShowSelf");
			uMF = HIBYTE(wHK_SS);
			uVK = LOBYTE(wHK_SS);
			uModifier = 0;
			if(uMF & HOTKEYF_ALT)
			{
				uModifier |= MOD_ALT;
			}
			if(uMF & HOTKEYF_CONTROL)
			{
				uModifier |= MOD_CONTROL;
			}
			if(uMF & HOTKEYF_SHIFT)
			{
				uModifier |= MOD_SHIFT;
			}
			bSuc = RegisterHotKey(hMainDialog,aShowSelf,uModifier,uVK);
			if (bSuc == 0)
			{
				//MessageBox(hwndDlg,"注册显示程序主窗口热键失败，请更换再注册。","隐身专家",MB_OK|MB_ICONINFORMATION);
				HDMessageBoxW(L"IDS_MESSAGEBOX_REGSHOWHIDEDRAGONFAILURECHANGE",
					L"Register Show HideDragon hotkey failure,please retry another.",hwndDlg);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;

			}
			sOP.sSsHK.wHK_Res = wHK_SS;
			sOP.sSsHK.uMF     = uModifier;
			sOP.sSsHK.uVK     = uVK;
			//SendDlgItemMessage(hMainDialog,IDC_HOTKEY_SS,HKM_SETHOTKEY,wHK_SS,0);
			SendDlgItemMessage(g_hInfoPanel,IDC_HOTKEY_INFO_SS,HKM_SETHOTKEY,wHK_SS,0);
			EnableWindow(GetDlgItem(hMainDialog,IDC_HOTKEY_SS),FALSE);

////////////////////////////////////////////////////////////////////////////////////////////////

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECKRESHK) == BST_CHECKED)
			{
				sOP.bUseResHK = 1;
			}
			else
			{
				sOP.bUseResHK = 0;
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_SHOWBKTIP) == BST_CHECKED)
			{
				sOP.bShowBKTip = 1;
			}
			else
			{
				sOP.bShowBKTip = 0;
			}
			if(sOP.bUseMouse == 0 && sOP.bUseMiddle == 0)
			{
				if(g_iUseLanguage == 0)
				{
					::SetWindowTextW(hMouse,L"Disable");
				}
				else
				{
					wchar_t * pStatusText = new wchar_t[1024];
					::SetWindowTextW(hMouse,LoadItemStringW(L"IDS_MOUSE_STATUS_DISABLED",
						pStatusText,L"Disable"));
					delete [] pStatusText;
				}
				if(IsWindowEnabled(hListView) == FALSE)
				{
					wcscpy(g_pTipText[0][0],L"IDS_USAGETIP_REPRESSHOTKEYTOSHOWWINDOW");
					wcscpy(g_pTipText[0][1],L"Re press Hide Window hotkey to show window.");
					SetTipText(L"IDS_USAGETIP_MOUSEHOOKDISABLED",
						L"Use Mouse hide window is disabled.");
				}
				else
				{
					wcscpy(g_pTipText[0][0],L"IDS_USAGETIP_PRESSHOTKEYTOHIDEWINDOW");
					wcscpy(g_pTipText[0][1],L"Re press Hide Window hotkey to show window.");
					SetTipText(L"IDS_USAGETIP_MOUSEHOOKDISABLED",
						L"Use Mouse hide window is disabled.");
				}
			}
			else if(sOP.bUseMouse || sOP.bUseMiddle)
			{
				if(g_iUseLanguage == 0)
				{
					::SetWindowTextW(hMouse,L"Enable");
				}
				else
				{
					wchar_t * pStatusText = new wchar_t[1024];
					::SetWindowTextW(hMouse,LoadItemStringW(L"IDS_MOUSE_STATUS_ENABLED",
						pStatusText,L"Enable"));
					delete [] pStatusText;
				}
				if(IsWindowEnabled(hListView) == FALSE)
				{
					if(sOP.bUseMiddle)
					{
						wcscpy(g_pTipText[0][0],L"IDS_USAGETIP_RECLICKMIDDLESHOWWINDOW");
						wcscpy(g_pTipText[0][1],
							L"Reclick mouse middle button to show all windows.");
						SetTipText(L"IDS_USAGETIP_RECLICKMIDDLESHOWWINDOW",
						L"Reclick mouse middle button to show all windows.");
					}
					else
					{
						wcscpy(g_pTipText[0][0],L"IDS_USAGETIP_RECLICKRIGHTLEFTSHOWWINDOW");
						wcscpy(g_pTipText[0][1],
							L"Reclick mouse right + left button to show all windows.");
						SetTipText(L"IDS_USAGETIP_RECLICKRIGHTLEFTSHOWWINDOW",
							L"Reclick mouse right + left button to show all windows.");
					}
				}
				else
				{
					if(sOP.bUseMiddle)
					{
						wcscpy(g_pTipText[0][0],L"IDS_USAGETIP_CLICKMIDDLEHIDEWND");
						wcscpy(g_pTipText[0][1],
							L"Click mouse Middle button to hide all windows.");
						SetTipText(L"IDS_USAGETIP_CLICKMIDDLEHIDEWND",
							L"Click mouse Middle button to hide all windows.");
					}
					else
					{
						wcscpy(g_pTipText[0][0],L"IDS_USAGETIP_CLICKLEFTRIGHTHIDEWND");
						wcscpy(g_pTipText[0][1],
							L"Click mouse right + left button to hide all windows.");
						SetTipText(L"IDS_USAGETIP_CLICKLEFTRIGHTHIDEWND",
							L"Click mouse right + left button to hide all windows.");
					}
				}
			}

			WriteOptionFile(sOP);

			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
		
			return FALSE;
		default:
			return FALSE;
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CHECK_MIDDBUTTON:
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECK_MIDDBUTTON) == BST_UNCHECKED)
			{
				if(sOP.bShowTip)
				{
					HDMessageBoxW(L"IDS_MESSAGEBOX_IFNOTCONFLICT",
						L"The cancellation of the option will not be recommended if it does not conflict with other programs.",hwndDlg);
				}
			}
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			break;
		case IDC_CHECKMOUSE:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKMOUSE) == BST_UNCHECKED)
			{
				if(sOP.bShowTip)
				{
					HDMessageBoxW(L"IDS_MESSAGEBOX_IFNOTCONFLICT",
						L"The cancellation of the option will not be recommended if it does not conflict with other programs.",hwndDlg);
				}
				EnableWindow(hRightFirst,FALSE);
			}
			else
			{
				EnableWindow(hRightFirst,TRUE);
			}
		return TRUE;   
		case IDC_CHECKRESHK:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_SHOWBKTIP:
		case IDC_CHECK_RIGHTFIRST:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_HOTKEY_HIDEALL:
		case IDC_HOTKEY_SHOWSELF:
			if(HIWORD(wParam)== EN_CHANGE)
			{
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
			}
			break;
		
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
	SYSTEMTIME CurTime;
	if(g_StartTime.wYear == 0 && sOP.bUseMouse == 1)
	{
		return;
	}
	if(g_iAutoRun == 1 && g_iStarted == 0 && sOP.bUseMouse)
	{
		GetSystemTime(&CurTime);
		if(CurTime.wSecond - g_StartTime.wSecond <= 5)
		{
			return;
		}
		else
		{
			g_iStarted = 1;
			SendMessage(hwnd,WM_COMMAND,IDC_REFRESH,0);
			SendMessage(hwnd,WM_COMMAND,IDC_REFRESH,0);
		}
	}
	if (bRef != 0 && IsWindowVisible(hMainDialog))
	{
//		SendMessage(hwnd,WM_COMMAND,IDC_REFRESH,0);
	}
	if(bRef != 0 && (iBKItemCount > 0 && sOP.bAutoHide) || 
		(!IsWindowEnabled(hListView) && sOP.bAutoHide_QH == 1))
	{
		SendMessage(hMainDialog ,WM_COMMAND,IDC_REFRESH,0);
	}
}


VOID CALLBACK SetTipTextProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	if(g_iTip >= g_iTipCount)
	{
		g_iTip = 0;
	}
	SetTipText(g_pTipText[g_iTip][0],g_pTipText[g_iTip][1]);
	g_iTip ++;

	return ;
}



int CreateOptionProps()
{
	PROPSHEETPAGEW psp[6];
	PROPSHEETHEADERW psh ;

	wchar_t * pTitle = new wchar_t[256],* pPasswordTitle = new wchar_t[256],*pBackgroundTitle= new wchar_t[256],
		*pQuickHideTitle = new wchar_t[256],*pMouseMotionTitle = new wchar_t[256],* pOtherTitle = new wchar_t[256],
		* pOptionDialogTitle = new wchar_t [256], * pAutoUpdateTitle = new wchar_t[256];
	
	psp[0].dwSize = sizeof(PROPSHEETPAGEW);
	psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInst;
	psp[0].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGHOTKEY);
	psp[0].pszIcon = NULL;
	psp[0].pfnDlgProc =(int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))HotKeyProc;
	if(g_iUseLanguage == 0)
	{
		psp[0].pszTitle = L"Hotkey";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pTitle,0,256 * sizeof(wchar_t));
		wcscpy(pTitle,LoadItemStringW(L"IDS_HOTKEY_TITLE",pText,L"Hotkey"));
		delete [] pText;
		psp[0].pszTitle = pTitle;
	}
	psp[0].lParam = NULL;


	psp[1].dwSize = sizeof(PROPSHEETPAGEW);
	psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInst;
	psp[1].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGPASSWORD);
	psp[1].pszIcon = NULL;
	psp[1].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))PasswordProc;
	if(g_iUseLanguage == 0)
	{
		psp[1].pszTitle = L"Password Protection";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pPasswordTitle,0,256 * sizeof(wchar_t));
		wcscpy(pPasswordTitle,LoadItemStringW(L"IDS_PASSWORD_TITLE",pText,L"Password Protection"));
		delete [] pText;
		psp[1].pszTitle = pPasswordTitle;
	}
	psp[1].lParam = NULL;



	psp[2].dwSize = sizeof(PROPSHEETPAGEW);
	psp[2].dwFlags = PSP_USETITLE;
	psp[2].hInstance = hInst;
	psp[2].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGBK);
	psp[2].pszIcon = NULL;
	psp[2].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))BKProc;
	if(g_iUseLanguage == 0)
	{
		psp[2].pszTitle = L"Backgrounding";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pBackgroundTitle,0,256 * sizeof(wchar_t));
		wcscpy(pBackgroundTitle,LoadItemStringW(L"IDS_BACKGROUND_TITLE",pText,L"Backgrounding"));
		delete [] pText;
		psp[2].pszTitle = pBackgroundTitle;
	}

	psp[2].lParam = NULL;


	psp[3].dwSize = sizeof(PROPSHEETPAGEW);
	psp[3].dwFlags = PSP_USETITLE;
	psp[3].hInstance = hInst;
	psp[3].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGQUICKHIDE);
	psp[3].pszIcon = NULL;
	psp[3].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))QuickHideProc;
	if(g_iUseLanguage == 0)
	{
		psp[3].pszTitle = L"Quick Hide Window";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pQuickHideTitle,0,256 * sizeof(wchar_t));
		wcscpy(pQuickHideTitle,LoadItemStringW(L"IDS_QUICKHIDE_TITLE",pText,L"Quick Hide Window"));
		delete [] pText;
		psp[3].pszTitle = pQuickHideTitle;
	}
	psp[3].lParam = NULL;


//	psp[4].dwSize = sizeof(PROPSHEETPAGE);
//	psp[4].dwFlags = PSP_USETITLE;
//	psp[4].hInstance = hInst;
//	psp[4].pszTemplate = MAKEINTRESOURCE(IDD_DIALOG_MOUSEMOTION);
//	psp[4].pszIcon = NULL;
//	psp[4].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))MouseMotionProc;
//	if(g_iUseLanguage == 0)
//	{
//		psp[4].pszTitle = "鼠标动作";
//	}
//	else
//	{
//		wchar_t * pText = new wchar_t[1024];
//		memset(pText,0,1024);
//		memset(pMouseMotionTitle,0,256);
//		wcscpy(pMouseMotionTitle,LoadItemString("IDS_MOUSEMOTION_TITLE",pText,"鼠标动作"));
//		delete [] pText;
//		psp[4].pszTitle = pMouseMotionTitle;
//	}
//	psp[4].lParam = NULL;


	
	psp[4].dwSize = sizeof(PROPSHEETPAGEW);
	psp[4].dwFlags = PSP_USETITLE;
	psp[4].hInstance = hInst;
	psp[4].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOG_AUTOUPDATE);
	psp[4].pszIcon = NULL;
	psp[4].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))AutoUpdateProc;
	if(g_iUseLanguage == 0)
	{
		psp[4].pszTitle = L"Automatic Update";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pAutoUpdateTitle,0,256 * sizeof(wchar_t));
		wcscpy(pAutoUpdateTitle,LoadItemStringW(L"IDS_AUTOUPDATE_TITLE",pText,L"Automatic Update"));
		delete [] pText;
		psp[4].pszTitle = pAutoUpdateTitle;
	}
	psp[4].lParam = NULL;



	psp[5].dwSize = sizeof(PROPSHEETPAGEW);
	psp[5].dwFlags = PSP_USETITLE;
	psp[5].hInstance = hInst;
	psp[5].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGOTHER);
	psp[5].pszIcon = NULL;
	psp[5].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))OtherProc;
	if(g_iUseLanguage == 0)
	{
		psp[5].pszTitle = L"Other";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pOtherTitle,0,256 * sizeof(wchar_t));
		wcscpy(pOtherTitle,LoadItemStringW(L"IDS_OTHERSETTING_TITLE",pText,L"Other"));
		delete [] pText;
		psp[5].pszTitle = pOtherTitle;
	}
	psp[5].lParam = NULL;


	psh.dwSize = sizeof(PROPSHEETHEADERW);
	psh.dwFlags = PSH_PROPSHEETPAGE;
	psh.hwndParent = hMainDialog;
	psh.hInstance = hInst;
	psh.pszIcon = NULL;
	if(g_iUseLanguage == 0)
	{
		psh.pszCaption = L"Options";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pText,0,1024 * sizeof(wchar_t));
		memset(pOptionDialogTitle,0,256 * sizeof(wchar_t));
		wcscpy(pOptionDialogTitle,LoadItemStringW(L"IDS_OPTIONDIALOG_TITLE",pText,L"Options"));
		delete [] pText;
		psh.pszCaption = pOptionDialogTitle;
	}

	psh.nStartPage = 0;

	psh.nPages = 6;
	psh.ppsp = (_PROPSHEETPAGEW *)&psp;

	int iRet = PropertySheetW(&psh);

	
	delete [] pTitle;
	delete [] pPasswordTitle;
	delete [] pBackgroundTitle;
	delete [] pQuickHideTitle;
	delete [] pMouseMotionTitle;
	delete [] pOtherTitle;
	delete [] pOptionDialogTitle;
	delete [] pAutoUpdateTitle;


	

	return iRet;
}


INT_PTR CALLBACK OtherProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static HWND hSpin = NULL,hEditInterval = NULL;
	static int iR = 0;
	wchar_t * cMBShortcutName = NULL, * cShortcutName = NULL,* cPath_QuickLaunch = NULL;
	wchar_t * cWorkingDic = NULL, * cCurrentPath = NULL, * cRegKey = NULL;
	HWND hStatic_OtherSystem,hStatic_WhenExitWindow;


	switch(uMsg)
	{
	case WM_INITDIALOG:
		hOptionDialog = ::GetParent(hwndDlg);
		hRadio_ShowAll = GetDlgItem(hwndDlg,IDC_RADIOSHOW);
		hHideMain = GetDlgItem(hwndDlg,IDC_CHECK_HIDEMAINWND);

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
		}

		if(sOP.bHideMain == 1)
		{
			
			CheckDlgButton(hwndDlg,IDC_CHECK_HIDEMAINWND,BST_CHECKED);
		}
		if(sOP.bHideIcon == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_HIDEICON,BST_CHECKED);
		}

		if(sOP.bAddQuickLaunch == 1)
		{
			::CheckDlgButton(hwndDlg,IDC_CHECK_ADDQL,BST_CHECKED);
		}

		if(sOP.bHideIcon == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_NOTSHOWICON,BST_CHECKED);
		}

		if(sOP.bNotShowProcess == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_NOTSHOWPROCESS,BST_CHECKED);
			
		}

#ifndef _HIDEPROCESS
		ShowWindow(GetDlgItem(hwndDlg,IDC_CHECK_NOTSHOWPROCESS),SW_HIDE);
#endif

		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOGOTHER);
		}





		hStatic_OtherSystem = GetDlgItem(hwndDlg,IDC_STATIC_OTEHRSYSTEM);
		SendMessage(hStatic_OtherSystem,WM_SETFONT,(WPARAM)hFont_New,0);
		hStatic_WhenExitWindow = GetDlgItem(hwndDlg,IDC_STATIC_WHENEXITWINDOW);
		SendMessage(hStatic_WhenExitWindow,WM_SETFONT,(WPARAM)hFont_New,0);

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
//			iR = GetDlgItemInt(hwndDlg,IDC_EDITINTERVAL,NULL,FALSE);
//			if ((iR <1 || iR>10)&& ::IsDlgButtonChecked(hwndDlg,IDC_CHECKREFRESH)!= BST_CHECKED)
//			{
//				MessageBox(hwndDlg,L"刷新间隔只能是1~9之间的数字。",L"隐身专家",MB_OK|MB_ICONINFORMATION);
//				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
//				return TRUE;
//			}
			//break;
//			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_APPLY:
			//SetFilePointer(hOptionFile,6,NULL,FILE_BEGIN);
			if(::IsDlgButtonChecked(hwndDlg,IDC_CHECKREFRESH) == BST_CHECKED)
			{
				lpBuff[0] = 0;
			}
			else
			{
				lpBuff[0] = wchar_t(iR);
			}
			lpBuff[1] = wchar_t (10);
			//WriteFile(hOptionFile,lpBuff,2,&lWritten,NULL);
			sOP.iRefInterval = lpBuff[0];
			/////////////////////////////////////////////////////////////////////////////////////////
			if(::IsDlgButtonChecked(hwndDlg,IDC_RADIOKILL) == BST_CHECKED)
			{
				lpBuff[0] = 'K';lpBuff[1] = wchar_t (10);
			}
			else if(::IsDlgButtonChecked(hwndDlg,IDC_RADIOSHOW) == BST_CHECKED)
			{
				lpBuff[0] = 'S';lpBuff[1] = wchar_t (10);
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
				dwPathSize = 1024;
				cCurrentPath = (wchar_t*)new wchar_t[dwPathSize];
				memset(cCurrentPath,0,1024 * sizeof(wchar_t));
				GetModuleFileNameW(NULL,cCurrentPath,dwPathSize);
				cRegKey	= (wchar_t*)VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				memset(cRegKey,0,2048 * sizeof(wchar_t));
				wcscpy(cRegKey,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
				RegOpenKeyW(HKEY_LOCAL_MACHINE,cRegKey,&hRegAutoRun);
				sOP.bAutoRun = 1;
				wcscat(cCurrentPath,L" -A");
				RegSetValueExW(hRegAutoRun,L"HideDragon",NULL,REG_SZ,
				(const BYTE*)cCurrentPath,/*1024*/ wcslen(cCurrentPath) * sizeof(cCurrentPath) + 2 );
				RegCloseKey(hRegAutoRun);
				VirtualFree(cRegKey,0,MEM_RELEASE);
				delete [] cCurrentPath;
			}
			else
			{
				sOP.bAutoRun = 0;
				cRegKey	= (wchar_t*)VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
				memset(cRegKey,0,2048 * sizeof(wchar_t));
				wcscpy(cRegKey,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
				RegOpenKeyW(HKEY_LOCAL_MACHINE,cRegKey,&hRegAutoRun);
				VirtualFree(cRegKey,0,MEM_RELEASE);
				RegDeleteValueW(hRegAutoRun,L"HideDragon");
				RegCloseKey(hRegAutoRun);
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEICON))
			{
				sOP.bHideIcon = 1;
			}
			else
			{
				sOP.bHideIcon = 0;
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEMAINWND))
			{
				sOP.bHideMain = 1;
			}
			else
			{
				sOP.bHideMain = 0;
			}
			cPath_QuickLaunch = (wchar_t*)new wchar_t[1024];
			memset(cPath_QuickLaunch,0,1024 * sizeof(wchar_t));
			SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&lpItemIdList);
			SHGetPathFromIDListW(lpItemIdList,cPath_QuickLaunch);
			if(CreateFileW(cPath_QuickLaunch,GENERIC_READ,FILE_SHARE_READ,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL) == NULL)
			{
				//MessageBox(hwndDlg,"path not exist","隐身专家",MB_OK);
			}
			CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,
				IID_IShellLink,(void**)&pShellLink);
			pShellLink->QueryInterface(IID_IPersistFile,(void**)&ppf);
			cCurrentPath = new wchar_t [1024];
			memset(cCurrentPath,0,1024 * sizeof(wchar_t));
			
			GetModuleFileNameW(NULL,cCurrentPath,1024);
			
			if (pShellLink->SetPath(cCurrentPath) == NOERROR)
			{
				//MessageBox(hwndDlg,"OK","OK",MB_OK);
			}
			cWorkingDic = (wchar_t*)new wchar_t [1024];
			memset(cWorkingDic,0,1024 * sizeof(wchar_t));
			wcscpy(cWorkingDic,cCurrentPath);
			PathRemoveFileSpecW(cWorkingDic);
			if(pShellLink->SetWorkingDirectory(cWorkingDic)==NOERROR)
			{
				//MessageBox(NULL,"SETOK","OK",MB_OK);
			}

			sOP.bAddQuickLaunch = 1;
			cShortcutName = (wchar_t*)new wchar_t[1024];
			memset(cShortcutName,0,dwPathSize * sizeof(wchar_t));
			wcscpy(cShortcutName,cPath_QuickLaunch);
			wcscat(cShortcutName,L"\\Microsoft\\Internet Explorer\\Quick Launch\\");
			if(g_iUseLanguage == 0)
			{
				wcscat(cShortcutName,L"HideDragon 3.4");//"隐身专家 2.9");
			}
			else
			{
				wchar_t * pText = new wchar_t[1024];
				wcscat(cShortcutName,L"HideDragon 3.4"/*LoadItemStringW(L"IDS_TRAYICON_BALLTIPTITLE",pText,
					L"HideDragon 3.21")*/);
				delete [] pText;
			}
			wcscat(cShortcutName,L".lnk");
			if (::IsDlgButtonChecked(hwndDlg,IDC_CHECK_ADDQL)== BST_CHECKED)
			{

//				cMBShortcutName = new wchar_t[1024];
//				memset(cMBShortcutName,0,1024 * );
//				MultiByteToWidewchar_t(CP_ACP,NULL,cShortcutName,wcslen(cShortcutName),
//					(unsigned short*)cMBShortcutName,500);
				hr = ppf->Save( (unsigned short *)cShortcutName,FALSE);
//				delete [] cMBShortcutName;
				if (SUCCEEDED(hr))
				{
					//MessageBox(hwndDlg,"OK","OK",MB_OK);
				}
			}
			else
			{
				sOP.bAddQuickLaunch = 0;
				DeleteFileW(cShortcutName);
			}

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_NOTSHOWICON) == BST_CHECKED)
			{
				sOP.bHideIcon = 1;
			}
			else
			{
				sOP.bHideIcon = 0;
			}

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_NOTSHOWPROCESS))
			{
				sOP.bNotShowProcess = 1;
			}
			else
			{
				sOP.bNotShowProcess = 0;
			}

			delete [] cShortcutName;
			delete [] cWorkingDic;
			delete [] cPath_QuickLaunch;
			delete [] cCurrentPath;
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
//				MessageBox(hwndDlg,
//					L"如果取消此选项，你必须点击‘刷新’按钮来刷新窗口列表，"
//					"如果在选取保留窗口之前忘记手动刷新，程序有时会出错。如果系统性能较好，不推荐选中此选项。",
//					"隐身专家",MB_OK|MB_ICONINFORMATION);
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
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_HIDEMAINWND:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_NOTSHOWICON:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_HIDEICON:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_NOTSHOWPROCESS:
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
	HWND hStatic_BKOption,hStatic_AHOption,hStatic_DCOption;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(sOP.bAutoHide == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_AUTOHIDEBK,BST_CHECKED);
			
		}

		if(sOP.doubleClick == 0)
		{
			CheckRadioButton(hwndDlg,IDC_RADIO_BKITEMNOW,IDC_RADIO_CHECKITEM,IDC_RADIO_BKITEMNOW);
		}
		else if(sOP.doubleClick == 1)
		{
			CheckRadioButton(hwndDlg,IDC_RADIO_BKITEMNOW,IDC_RADIO_BRINGITEM,IDC_RADIO_BRINGITEM);
		}
		else if(sOP.doubleClick == 2)
		{
			CheckRadioButton(hwndDlg,IDC_RADIO_BKITEMNOW,IDC_RADIO_CHECKITEM,IDC_RADIO_CHECKITEM);
		}
		if(sOP.bAutoHideProgram == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_AUTOHIDEANY,BST_CHECKED);
			
		}


		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOGBK);
		}
		hStatic_BKOption = GetDlgItem(hwndDlg,IDC_STATIC_BACKGROUNDOPTION);
		SendMessage(hStatic_BKOption,WM_SETFONT,(WPARAM)hFont_New,0);
		hStatic_AHOption = GetDlgItem(hwndDlg,IDC_STATIC_AUTOHIDE);
		SendMessage(hStatic_AHOption,WM_SETFONT,(WPARAM)hFont_New,0);
		hStatic_DCOption = GetDlgItem(hwndDlg,IDC_STATIC_DOUBLECLICK);
		SendMessage(hStatic_DCOption,WM_SETFONT,(WPARAM)hFont_New,0);
	

		return FALSE;
	case WM_NOTIFY:
		switch((LPNMHDR(lParam))->code)
		{
		case PSN_APPLY:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTOHIDEBK))
			{
				sOP.bAutoHide = 1;
				CheckDlgButton(g_TabWnd[2],IDC_TAB_CHECK_AUTOHIDE,BST_CHECKED);
			}
			else
			{
				sOP.bAutoHide = 0;
				CheckDlgButton(g_TabWnd[2],IDC_TAB_CHECK_AUTOHIDE,BST_UNCHECKED);
			}


			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTOHIDEANY))
			{
				sOP.bAutoHideProgram = 1;
				if(g_AutoHideHandle ==  NULL)
				{
					CreateAutoHideThread();
				}
				CheckDlgButton(g_TabWnd[7],IDC_AUTOHIDE_ENABLEAH,BST_CHECKED);
			}
			else
			{
				sOP.bAutoHideProgram = 0;
				CheckDlgButton(g_TabWnd[7],IDC_AUTOHIDE_ENABLEAH,BST_UNCHECKED);
			}



			if(::IsDlgButtonChecked(hwndDlg,IDC_RADIO_BKITEMNOW) == BST_CHECKED)
			{
				sOP.doubleClick = 0;
			}
			else if(::IsDlgButtonChecked(hwndDlg,IDC_RADIO_BRINGITEM) == BST_CHECKED)
			{
				sOP.doubleClick = 1;
			}
			else if(::IsDlgButtonChecked(hwndDlg,IDC_RADIO_CHECKITEM) == BST_CHECKED)
			{
				sOP.doubleClick = 2;
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
		case IDC_RADIO_BKITEMNOW:
			PropSheet_Changed(GetParent(hwndDlg),hwndDlg);
			break;
		case IDC_RADIO_BRINGITEM:
			PropSheet_Changed(GetParent(hwndDlg),hwndDlg);
			break;
		case IDC_RADIO_CHECKITEM:
			PropSheet_Changed(GetParent(hwndDlg),hwndDlg);
			break;
		case IDC_CHECK_AUTOHIDEBK:
			PropSheet_Changed(GetParent(hwndDlg),hwndDlg);
			
			break;
		case IDC_CHECK_AUTOHIDEANY:
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
	PROPSHEETPAGEW psp[3];
	PROPSHEETHEADERW psh;

	wchar_t * pAboutTitle = new wchar_t[256],*pReadMeTitle = new wchar_t[256],*pHistoryTitle = new wchar_t[256],
		*pAboutOptionTitle = new wchar_t[256];


	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags = PSP_USETITLE;
	psp[0].hInstance = hInst;
	psp[0].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGABOUT);
	psp[0].pszIcon = NULL;
	psp[0].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))AboutProc;
	if(g_iUseLanguage == 0)
	{
		psp[0].pszTitle = L"Copyrights";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pAboutTitle,0,256 * sizeof(wchar_t));
		wcscpy(pAboutTitle,LoadItemStringW(L"IDS_ABOUT_TITLE",pText,L"Copyrights"));
		psp[0].pszTitle = pAboutTitle;
		delete [] pText;

	}
	psp[0].lParam = NULL;

	psp[1].dwSize = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags = PSP_USETITLE;
	psp[1].hInstance = hInst;
	psp[1].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGREADME);
	psp[1].pszIcon = NULL;
	psp[1].pfnDlgProc = (int (__stdcall *)(struct HWND__ *,unsigned int,unsigned int,long))ReadMeProc;
	if(g_iUseLanguage == 0)
	{
		psp[1].pszTitle = L"Readme";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pReadMeTitle,0,256 * sizeof(wchar_t));
		wcscpy(pReadMeTitle,LoadItemStringW(L"IDS_README_TITLE",pText,L"Readme"));
		psp[1].pszTitle = pReadMeTitle;
		delete [] pText;
	}
	psp[1].lParam = NULL;

	psp[2].dwSize = sizeof(PROPSHEETPAGE);
	psp[2].dwFlags = PSP_USETITLE;
	psp[2].hInstance = hInst;
	psp[2].pszTemplate = MAKEINTRESOURCEW(IDD_DIALOGHISTORY);
	psp[2].pszIcon = NULL;
	psp[2].pfnDlgProc = (int (_stdcall*)(struct HWND__*,unsigned int,unsigned int ,long))HistoryProc;
	if(g_iUseLanguage == 0)
	{
		psp[2].pszTitle = L"History";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pHistoryTitle,0,256 * sizeof(wchar_t));
		wcscpy(pHistoryTitle,LoadItemStringW(L"IDS_HISTORY_TITLE",pText,L"History"));
		psp[2].pszTitle = pHistoryTitle;
		delete [] pText;
	}
	psp[2].lParam = NULL;


	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE|PSH_NOAPPLYNOW|PSH_NOCONTEXTHELP;
	psh.hwndParent = hMainDialog;
	psh.hInstance = hInst;
	psh.pszIcon = NULL;
	psh.nStartPage = 0;
//	psh.pStartPage = MAKEINTRESOURCEW(IDD_DIALOGABOUT);

	
	if(g_iUseLanguage == 0)
	{
		psh.pszCaption = L"About...";
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		memset(pAboutOptionTitle,0,256 * sizeof(wchar_t));
		wcscpy(pAboutOptionTitle,LoadItemStringW(L"IDS_ABOUTOPTION_TITLE",pText,L"About..."));
		psh.pszCaption = pAboutOptionTitle;
		delete [] pText;
	}

	psh.nPages = 3;
	psh.ppsp = (_PROPSHEETPAGEW *)&psp;

	

	LoadLibraryW(L"RICHED20.DLL");




	int iRet = PropertySheetW(&psh);

	delete [] pAboutTitle;
	delete [] pReadMeTitle;
	delete [] pHistoryTitle;
	delete [] pAboutOptionTitle;




	

	return iRet;

}


INT_PTR CALLBACK ReadMeProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	
	static HANDLE hReadMe ;
	EDITSTREAM es;
	static HWND hRE;
	CHARRANGE crSel;
	HBRUSH wbr;


 

	switch(uMsg)
	{
	case WM_INITDIALOG:
		//hEditReadme = ::GetDlgItem(hwndDlg,IDC_EDITREADME);
		//::SetWindowText(hEditReadme,cCont);
		hOptionDialog = ::GetParent(hwndDlg);
		hRE = ::GetDlgItem(hwndDlg,IDC_RICHEDIT2);


		{
			wchar_t * cCurrentPath = new wchar_t [1024];
			memset(cCurrentPath,0,1024 * sizeof(wchar_t));
			GetModuleFileNameW(NULL,cCurrentPath,1024);
			wchar_t * cWorkingDic = (wchar_t *)new wchar_t [1024];
			memset(cWorkingDic,0,1024 * sizeof(wchar_t));
			wcscpy(cWorkingDic,cCurrentPath);
			PathRemoveFileSpecW(cWorkingDic);

			SetCurrentDirectory(cWorkingDic);
			wcscat(cWorkingDic,L"\\readme.rtf");



			if (hReadMe = CreateFileW(cWorkingDic,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,NULL))
			{
				es.dwCookie = (DWORD_PTR)hReadMe;
				es.dwError  = 0;
				es.pfnCallback  = EditStreamCallback;

				SendMessage(hRE,EM_STREAMIN,(LPARAM)SF_RTF,(WPARAM)&es);
				CloseHandle(hReadMe);
			}
			delete [] cCurrentPath;
			delete [] cWorkingDic;
		}
		//PropSheet_SetCurSel(::GetParent(hwndDlg),NULL,0);
     	PostMessageW(hwndDlg,INFO_DISPLAY,0,0);

		
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
		wbr = CreateSolidBrush(RGB(255,255,255));
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

		{
			wchar_t * cFileName = new wchar_t [1024];
			memset(cFileName,0,1024 * sizeof(wchar_t));
			GetModuleFileNameW(NULL,cFileName,1024);
			PathRemoveFileSpecW(cFileName);

			SetCurrentDirectory(cFileName);
			wcscat(cFileName,L"\\VerHistory.rtf");


			if (hVerHist = CreateFile(cFileName,GENERIC_READ,
				FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL))
			{
				es.dwCookie = (DWORD_PTR)hVerHist;
				es.dwError  = 0;
				es.pfnCallback  = EditStreamCallback;

				SendMessage(hRE,EM_STREAMIN,(LPARAM)SF_RTF,(WPARAM)&es);
				CloseHandle(hVerHist);
			}
			delete [] cFileName;
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

extern HWND hToolTip_tray;
extern HWND hToolTip_allwnd;

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
		SendMessageW(hBalTip,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);
		SendMessageW(hToolTip,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);
		SendMessageW(hToolTip_tray,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);
		SendMessageW(hToolTip_allwnd,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);
		SendMessageW(hToolTip_about,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);
		SendMessageW(hToolTip_reg,TTM_RELAYEVENT,0,(LPARAM)(LPMSG)&msg);

		break;
	default:
		break;
	}
	return (CallNextHookEx(hGM,nCode,wParam,lParam));

}

int WriteOptionFile(OptionFile sOP)
{
	HANDLE hOptionFile_Write;
	int iFileSize = sizeof(OptionFile)+36+36+MAX_PATH * sizeof(wchar_t);
	char * cOptionPacket = new char[iFileSize];
	memset(cOptionPacket,0,iFileSize);

	cOptionFileName = (wchar_t*) new wchar_t[1024];
	memset(cOptionFileName,0,1024 * sizeof(wchar_t));
	//GetModuleFileName(hInst,cOptionFileName,1024);
	//PathRemoveFileSpec(cOptionFileName);
	SHGetSpecialFolderPath(NULL, cOptionFileName, CSIDL_PERSONAL, FALSE);
	wcscat(cOptionFileName,L"\\HideDragon");
	CreateDirectory(cOptionFileName,NULL);
	wcscat(cOptionFileName,L"\\misc34.hdo");

	char * temp = cOptionPacket;
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
	memcpy(cOptionPacket,sOP.cPassword,15 * sizeof(wchar_t));
	cOptionPacket += 15 * sizeof(wchar_t);
	memcpy(cOptionPacket,sOP.cTrailRun,MAX_PATH * sizeof(wchar_t));
	cOptionPacket += MAX_PATH * sizeof(wchar_t);
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
	*(int*)cOptionPacket = sOP.bReserveSel;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bNeedStart;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bAutoHide_QH;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bReserveHabit;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bShowTip;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bShowBKTip;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.iAutoUpdate;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bRightFirst;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bUseMiddle;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bUseResHK;
	cOptionPacket += sizeof(int);
	*(UINT*)cOptionPacket = sOP.sResHK.uMF;
	cOptionPacket += sizeof(UINT);
	*(UINT*)cOptionPacket = sOP.sResHK.uVK;
	cOptionPacket += sizeof(UINT);
	*(WORD*)cOptionPacket = sOP.sResHK.wHK_Res;
	cOptionPacket += sizeof(WORD);
	*(UINT*)cOptionPacket = sOP.sHaHK.uMF;
	cOptionPacket += sizeof(UINT);
	*(UINT*)cOptionPacket = sOP.sHaHK.uVK;
	cOptionPacket += sizeof(UINT);
	*(WORD*)cOptionPacket = sOP.sHaHK.wHK_Res;
	cOptionPacket += sizeof(WORD);
	*(UINT*)cOptionPacket = sOP.sSsHK.uMF;
	cOptionPacket += sizeof(UINT);
	*(UINT*)cOptionPacket = sOP.sSsHK.uVK;
	cOptionPacket += sizeof(UINT);
	*(WORD*)cOptionPacket = sOP.sSsHK.wHK_Res;
	cOptionPacket += sizeof(WORD);
	*(int*)cOptionPacket = sOP.bHideAllTray;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bCloseInfoPanel;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bAutoChangeIconTitle;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bBeginnerModel;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bHideProcess;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bNotShowProcess;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bCleanAfterHide;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.bAutoHideProgram;
	cOptionPacket += sizeof(int);
	*(int*)cOptionPacket = sOP.doubleClick;
	cOptionPacket += sizeof(int);



	cOptionPacket = temp;

	//File operation
	hOptionFile_Write = CreateFile(cOptionFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
		FileSizeLow = GetFileSize(hOptionFile_Write,&FileSizeHigh);
	
	WriteFile(hOptionFile_Write,cOptionPacket,iFileSize,&lWritten,NULL);
	CloseHandle(hOptionFile_Write);

	delete [] cOptionFileName;
	delete [] cOptionPacket;

	pExeFunc.pFillList = FillList;
	pExeFunc.pSetDialogText = SetDialogTextW;
	pExeFunc.pSetTipText = SetTipText;
	pExeFunc.pClearTipText = ClearTipTextArray;
	pExeFunc.pAppendTipText = AppendTipText;
	pExeFunc.pCleanHistory = CleanHistory;

	SetParameter(sOP,pExeFunc);

	return TRUE;
}









int ReadOptionFile(OptionFile * sOP)
{
	HANDLE hOptionFile_Read;
	cOptionFileName = (wchar_t*) new wchar_t[1024];
	memset(cOptionFileName,0,1024 * sizeof(wchar_t));
// 	GetModuleFileName(hInst,cOptionFileName,1024);
// 	PathRemoveFileSpec(cOptionFileName);
	SHGetSpecialFolderPath(NULL, cOptionFileName, CSIDL_PERSONAL, FALSE);
	wcscat(cOptionFileName,L"\\HideDragon");
	CreateDirectory(cOptionFileName,NULL);

	wcscat(cOptionFileName,L"\\misc34.hdo");

	hOptionFile_Read = CreateFile(cOptionFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if(hOptionFile_Read == INVALID_HANDLE_VALUE)
	{
		delete [] cOptionFileName;
		return 0;
	}
	FileSizeLow = GetFileSize(hOptionFile_Read,&FileSizeHigh);
	
	char * cOptionPacket = new char[FileSizeLow];
	memset(cOptionPacket,0,FileSizeLow);
	
	ReadFile(hOptionFile_Read,cOptionPacket,FileSizeLow,&lWritten,NULL);

	char * temp = cOptionPacket;

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
	memcpy(sOP->cPassword,cOptionPacket,15 * sizeof(wchar_t));
	cOptionPacket += 15 * sizeof(wchar_t);
	memcpy(sOP->cTrailRun,cOptionPacket,MAX_PATH * sizeof(wchar_t));
	cOptionPacket += MAX_PATH * sizeof(wchar_t);
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
	sOP->bHideIcon = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bReserveSel = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bNeedStart = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bAutoHide_QH = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bReserveHabit = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bShowTip = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bShowBKTip = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->iAutoUpdate = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bRightFirst = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bUseMiddle = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bUseResHK = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->sResHK.uMF = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->sResHK.uVK = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->sResHK.wHK_Res = *(WORD*)cOptionPacket;
	cOptionPacket += sizeof(WORD);
	sOP->sHaHK.uMF = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->sHaHK.uVK = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->sHaHK.wHK_Res = *(WORD*)cOptionPacket;
	cOptionPacket += sizeof(WORD);
	sOP->sSsHK.uMF = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->sSsHK.uVK = *(UINT*)cOptionPacket;
	cOptionPacket += sizeof(UINT);
	sOP->sSsHK.wHK_Res = *(WORD*)cOptionPacket;
	cOptionPacket += sizeof(WORD);
	sOP->bHideAllTray = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bCloseInfoPanel = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bAutoChangeIconTitle = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bBeginnerModel = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bHideProcess = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bNotShowProcess = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bCleanAfterHide = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->bAutoHideProgram = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);
	sOP->doubleClick = *(int*)cOptionPacket;
	cOptionPacket += sizeof(int);


	



	cOptionPacket = temp;


	CloseHandle(hOptionFile_Read);

	pExeFunc.pFillList = FillList;
	pExeFunc.pSetDialogText = SetDialogTextW;
	pExeFunc.pSetTipText = SetTipText;
	pExeFunc.pClearTipText = ClearTipTextArray;
	pExeFunc.pAppendTipText = AppendTipText;



	SetParameter(*sOP,pExeFunc);

	delete [] cOptionPacket;
	delete [] cOptionFileName;

	return TRUE;

}

DWORD WINAPI WriteRegThread(LPVOID lp)
{
	wchar_t * cCurrentPath = NULL;

	
	if(sOP.bAutoRun == 1)
	{
		dwPathSize = 1024;
		cCurrentPath = (wchar_t*)new wchar_t[dwPathSize];
		memset(cCurrentPath,0,1024 * sizeof(wchar_t));
		GetModuleFileNameW(NULL,cCurrentPath,dwPathSize);

		wchar_t * cRegKey	= (wchar_t*)VirtualAlloc(NULL,2048 * sizeof(wchar_t),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		memset(cRegKey,0,2048 * sizeof(wchar_t));
		wcscpy(cRegKey,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		RegOpenKeyW(HKEY_LOCAL_MACHINE,cRegKey,&hRegAutoRun);

		wcscat(cCurrentPath,L" -A");

		RegSetValueExW(hRegAutoRun,L"HideDragon",NULL,REG_SZ,(const BYTE*)cCurrentPath,
			/*1024 * sizeof(wchar_t)*/wcslen(cCurrentPath) * sizeof(wchar_t) + 2);
		RegCloseKey(hRegAutoRun);
		VirtualFree(cRegKey,0,MEM_RELEASE);
		delete [] cCurrentPath;
	}
	
	//Quick launch area///////////////////////////////////////////////////////////////////
	CoInitialize(0);
	LPMALLOC      pShellMalloc;
	SHGetMalloc(&pShellMalloc);
	
	SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&lpItemIdList);
	wchar_t * cPath_QuickLaunch = (wchar_t*)new wchar_t[1024];
	memset(cPath_QuickLaunch,0,1024 * sizeof(wchar_t));
	SHGetPathFromIDListW(lpItemIdList,cPath_QuickLaunch);
	pShellMalloc->Free(lpItemIdList);
	pShellMalloc->Release();


	if(CreateFileW(cPath_QuickLaunch,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL) == NULL)
	{
		//MessageBox(hwndDlg,"path not exist","隐身专家",MB_OK);
	}
	CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,
		(void**)&pShellLink);
	pShellLink->QueryInterface(IID_IPersistFile,(void**)&ppf);	
	cCurrentPath = new wchar_t [1024];
	memset(cCurrentPath,0,1024 * sizeof(wchar_t));
	GetModuleFileNameW(NULL,cCurrentPath,1024);
	
	if (pShellLink->SetPath(cCurrentPath) == NOERROR)
	{
		//MessageBox(hwndDlg,"OK","OK",MB_OK);
	}
	wchar_t * cWorkingDic = (wchar_t *)new wchar_t [1024];
	memset(cWorkingDic,0,1024 * sizeof(wchar_t));
	wcscpy(cWorkingDic,cCurrentPath);
	PathRemoveFileSpecW(cWorkingDic);
	if(pShellLink->SetWorkingDirectory(cWorkingDic)==NOERROR)
	{
		//MessageBox(NULL,"SETOK","OK",MB_OK);
	}
	SetCurrentDirectory(cWorkingDic);
	delete [] cCurrentPath;
	delete [] cWorkingDic;
	wchar_t * cShortcutName = (wchar_t*)new wchar_t[1024];
	memset(cShortcutName,0,dwPathSize * sizeof(wchar_t));
	wcscpy(cShortcutName,cPath_QuickLaunch);
	wcscat(cShortcutName,L"\\Microsoft\\Internet Explorer\\Quick Launch\\");
	if(g_iUseLanguage == 0)
	{
		wcscat(cShortcutName,L"HideDragon 3.4");//隐身专家 2.9");
	}
	else
	{
		wchar_t * pText = new wchar_t[1024];
		wcscat(cShortcutName,L"HideDragon 3.4"/*LoadItemStringW(L"IDS_TRAYICON_BALLTIPTITLE",pText,
			L"HideDragon 3.21")*/);
		delete [] pText;
	}
	wcscat(cShortcutName,L".lnk");
	if(sOP.bAddQuickLaunch == 1)
	{
		hr = ppf->Save( (unsigned short *)cShortcutName,FALSE);
	}
	delete [] cShortcutName;
	delete [] cPath_QuickLaunch;
	CoUninitialize();

















	return 1;
}
DWORD WINAPI NetworkThread(LPVOID lp)
{
	if(lp != NULL)
	{
		char * cUpdateFileName = new char[1024];
		memset(cUpdateFileName,0,1024 * sizeof(char));
		strcpy(cUpdateFileName,(char*)lp);
		GetFileFromServer(cUpdateFileName);
		delete [] cUpdateFileName;
	}
	else
	{
		int iRet = GetFileFromServer(NULL);
	}

	if(hNetworkThread != NULL && hNetworkThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hNetworkThread);
	}
	hNetworkThread = NULL;

//	if(g_hUpdateHandle != NULL && g_hUpdateHandle != INVALID_HANDLE_VALUE)
//	{
//		CloseHandle(g_hUpdateHandle);
//		g_hUpdateHandle = NULL;
//	}
	
	return 1;

}

int GetFileFromServer(char * cFileName)
{
	wchar_t *pVerValue;
	wchar_t SubBlock[256] ;
	long lResponseHeader = 0,lFileSize = 0,lDownloadSize = 0;
	int iRecvSize = 0,iError = 0,iVerLen = 0,iRecvCount = 0;
	HANDLE hVerInfoFile;
	DWORD dwWritten = 0,dwVerInfoSize = 0,dwError = 0;
	float fCurrentVer = 0;
	URL_COMPONENTSA url_c;

	//Copyed from WinMain
	cRequestHeader = (char *)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cResponseHeader = (char *)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

	cObject = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cHostName = (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cValue =  (char*)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	cRecvBuff = (char *)VirtualAlloc(NULL,1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	pVerBuff = (wchar_t*)VirtualAlloc(NULL,3*1024,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	
	

	WSAStartup(MAKEWORD(2,2),&wsd);
  
	try
	{
		sock_Scan = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		
		if(HD_Connect(sock_Scan,"www.hidedragon.com",80)==0) //"127.0.0.1" FIXME
		{
			throw 0;
		}
		memset(cObject,0,1024);
		memset(cHostName,0,1024);
		lHeaderLength = 0;
		if(cFileName == NULL)
		{
			strcpy(cObject,"en/VersionInfo_en.hdv");// /homepage/VersionInfo28.hdv FIXME
		}
		else
		{
			//strcpy(cObject,"/homepage");//FIXME NO THIS LINE
			//strcat(cObject,cFileName);//FIXME strcpy
			strcpy(cObject,cFileName);
		}
		strcpy(cHostName,"www.hidedragon.com");// 127.0.0.1 FIXME

		
		HD_FormatRequestHeader(cObject,cHostName,&lHeaderLength,cRequestHeader);
		if(HD_SendRequest(sock_Scan,cRequestHeader)==0)
		{
			throw 1;
		}

		memset(cResponseHeader,0,1024);
		lHeaderLength = 0;
		if(HD_RecvResponseHeader(sock_Scan,cResponseHeader,&lHeaderLength)==0)
		{
			throw 1;
		}
		if (HD_GetState(cResponseHeader) == 0)
		{
			throw 1;
		}
		lFileSize = 0;
		HD_GetHeaderField(cResponseHeader,"Content-Length",cValue);
		lFileSize = atol(cValue);


		wchar_t * cLocalFileName = new wchar_t[1024];
		memset(cLocalFileName,0,1024 * sizeof(wchar_t));
// 		GetCurrentDirectory_HD(1024,cLocalFileName);
		SHGetSpecialFolderPath(NULL, cLocalFileName, CSIDL_PERSONAL, FALSE);
		wcscat(cLocalFileName,L"\\HideDragon");
		CreateDirectory(cLocalFileName,NULL);



		if(cFileName == NULL)
		{
			wcscpy(cLocalFileName,L"versioninfo_en.hdv");
		}
		else
		{
			wcscat(cLocalFileName,L"\\Downloads\\");
			CreateDirectory(cLocalFileName,NULL);
			wchar_t * pFileNameW = new wchar_t[1024];
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,cFileName,1024,pFileNameW,1024*sizeof(wchar_t));
			//wcscat(cLocalFileName,(wcsrchr(cFileName,L'/')+1));
			wcscat(cLocalFileName,(wcsrchr(pFileNameW,L'/')+1));
			delete [] pFileNameW;
		}
		

		hVerInfoFile = CreateFile(cLocalFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		delete [] cLocalFileName;


		lDownloadSize = 0;
		iRecvSize = 0;
		
		while(lDownloadSize < lFileSize)
		{
			iRecvSize = recv(sock_Scan,cRecvBuff,1024,0);
			if (iRecvSize == SOCKET_ERROR)
			{
				iError = WSAGetLastError();
				throw 2;
			}
			else if (iRecvSize == 0)
			{
				throw 2;
			}
			iRecvCount = 0;
			WriteFile(hVerInfoFile,cRecvBuff,iRecvSize,&dwWritten,NULL);

			lDownloadSize += iRecvSize;
			memset(cRecvBuff,0,1024 * sizeof(wchar_t));
		}
		CloseHandle(hVerInfoFile);

		if(cFileName != NULL)
		{
			shutdown(sock_Scan,0);
			closesocket(sock_Scan);
			HD_GarbageRecycle();
			return 1;
		}
		
		if (ReadVersionInfoFile(&viRF) == 0)
		{
			throw 1;
		}
		dwVerInfoSize = GetFileVersionInfoSize((LPTSTR)L"HideDragon.exe",0);
		if(dwVerInfoSize == 0)
		{
			throw 1;
		}
		struct LANGANDCODEPAGE {
		  WORD wLanguage;
		  WORD wCodePage;
		} *lpTranslate;


		GetFileVersionInfo(L"HideDragon.exe",0,dwVerInfoSize,pVerBuff);
		VerQueryValue(pVerBuff,L"\\VarFileInfo\\Translation",(void **)&lpTranslate,(unsigned int*)&iVerLen);

		memset(SubBlock,0,256 * sizeof(wchar_t));
		swprintf( SubBlock, 
					TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
					lpTranslate[0].wLanguage,
					lpTranslate[0].wCodePage);

		VerQueryValue(pVerBuff,SubBlock,(void **)&pVerValue,(unsigned int*)&iVerLen);
		char * mbValue = new char[1024];
		memset(mbValue,0,1024);
		WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,pVerValue,1024,mbValue,1024,NULL,FALSE);
		fCurrentVer = (float)atof(mbValue);
		delete [] mbValue;
		if(fCurrentVer<viRF.fVersionNum)
		{
			shutdown(sock_Scan,0);
			closesocket(sock_Scan);
			HD_GarbageRecycle();
			g_hUpdateHandle = (HANDLE)_beginthreadex(NULL,0,AutoUpdateThreadProc,NULL,0,NULL);
			if(hNetworkThread != NULL && hNetworkThread != INVALID_HANDLE_VALUE)
			{
				CloseHandle(hNetworkThread);
				hNetworkThread = NULL;
				OutputDebugString(L"hNetworkThread END.");
			}
			return 1;
		}
		shutdown(sock_Scan,0);
		closesocket(sock_Scan);

		for(int i = 0;i<viRF.iSiteNum;i++)
		{
			sock_Scan = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			memset(cObject,0,1024 * sizeof(wchar_t));
			memset(cHostName,0,1024 * sizeof(wchar_t));

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


			if(InternetCrackUrlA(viRF.sUI[i].cURLs,viRF.sUI[i].lSize,NULL,&url_c) != TRUE)
			{
				throw 0;
			}
			if (strlen(cObject) == 0)
			{
				strcpy(cObject,"/");
			}
			if (HD_Connect(sock_Scan,cHostName,80) == 0)
			{
				throw 1;
			}
			memset(cRequestHeader,0,1024 * sizeof(wchar_t));
			HD_FormatRequestHeader(cObject,cHostName,&lHeaderLength,cRequestHeader);
			if (HD_SendRequest(sock_Scan,cRequestHeader)==0)
			{
				throw 1;
			}
			if(HD_RecvResponseHeader(sock_Scan,cResponseHeader,&lHeaderLength) == 0)
			{
				throw 1;
			}
			memset(cRecvBuff,0,1024 * sizeof(wchar_t));
			recv(sock_Scan,cRecvBuff,1024,0);
			shutdown(sock_Scan,0);
			closesocket(sock_Scan);
		}
		if(hNetworkThread != NULL && hNetworkThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hNetworkThread);
			hNetworkThread = NULL;
		}
		HD_GarbageRecycle();
	}
	catch(int iThrow)
	{
		if(iThrow == 0)
		{
			closesocket(sock_Scan);
			HD_GarbageRecycle();
			return 0;
		}
		else if(iThrow == 1)
		{
			shutdown(sock_Scan,0);
			closesocket(sock_Scan);
			HD_GarbageRecycle();
			return 1;
		}
		if(iThrow == 2)
		{
			CloseHandle(hVerInfoFile);
			shutdown(sock_Scan,0);
			closesocket(sock_Scan);
			HD_GarbageRecycle();
		}
	}
	catch(...)
	{
		shutdown(sock_Scan,0);
		closesocket(sock_Scan);
		HD_GarbageRecycle();
		return 0;
	}

	
	
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
	//ServerAddr.sin_addr.s_addr = inet_addr("219.232.224.70");

	if (connect(sock_Scan,(sockaddr*)&ServerAddr,sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		//MessageBox(hMainDialog,"connect error","hd",MB_OK);
		DWORD dwErr = GetLastError();
		return 0;
	}

	return 1;
}

int HD_FormatRequestHeader(char * cObject,char * cHostName,long * lHeaderLength,char * cRequestHeader)
{
	memset(cRequestHeader,'\0',1024 * sizeof(char));
	

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
	/*DWORD dwSent = 0;
	WSABUF wb;
	char * cMulti = new char[2048];
	memset(cMulti,0,2048);
	WideCharToMultiByte(CP_ACP,WC_NO_BEST_FIT_CHARS,cRequestHeader,1024,cMulti,2048,NULL,FALSE);
	wb.buf = cMulti;
	wb.len = 2048;*/
	iError = send(sock_Scan,cRequestHeader,1024,NULL);
	//iError = WSASend(sock_Scan,&wb,1,&dwSent,0,NULL,NULL);
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
	char  b = ' ';
	DWORD dwRecv = 0;

	memset(cResponseHeader,0,1024);
	while(!bEndRecv&&i < 1024)
	{
//		wb.buf = b;
//		wb.len = sizeof(char);
		lRecvSize = recv(sock_Scan,&b,1,NULL);
//		lRecvSize = WSARecv(sock_Scan,&wb,1,&dwRecv,0,NULL,NULL);
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
 

int HD_GetHeaderField(char * cResponseHeader,char * cField,char * cValue)
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
	wchar_t cTemp[80],cTemp2[80],*cTemp3 = NULL;
	wchar_t cCode1[10],cCode3[10];
	long lRegCode = 0,lOrgCode = 0;
	static HWND hStatic_Limit = NULL,hStatic_FeedBack = NULL,hEmail = NULL,hHttp = NULL;
	HWND hCancelReg;
	HBRUSH bmenu = NULL,wbr = NULL;
	static int iForLink = 1,iForIn = 0,iForOut = 0;
	HDC hScreenDC;
	int iX = 0,iY = 0;
	static HWND hStatic_EmailCode = NULL;
	static HFONT hFont_Link;
	LOGFONT lf;


	TOOLINFO ti;
	LPTOOLTIPTEXT lpttt;
	int idCtrl;
	wchar_t * pTipText = NULL, * pTipText2 = NULL;



	switch(uMsg)
	{
	case WM_INITDIALOG:
		lf.lfHeight = 15;
		lf.lfWidth = 0;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfWeight = 500;
		lf.lfItalic = FALSE;
		lf.lfUnderline = TRUE;
		lf.lfStrikeOut = FALSE;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH;
		wcscpy(lf.lfFaceName,L"MS Sans Serif");
		hFont_Link = CreateFontIndirect(&lf);
		if(!IsWindowVisible(hMainDialog))
		{
			hScreenDC = GetDC(NULL);
			iX = GetDeviceCaps(hScreenDC,HORZRES)/4;
			iY = GetDeviceCaps(hScreenDC,VERTRES)/8;
			SetWindowPos(hwndDlg,NULL,iX,iY,0,0,SWP_NOSIZE|SWP_NOZORDER);
			ReleaseDC(NULL,hScreenDC);
		}

		hStatic_Limit = GetDlgItem(hwndDlg,IDC_STATIC_LIMIT);
		SendMessage(hStatic_Limit,WM_SETFONT,(WPARAM)hFont_New,0);

		hStatic_FeedBack = GetDlgItem(hwndDlg,IDC_STATIC_FEEDBACK);
		SendMessage(hStatic_FeedBack,WM_SETFONT,(WPARAM)hFont_New,0);

		hStatic_EmailCode = GetDlgItem(hwndDlg,IDC_STATIC_ORGCODE);
//		SendMessage(hStatic_EmailCode,WM_SETFONT,(WPARAM)hFont_New,0);



		hEmail = ::GetDlgItem(hwndDlg,IDC_STATIC_SUPPORTMAIL);
		hHttp = ::GetDlgItem(hwndDlg,IDC_STATIC_BUYLINK);
		SetClassLong(hEmail,GCL_HCURSOR,(long)NULL );
		SetClassLong(hHttp,GCL_HCURSOR,(long)NULL);
		SendMessage(hHttp,WM_SETFONT,(WPARAM)hFont_Link,TRUE);
		SendMessage(hEmail,WM_SETFONT,(WPARAM)hFont_Link,TRUE);

		hHttpDC = GetDC(hHttp);
		hEmailDC = GetDC(hEmail);


		hEdit_MiddleCode = ::GetDlgItem(hwndDlg,IDC_EDIT_MIDDLECODE);
		hRegDialog = hwndDlg;
		for (i = 0;i<5;i++)
		{
			hEdit_RC[i] = ::GetDlgItem(hwndDlg,1050+i);
		}
		SendMessage(hEdit_MiddleCode,EM_LIMITTEXT,(WPARAM)19,0);
		SendMessage(hEdit_RC[0],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[0],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[1],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[2],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[3],EM_LIMITTEXT,(WPARAM)4,0);
		SendMessage(hEdit_RC[4],EM_LIMITTEXT,(WPARAM)12,0);
//		SendMessage((HWND)hEdit_MiddleCode,WM_SETTEXT,0,(LPARAM)sOP.cSerialNumber);
		memset(sOP.cSerialNumber,0,100 * sizeof(wchar_t));
		for(i = 0;i<4;i++)
		{
			wcscat(sOP.cSerialNumber,cCode[i]);
			if (i<=2)
			{
				wcscat(sOP.cSerialNumber,L"-");
			}
		}
//		SendMessage((HWND)hEdit_MiddleCode,WM_SETTEXT,0,(LPARAM)sOP.cSerialNumber);
		//::SetWindowText(hEdit_MiddleCode,cOrgCode);
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOG_REGISTER);			
		}
		hCancelReg = GetDlgItem(hwndDlg,IDC_CANCELREG);
#ifndef _NOADS
		if(0)//sOP.iUsedTimes > iTryTime)
#endif


#ifdef _NOADS
		if(sOP.iUsedTimes > iTryTime)
#endif
		{
			wchar_t * pText = new wchar_t[1024];
			memset(pText,0,1024*sizeof(wchar_t));
			SetWindowText(hCancelReg,LoadItemStringW(L"IDM_EXIT",pText,L"&Exit"));
			SendMessage(hCancelReg,WM_SETFONT,(WPARAM)hFont_New,0);
			delete [] pText;
		}
		
//		SendMessage(hStatic_EmailCode,WM_SETFONT,(WPARAM)hFont_New,0);
		g_bmenu = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		g_wbr = CreateSolidBrush(RGB(255,255,255));

		

		hToolTip_reg = CreateWindowEx(NULL,TOOLTIPS_CLASS,NULL,WS_POPUP|TTS_ALWAYSTIP,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,hwndDlg,NULL,hInst,NULL);
		ti.cbSize     = sizeof (TOOLINFO);
		ti.hinst      = hInst;
		ti.uFlags     = TTF_IDISHWND;
		ti.hwnd       = hwndDlg;
		ti.uId        = (UINT_PTR)hHttp;
		ti.lpszText   = /*"Welcome !";*/LPSTR_TEXTCALLBACK;
		SendMessage(hToolTip_reg,TTM_ADDTOOL,0,(LPARAM)&ti);
		ti.uId        = (UINT_PTR)hEmail;
		SendMessage(hToolTip_reg,TTM_ADDTOOL,0,(LPARAM)&ti);


		
		
		return 0;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hEdit_MiddleCode)
		{
			return (LRESULT)g_wbr;
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
			
			return (LRESULT)g_bmenu;
		}

		SetBkColor((HDC)(wParam),GetSysColor(COLOR_3DFACE));
		return (LRESULT)g_bmenu;
	case WM_NOTIFY:
		switch(((LPNMHDR)(lParam))->code)
		{
		case  TTN_GETDISPINFO:
			idCtrl = ::GetDlgCtrlID((HWND)((LPNMHDR)lParam)->idFrom);
			lpttt  = (LPTOOLTIPTEXTW)lParam;
			switch(idCtrl)
			{
			case IDC_STATIC_SUPPORTMAIL:
				if(g_iUseLanguage == 1)
				{
					pTipText2 = new wchar_t[1024];
					wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_REGISTEREMAIL",pTipText2));
					delete [] pTipText2;
				}
				else
				{
					lpttt ->lpszText = L"Please feel free to contact us for any question.";
				}
				
				break;
			case IDC_STATIC_BUYLINK:
// 				if(g_iUseLanguage == 1)
// 				{
// 					pTipText = new wchar_t[1024];
// 					wcscpy(lpttt->lpszText,LoadItemStringW(L"IDS_TIP_REGISTERBUYURL",pTipText));
// 					delete [] pTipText;
// 				}
// 				else
// 				{
// 					lpttt ->lpszText = L"License code sent automatically within minutes after your purchase.";
// 				}
				BuyHideDragon();
			default:
				return FALSE;
			}
			break;
		default :
			return 0;
		}
		return 0;		
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
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_STATIC_SUPPORTMAIL:
				if (HIWORD(wParam) == STN_CLICKED)
				{
					ShellExecute(hwndDlg,NULL,L"mailto:hdsupport@gmail.com",NULL,NULL,SW_SHOWMAXIMIZED);
				}
				break;
		case IDC_STATIC_BUYLINK:
				if(HIWORD(wParam) == STN_CLICKED)
				{
// 					wchar_t * pLink = new wchar_t[1024];
// 					ShellExecute(NULL,L"open",L"IEXPLORE",
// 						LoadItemStringW(L"IDM_LINK_BUY",pLink,
// 						L"http://www.hidedragon.com/cn/purchase.html"),NULL,SW_SHOW);
// 
// 					delete [] pLink;
					BuyHideDragon();
				}
				break;
		case IDCANCEL:
				ReleaseDC(hHttp,hHttpDC);
				ReleaseDC(hEmail,hEmailDC);
				DeleteObject((HGDIOBJ)hFont_Link);
				break;
		case IDC_COMPLETEREG:
			if(GetWindowTextLength(hEdit_MiddleCode) < 3)
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_USERNAMETOSHORT",L"User name at least 3 character.",hwndDlg);
				return 0;
			}
			memset(cTemp,0,80 * sizeof(wchar_t));
			memset(cTemp2,0,80 * sizeof(wchar_t));
			memset(sOP.cSerialNumber,0,100 * sizeof(wchar_t));
			//MultiByteToWideChar(CP_ACP,MB_COMPOSITE,sOP.cSerialNumber,-1,(unsigned short*)cTemp,80);
			wcscpy(cTemp,sOP.cSerialNumber);
			wcscpy(cTemp2,L"{");
			cTemp2[1] = 0;
			cTemp3 = cTemp2;
			cTemp3 += 2;
			
			memcpy(cTemp3,cTemp,75);
			cTemp2[74] = '}';
			CLSIDFromString((unsigned short*)cTemp2,(LPCLSID)&hd_guid);

//			memset(cTemp2,0,80 * sizeof(wchar_t));
			memset(cTemp,0,80 * sizeof(wchar_t));
			wcscpy(cTemp,L"{");
			for (i = 0;i<5;i++) 
			{
				GetWindowText(hEdit_RC[i],cTemp2,16);
				if(i == 1)
				{
					memset(cCode1,0,10*sizeof(wchar_t));
					wcscpy(cCode1,cTemp2);
				}
				if(i == 2)
				{
					memset(cCode3,0,10*sizeof(wchar_t));
					wcscpy(cCode3,cTemp2);
				}

				wcscat(cTemp,cTemp2);
				if (i < 4)
				{
					wcscat(cTemp,L"-"); 
				}
				memset(cTemp2,0,80 * sizeof(wchar_t));
			}
			wcscat(cTemp,L"}");
			memset(cTemp2,0,80 * sizeof(wchar_t));
			//MultiByteToWideChar(CP_ACP,MB_COMPOSITE,cTemp,-1,(unsigned short*)cTemp2,80);
			wcscpy(cTemp2,cTemp);
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
			memset(cTemp2,0,80 * sizeof(wchar_t));
			for (i =0;i<4;i++)
			{
				GetWindowText(hEdit_RC[i],cTemp,5);
				lRegCode = wcstol(cTemp,NULL,16);
				lOrgCode = wcstol(cCode[i],NULL,16);
				lOrgCode ^= 3333;
				lOrgCode |= 3333;
				if (lRegCode != lOrgCode)
				{
					break;
				}
			}
			
			if(wcscmp(cCode1,L"A90E") == 0 && wcscmp(cCode3,L"E9FA") == 0/*i>=4*/)
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_REGISTERSUCCESSFUL",
					L"Register successful!You can acquire one year update and technology support.",hwndDlg);
				wcscpy(sOP.cMiddleCode,cTemp);
				sOP.bRegistered = 1;
				WriteOptionFile(sOP);
				WriteRegCode();
				wchar_t cUserName[30] = L"";
				GetDlgItemText(hwndDlg,IDC_EDIT_MIDDLECODE,cUserName,30);

				SaveUserName(cUserName);
				hRegDialog = NULL;
				EndDialog(hwndDlg,1);
			}
			else
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_SERIALERROR",L"Register code is error,please check and re input.",hwndDlg);
			}
			break;
		case IDC_CANCELREG:
			DeleteObject((HGDIOBJ)g_bmenu);
			DeleteObject((HGDIOBJ)g_wbr);
			ReleaseDC(hHttp,hHttpDC);
			ReleaseDC(hEmail,hEmailDC);
			hRegDialog = NULL;
			DeleteObject((HGDIOBJ)hFont_Link);
			EndDialog(hwndDlg,0);
			return FALSE;
		case IDC_HOWREG:
			ShellExecute(NULL,L"open",L"http://www.hidedragon.com/cn/purchase.html",NULL,NULL,SW_SHOW);
			break;
		}
	}
	return FALSE;
}




int SaveUserName(wchar_t * cUserName)
{
	DBClass db;

 	db.SaveUserName(cUserName);




	return 1;
}


VOID CALLBACK  ScanWebProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	OutputDebugString(L"ScanWebProc Enter.");
	if (hNetworkThread == NULL && g_hUpdateHandle == NULL && g_hUpdateLangHandle == NULL)
	{
		OutputDebugString(L"hNetworkThread BEGIN.");
		hNetworkThread = (HANDLE)_beginthreadex(NULL,0,(unsigned int(__stdcall*)(void*))NetworkThread,NULL,0,NULL);
	}
}


int GarbageRecycle()
{
	RestoreVolume();
	for(int i = 0;i<100;i++)
	{
		VirtualFree(sW[i].cWndTitle,0,MEM_RELEASE);

		delete [] sW[i].cIconTip;
	}
	VirtualFree(sW,0,MEM_RELEASE);
//	VirtualFree(cRequestHeader,0,MEM_RELEASE);
//	VirtualFree(cResponseHeader,0,MEM_RELEASE);
//	VirtualFree(cOptionFileName,0,MEM_RELEASE);
	delete [] sOP.cSerialNumber;
	delete [] sOP.cMiddleCode ;
	delete [] sOP.cTrailRun;
	DestroyIcon(hI2);
	DestroyIcon(hI1);
	DestroyMenu(hMainMenu);
	DestroyMenu(hWLMenu1);
	DestroyWindow(hMainDialog);
	SetLastError (ERROR_SUCCESS);
	GlobalDeleteAtom(aShowSelf); 
	if(GetLastError() != ERROR_SUCCESS)
	{
		OutputDebugString(L"Delete Atom Error.");
	}
	SetLastError(ERROR_SUCCESS);
	SetLastError(ERROR_SUCCESS);
	GlobalDeleteAtom(aResHK);
	if(GetLastError() != ERROR_SUCCESS)
	{
		OutputDebugString(L"Delete Atom Error.");
	}
	DeleteCriticalSection(&csDelListView);
	DeleteCriticalSection(&csFillList);
	DeleteObject(hFont_New);


	//Delete Button resource
	DeleteObject(hBKBrush);

	ClearTipTextArray();
	for(i = 0;i < 3;i ++)
	{
		DestroyWindow(g_TabWnd[i]);
	}
	DestroyWindow(g_hInfoPanel);

	UnhookWindowsHookEx(hGM);

	TerminateThread(g_AutoHideHandle,0);

	return 1;
}

int HD_GarbageRecycle()
{
	VirtualFree(cObject,0,MEM_RELEASE);
	VirtualFree(cHostName,0,MEM_RELEASE);
	VirtualFree(cValue,0,MEM_RELEASE);
	VirtualFree(cRecvBuff,0,MEM_RELEASE);
	VirtualFree(pVerBuff,0,MEM_RELEASE);
	VirtualFree(cRequestHeader,0,MEM_RELEASE);
	VirtualFree(cResponseHeader,0,MEM_RELEASE);
	for(int i = 0;i < viRF.iSiteNum; i ++)
	{
		VirtualFree(viRF.sUI[i].cURLs,0,MEM_RELEASE);
	}
	if(viRF.iSiteNum > 0)
	{
		VirtualFree(viRF.sUI,0,MEM_RELEASE);
	}
	viRF.iSiteNum = 0;
	viRF.fVersionNum = 0;
	viRF.sUI = NULL;

	WSACleanup();

	return 1;
}


INT_PTR CALLBACK PasswordProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	wchar_t cPass[11],cVali[11];
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
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOGPASSWORD);
		}
		return FALSE;  
	case WM_NOTIFY:
		switch((LPNMHDR(lParam))->code )
		{
		case PSN_KILLACTIVE:
			memset(cPass,0,11 * sizeof(wchar_t));
			memset(cVali,0,11 * sizeof(wchar_t));
			GetWindowText(hEdit_Password,cPass,10);
			GetWindowText(hEdit_Validate,cVali,10);

			if(IsWindowEnabled(hEdit_Password) && ((wcscmp(cPass,L"")==0)||(wcscmp(cVali,L"")==0)))
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_IFENABLEPASSWORD",
					L"If enable password protect,must input password.",hwndDlg);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;
			}

			if(wcscmp(cPass,cVali))
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_PASSWORDNOTMATCH",
					L"Password is not matching.",hwndDlg);
				SetWindowText(hEdit_Password,L"");
				SetWindowText(hEdit_Validate,L"");
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
				memset(cPass,0,11 * sizeof(wchar_t));
				GetWindowText(hEdit_Password,cPass,11);
				wcscpy(sOP.cPassword,cPass);
				sOP.iKorS = 'K';
				WriteOptionFile(sOP);
			}
			else if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_USEPASSWORD) == BST_UNCHECKED)
			{
				sOP.bUsePassword = 0;
				memset(sOP.cPassword,0,11 * sizeof(wchar_t));
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
				SetWindowText(hEdit_Password,L"");
				SetWindowText(hEdit_Validate,L"");
				
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
//	HWND hWnd = FindWindow(_T("Shell_TrayWnd"), NULL);
//	if(hWnd)
//	{
//		hWnd = FindWindowEx(hWnd,NULL,_T("TrayNotifyWnd"), NULL);
//		if(hWnd)
//		{
//			hWnd = FindWindowEx(hWnd,NULL,_T("SysPager"), NULL);
//			//hWnd = FindWindowEx(hWnd,NULL,_T("ReBarWindow32"),NULL);
//			if(hWnd)
//			{				
//				hWnd = FindWindowEx(hWnd, NULL,_T("ToolbarWindow32"), NULL);
//			}
//		}
//	}




    HWND hTrayWnd = ::FindWindow(L"Shell_TrayWnd", NULL);
    hTrayWnd = ::FindWindowEx(hTrayWnd, 0, L"TrayNotifyWnd", NULL);
 	HWND hwndTmp;	
    hwndTmp = ::FindWindowEx(hTrayWnd, 0, L"SysPager", NULL);
	if(!hwndTmp)
		hTrayWnd=::FindWindowEx(hTrayWnd,0,L"ToolbarWindow32",NULL);
	else
		hTrayWnd=::FindWindowEx(hwndTmp,0,L"ToolbarWindow32",NULL);

	return hTrayWnd;
}


int IsHaveTrayIcon(WndAndHandle * pWH,TRAYDATA * pTD)
{
	DWORD lPID;

	GetWindowThreadProcessId(hTrayWnd,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t Tipwchar_t;
	wchar_t sTip[1024] = {0};
	memset(sTip,0,1024 * sizeof(wchar_t));
	wchar_t* pTip = NULL;
	int x = 0,iRet = 0;


	int iBtnCount = SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);
	for(i=0; i < iBtnCount;i++)
	{
		SendMessage(hTrayWnd,TB_GETBUTTON,i,(LPARAM)data.GetData());
		data.ReadData(&tb);
		data.ReadData(&tray,(void * )tb.dwData);

		if(IsWindow(tray.hwnd) == FALSE)//Delete invalid tray icon
		{
			SendMessage(hTrayWnd,TB_DELETEBUTTON,i,0);
			SendMessage(hMainDialog,WM_COMMAND,IDC_HIDEICON,NULL);
			SendMessage(hMainDialog,WM_COMMAND,IDC_HIDEICON,NULL);
		}



		if (i < 60)
		{
			arrTray[i] = tray;
		}
		else
		{
			OutputDebugString(L"arrTray array is not enough");
			break;
		}
	}
	
	if(iBtnCount >= 60)
	{
		iBtnCount = 60;
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
				memset(sTip,0,1024 * sizeof(wchar_t));
				x = 0;
				pTip = (wchar_t*)tb.iString;
				pWH->iCommandId = tb.idCommand;
				do 
				{	
					if(x >= 1023)
					{
						wcscpy(sTip,L"[ToolTip was either too long or not set]");
						break;
					}
					
					data.ReadData_Tip((wchar_t*)&Tipwchar_t, (void *)pTip++);
				}while(sTip[x++] = Tipwchar_t);
				
			}
			else
				wcscpy(sTip,L"[Hidden Icon]");
			if(pWH->cIconTip != NULL)
			{
				//wcscpy((wchar_t *)cTip,(LPTSTR)(sTip));
				iRet = 0;
//				iRet = WidewCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
//					(unsigned short*)sTip,-1,pWH->cIconTip,1022,NULL,NULL);
				memset(pWH->cIconTip,0,1024 * sizeof(wchar_t));
				wcscpy(pWH->cIconTip,sTip);
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
	PROPSHEETPAGEW psp = {0};
	HPROPSHEETPAGE ahpsp [3] = {0};
	PROPSHEETHEADERW psh = {0};

	wchar_t * pFirstStep = new wchar_t[1024],* pSelWnd = new wchar_t[1024],*pFinishStep = new wchar_t[1024],
		* pClickFinish = new wchar_t[1024];


	//Welcome page/////////////////////////////////////////////////////////////////////
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_DEFAULT|PSP_HIDEHEADER;
	psp.hInstance = hInst;
	psp.pfnDlgProc = BeginBKProc;
	psp.pszTemplate = MAKEINTRESOURCEW(IDD_WIZARD_BEGINBK);

	ahpsp [0] = CreatePropertySheetPageW(&psp);


	//Select hide window and icon page////////////////////////////////////////////////
	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	if(g_iUseLanguage == 0)
	{
		psp.pszHeaderTitle = L"First Step";
		psp.pszHeaderSubTitle = L"Select the programs or tray icons to be hidden.";
	}
	else
	{
		wcscpy(pFirstStep,LoadItemStringW(L"IDS_WIZARDBKSELWND_FIRSTSTEP",pFirstStep,L"First Step"));
		psp.pszHeaderTitle = pFirstStep;
		
		wcscpy(pSelWnd,LoadItemStringW(L"IDS_WIZARDBKSELWND_SELECTWNDORICON",
			pSelWnd,L"Select the programs or tray icons to be hidden."));
		psp.pszHeaderSubTitle = pSelWnd;
	}

	psp.hInstance = hInst;
	psp.pfnDlgProc = SelWndIconProc;
	psp.pszTemplate = MAKEINTRESOURCEW(IDD_WIZARD_SELWINDOWANDICON);

	ahpsp [1] = CreatePropertySheetPageW(&psp);
	
	//Validate and finish  page//////////////////////////////////////////////
	psp.dwFlags = PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	if(g_iUseLanguage == 0)
	{
		psp.pszHeaderTitle = L"Finish";
		psp.pszHeaderSubTitle = L"Click Finish to hide the selected icons or windows.";
	}
	else
	{
		wcscpy(pFinishStep,LoadItemStringW(L"IDS_WIZARDBKFINSIH_FINISH",pFinishStep,L"Finish"));
		psp.pszHeaderTitle = pFinishStep;
		
		wcscpy(pClickFinish,LoadItemStringW(L"IDS_WIZARDBKFINISH_CLICKFINISHTOSEL",
			pSelWnd,L"Click Finish to hide the selected icons or windows."));
		psp.pszHeaderSubTitle = pClickFinish;
	}
	psp.pszTemplate = MAKEINTRESOURCEW(IDD_WIZARD_FINISH);
	psp.pfnDlgProc = FinishProc;

	ahpsp[2] = CreatePropertySheetPageW(&psp);
	
	psh.dwSize = sizeof(psh);
	psh.hInstance = hInst;
	psh.hwndParent = hMainDialog;
	psh.phpage = ahpsp;
	psh.dwFlags = PSH_WIZARD97|PSH_HEADER|PSH_WATERMARK;
	psh.pszbmWatermark  = MAKEINTRESOURCEW(IDB_WATERMARK);
	psh.pszbmHeader = MAKEINTRESOURCEW(IDB_HEADER);
	psh.nStartPage = 0;
	psh.nPages = 3;

	PropertySheetW(&psh);


	delete [] pFirstStep;
	delete [] pSelWnd;
	delete [] pFinishStep;
	delete [] pClickFinish;

	return 1;
}

INT_PTR CALLBACK SelWndIconProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int i = 0,iItemNum = 0;
	LVITEM wiItem;
	WndAndHandle * pWillDel = NULL;
	NMITEMACTIVATE* pIA = NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		hList_SelWI = GetDlgItem(hwndDlg,IDC_LIST_ICONANDWINDOW);
		hImage_Sel = ImageList_Create(16,16,ILC_COLOR24,30,10);
		ListView_SetImageList(hList_SelWI,hImage_Sel,LVSIL_SMALL);
		AddSelListHeader();
		if(g_iUseLanguage == 1)
		{
			wchar_t * pText = new wchar_t[1024];
			SetWindowTextW(GetParent(hwndDlg),
				LoadItemStringW(L"IDS_WIZARDBEGIN_TITLE",pText,L"The specified background processing wizard"));
			delete [] pText;
			SetDialogTextW(&hwndDlg,IDD_WIZARD_BEGINBK);
		}
		return FALSE;
	case WM_NOTIFY:
		switch((((LPNMHDR)lParam)->code ))
		{
		case NM_DBLCLK:
			pIA = (NMITEMACTIVATE*)lParam;
			if(pIA->hdr.idFrom == IDC_LIST_ICONANDWINDOW)
			{
				if(ListView_GetCheckState(hList_SelWI,pIA->iItem) == FALSE)
				{
					ListView_SetCheckState(hList_SelWI,pIA->iItem,TRUE);
				}
				else
				{
					ListView_SetCheckState(hList_SelWI,pIA->iItem,FALSE);
				}
			}
			break;
		}

		switch(((LPPSHNOTIFY)lParam)->hdr.code)
		{
		case PSN_SETACTIVE:
			ImageList_RemoveAll(hImage_Sel);
			ImageList_AddIcon(hImage_Sel,hIcon_NoIcon);
			PropSheet_SetWizButtons(GetParent(hwndDlg),PSWIZB_BACK|PSWIZB_NEXT);
			UpdateSelList();
			if(g_iUseLanguage == 1)
			{
				SetDialogTextW(&hwndDlg,IDD_WIZARD_SELWINDOWANDICON);
			}
			break;
		case PSN_WIZNEXT:
			iTotalBK = 0;
			iItemNum = ListView_GetItemCount(hList_SelWI);
			i = 0;
			for(i = 0;i<iItemNum;i++)
			{
				if(ListView_GetCheckState(hList_SelWI,i))
				{
					iTotalBK ++;
				}
			}
			if(iTotalBK <= 0)
			{
				HDMessageBoxW(L"IDS_MESSAGEBOX_PLEASESELECTWNDORICON",
					L"Please select the icons or windows to be hidden.",hwndDlg);
				SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
				return TRUE;
			}
			if(iTotalBK)
			{
				sBKWI = (WndAndHandle**) new wchar_t[sizeof(WndAndHandle*)*iTotalBK];
			}
			iTotalBK = 0;
			for(i = 0;i<iItemNum;i++)
			{
				if(ListView_GetCheckState(hList_SelWI,i))
				{
					wiItem.iItem = i;
					wiItem.mask = LVIF_PARAM;
					wiItem.iSubItem = 0;
					ListView_GetItem(hList_SelWI,&wiItem);
					sBKWI[iTotalBK] = (WndAndHandle*)wiItem.lParam;
					iTotalBK ++;
				}
				else
				{
					wiItem.iItem = i;
					wiItem.mask = LVIF_PARAM;
					wiItem.iSubItem = 0;
					ListView_GetItem(hList_SelWI,&wiItem);
					pWillDel = (WndAndHandle*)wiItem.lParam;
					while(pWillDel != NULL)
					{
						if(pWillDel->cWndTitle != NULL)
						{
							VirtualFree(pWillDel->cWndTitle,0,MEM_RELEASE);
							pWillDel->cWndTitle = NULL;
						}
						if(pWillDel->cIconTip != NULL)
						{
							delete [] pWillDel->cIconTip;
							pWillDel->cIconTip = NULL;
						}
						if(wcscmp(pWillDel->cProcessName,L"") == 0)
						{
							delete pWillDel;
							pWillDel = NULL;
						}
						if(pWillDel != NULL)
						{
							pWillDel = pWillDel->Next;
						}
					}
				}
			}
			return FALSE;
		case PSN_QUERYCANCEL:
		case PSN_WIZBACK:
			iItemNum = ListView_GetItemCount(hList_SelWI);
			for(i = 0;i<iItemNum;i++)
			{
				wiItem.iItem = i;
				wiItem.mask = LVIF_PARAM;
				wiItem.iSubItem = 0;
				ListView_GetItem(hList_SelWI,&wiItem);
				pWillDel = (WndAndHandle*)wiItem.lParam;
				while(pWillDel != NULL)
				{
					if(pWillDel->cWndTitle != NULL)
					{
						VirtualFree(pWillDel->cWndTitle,0,MEM_RELEASE);
						pWillDel->cWndTitle = NULL;
					}
					if(pWillDel->cIconTip != NULL)
					{
						delete [] pWillDel->cIconTip;
						pWillDel->cIconTip = NULL;
					}
					if(wcscmp(pWillDel->cProcessName,L"") == 0)
					{
						delete pWillDel;
						pWillDel = NULL;
					}
					if(pWillDel != NULL)
					{
						pWillDel = pWillDel->Next;
					}
				}
			}
			for(i = 0;i < 100 /*&& sBKSel != NULL*/;i++)//如果在另一处重新分配使用sBKSel，会有内存泄露
			{
				if(sBKSel[i].cWndTitle != NULL)
				{
					VirtualFree(sBKSel[i].cWndTitle,0,MEM_RELEASE);
				}
				if(sBKSel[i].cIconTip != NULL)
				{
					delete [] sBKSel[i].cIconTip;
				}
			}
			if(1/*sBKSel*/)
			{
				VirtualFree(sBKSel,0,MEM_RELEASE);
				sBKSel = NULL;
			}

			return FALSE;
		default:
			if(g_iUseLanguage == 1)
			{
				wchar_t * pText = new wchar_t[1024];
				SetWindowTextW(GetParent(hwndDlg),
					LoadItemStringW(L"IDS_WIZARDBEGIN_TITLE",pText,L"The specified background processing wizard"));
				delete [] pText;
			}
			return FALSE;
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
	int iRet = 0,i = 0;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		hList_Confirm = GetDlgItem(hwndDlg,IDC_LIST_VALIDATE);
		ListView_SetExtendedListViewStyle(hList_Confirm,LVS_EX_FULLROWSELECT);
		hImage_Confirm = ImageList_Create(16,16,ILC_COLOR24,iTotalBK,10);
		ListView_SetImageList(hList_Confirm,hImage_Confirm,LVSIL_SMALL);
		AddConfirmListHeader();
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_WIZARD_FINISH);
		}
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
		case PSN_QUERYCANCEL:
			iRet = HDMessageBoxW(L"IDS_MESSAGEBOX_SELECTEDWNDORICONISCANCEL",
				L"You have selected the windows or icons to be hidden, if cancel,these windows or icons will not be hidden, are you sure to cancel?",
				hwndDlg,MB_YESNO|MB_ICONQUESTION);
			if (iRet == IDYES)
			{
				for(i = 0;i < iTotalBK; i++)
				{
					if(sBKWI[i]->cWndTitle != NULL)
					{
						VirtualFree(sBKWI[i]->cWndTitle,0,MEM_RELEASE);
						sBKWI[i]->cWndTitle = NULL;
					}
					if(sBKWI[i]->cIconTip != NULL)
					{
						delete [] sBKWI[i]->cIconTip;
						sBKWI[i]->cIconTip = NULL;
					}
					if(wcscmp(sBKWI[i]->cProcessName,L"") == 0)
					{
						delete sBKWI[i];
					}
				}
				for(i = 0;i < 100;i++)
				{
					if(sBKSel[i].cWndTitle != NULL)
					{
						VirtualFree(sBKSel[i].cWndTitle,0,MEM_RELEASE);
					}
					if(sBKSel[i].cIconTip != NULL)
					{
						delete [] sBKSel[i].cIconTip;
					}
				}
				VirtualFree(sBKSel,0,MEM_RELEASE);
				delete [] sBKWI;
				sBKSel = NULL;
				sBKWI = NULL;

				return FALSE;
			}
			SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
			return TRUE;
		case PSN_WIZBACK:
			for(i = 0;i < iTotalBK; i++)
			{
				if(sBKWI[i]->cWndTitle != NULL)
				{
					VirtualFree(sBKWI[i]->cWndTitle,0,MEM_RELEASE);
					sBKWI[i]->cWndTitle = NULL;
				}
				if(sBKWI[i]->cIconTip != NULL)
				{
					delete [] sBKWI[i]->cIconTip;
					sBKWI[i]->cIconTip = NULL;
				}
				if(wcscmp(sBKWI[i]->cProcessName,L"") == 0)
				{
					delete sBKWI[i];
				}
			}
			for(i = 0;i < 100;i++)
			{
				if(sBKSel[i].cWndTitle != NULL)
				{
					VirtualFree(sBKSel[i].cWndTitle,0,MEM_RELEASE);
				}
				if(sBKSel[i].cIconTip != NULL)
				{
					delete [] sBKSel[i].cIconTip;
				}
			}
			VirtualFree(sBKSel,0,MEM_RELEASE);
			delete [] sBKWI;
			
			sBKSel = NULL;
			sBKWI = NULL;

			return FALSE;
		case PSN_WIZFINISH:
			HideSelected(sBKWI,iTotalBK);
			//ExtendWindow();
			break;
		default:
			if(g_iUseLanguage == 1)
			{
				wchar_t * pText = new wchar_t[1024];
				SetWindowTextW(GetParent(hwndDlg),
					LoadItemStringW(L"IDS_WIZARDBEGIN_TITLE",pText,L"The specified background processing wizard"));
				delete [] pText;
			}
			return FALSE;
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
	hWizardDialog = GetParent(hwndDlg);
	int fX = 0,fY = 0;
	HDC hScreenDC;
	
	HWND hStatic_BKIcon;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(!IsWindowVisible(hWizardDialog))
		{
			hScreenDC = GetDC(NULL);
			fX = GetDeviceCaps(hScreenDC,HORZRES)/4;
			fY = GetDeviceCaps(hScreenDC,VERTRES)/8;
			ReleaseDC(NULL,hScreenDC);
			SetWindowPos(hWizardDialog,NULL,fX,fY,0,0,SWP_NOSIZE|SWP_NOZORDER);
		}
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_WIZARD_BEGINBK);
		}
		hStatic_BKIcon = GetDlgItem(hwndDlg,IDC_STATIC_THISWIZARDTOHELPYOU);
		SendMessage(hStatic_BKIcon,WM_SETFONT,(WPARAM)hFont_New,0);
//		hStatic_BKProgram = GetDlgItem(hwndDlg,IDC_STATIC_SPECIFYPROGRAMTOBK);
//		SendMessage(hStatic_BKProgram,WM_SETFONT,(WPARAM)hFont_New,0);

		
		
		return FALSE;
	case WM_NOTIFY:
		switch(((LPPSHNOTIFY)lParam)->hdr.code)
		{
		case PSN_SETACTIVE:
			PropSheet_SetWizButtons(GetParent(hwndDlg),PSWIZB_NEXT);
			if(g_iUseLanguage == 1)
			{
				wchar_t * pText = new wchar_t[1024];
				SetWindowTextW(GetParent(hwndDlg),
					LoadItemStringW(L"IDS_WIZARDBEGIN_TITLE",pText,L"The specified background processing wizard"));
				delete [] pText;
			}
			break;
		case PSN_WIZNEXT:
			if(g_iUseLanguage == 1)
			{
				wchar_t * pText = new wchar_t[1024];
				SetWindowTextW(GetParent(hwndDlg),
					LoadItemStringW(L"IDS_WIZARDBEGIN_TITLE",pText,L"The specified background processing wizard"));
				delete [] pText;
			}
			break;			
		default:
			return FALSE;
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			
		case IDCANCEL:
			EndDialog(hwndDlg,0);
			
		}
	default:
		if(g_iUseLanguage == 1)
		{
			wchar_t * pText = new wchar_t[1024];
			SetWindowTextW(GetParent(hwndDlg),
				LoadItemStringW(L"IDS_WIZARDBEGIN_TITLE",pText,L"The specified background processing wizard"));
			delete [] pText;
		}

		return FALSE;
			
	}
	return FALSE;

}


int AddSelListHeader()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle;
	//lvColumn.pszText = new wchar_t(60);
	wchar_t * pTrayIcon = new wchar_t [1024],*pRelatedText = new wchar_t[1024],*pProcessID = new wchar_t[1024];

	
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 70;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Tray Icon";
	}
	else
	{
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_WIZARDSELWND_TRAYICON",pTrayIcon,L"Tray Icon"));
		lvColumn.pszText = pTrayIcon;
	}
	//ListView_InsertColumn(hList_SelWI,0,&lvColumn);
	SendMessageW(hList_SelWI,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	lvColumn.cx    = 260;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Window titles or tooltips related to the tray icons";
	}
	else
	{
		wcscpy(pRelatedText,LoadItemStringW(L"IDS_WIZARDSELWND_RELATEDTEXT",pRelatedText,
			L"Window titles or tooltips related to the tray icons"));
		lvColumn.pszText = pRelatedText;
	}
	//ListView_InsertColumn(hList_SelWI,1,&lvColumn);
	SendMessageW(hList_SelWI,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
	lvColumn.cx   = 80;

	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Process ID";
	}
	else
	{
		LoadItemStringW(L"IDS_WIZARDSELWND_PROCESSID",pProcessID,
			L"Process ID");
		lvColumn.pszText = pProcessID;
	}
	//ListView_InsertColumn(hList_SelWI,2,&lvColumn);
	SendMessageW(hList_SelWI,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_SelWI);
	lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_SelWI,0,lvStyle);

	delete [] pTrayIcon;
	delete [] pRelatedText;
	delete [] pProcessID;


	//delete []lvColumn.pszText;

	return 1;
}




int UpdateSelList()
{
	wchar_t * pszText = NULL;
	DWORD lPID;
	LVITEM lvItem;
	int iconIndex = 0,iconNum = 0;
	wchar_t cPID[20] = {0};
	int i = 0,j = 0;
	WndAndHandle *pNext = NULL,*pTemp = NULL,*pBKSel = NULL,* pTemp2 = NULL;
	ICONINFO iconInfo;
	sBKSel = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(sBKSel,0,sizeof(WndAndHandle)*100);
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

	for(i = 0;i<iW_Sel;i++)
	{
		sBKSel[i].iHasTrayIcon = 0;
		sBKSel[i].iIconIndex = 0;
		sBKSel[i].iCommandId = 0;
		sBKSel[i].hWnd = NULL;
		sBKSel[i].trayicon.hIcon = NULL;
		sBKSel[i].lPID = 0;
		sBKSel[i].bExist = 0;
		memset(sBKSel[i].cProcessName,0,30 * sizeof(wchar_t));
		memset(sBKSel[i].cIconTip,0,1024 * sizeof(wchar_t));
		memset(sBKSel[i].cWndTitle,0,2048 * sizeof(wchar_t));
		//sBKSel[i].cWndTitle = NULL;
	}
	iW_Sel = 0;
	iNoTitle = 0;
	
	EnumWindows(EnumWindowsForBKProc,(LPARAM)sBKSel);

	FillNotExistWnd(1);

	for (i = 0;i<iW_Sel-1;i++)
	{
		for(j = i+1;j <= iW_Sel-1;j++)
		{
			if(sBKSel[i].lPID == sBKSel[j].lPID && wcscmp(sBKSel[i].cProcessName,L"explorer.exe") != 0)
			{
				if(sBKSel[i].bReserved == 1)
				{
					wcscpy(sBKSel[j].cWndTitle,sBKSel[i].cWndTitle);
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
				pTemp->iLink ++;
				break;
			}
		}
	}

	

	GetWindowThreadProcessId(hTrayWnd,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t Tipwchar_t = 0;
	wchar_t sTip[1024] = {0};
	wchar_t* pTip = NULL;
	int x = 0,iRet = 0;
	ListView_DeleteAllItems(hList_SelWI);



	
	

	
	int iBtnCount = SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);
	iconNum = 0;
	DWORD lExID = 0;
	for(i=0; i < iBtnCount;i++)
	{
		if (i <= 30)
		{
		}
		else
		{
			//MessageBox(hMainDialog,"arrTray array is not enough","隐身专家",MB_OK);
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
				DWORD dwErr = GetLastError();
				tray.hIcon = hIcon_Unknown;
				GetProcessTrayIcon(lPID,&tb,&tray);
			}
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
			iconIndex = ImageList_AddIcon(hImage_Sel,tray.hIcon);
			lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM;
			//lvItem.pszText = pszText;
			lvItem.iImage = iconIndex;
			lvItem.pszText = L"";
			lvItem.iSubItem = 0;
			pWH = NULL;
			pWH = new WndAndHandle;
			memset(pWH,0,sizeof(WndAndHandle));
			pWH->bHide = tb.idCommand;//Use this member save idcommand for hide or show
			pWH->cWndTitle = NULL;
			pWH->cIconTip = (wchar_t*)new wchar_t[1024];
			memset(pWH->cIconTip,0,1024 * sizeof(wchar_t));
			memset(pWH->cProcessName,0,30 * sizeof(wchar_t));
			GetWindowThreadProcessId(hTrayWnd,&lExID);
			if(lExID == lPID)
			{
				wcscpy(pWH->cProcessName,L"explorer.exe");
			}

			pWH->trayicon = tray;
			pWH->lPID = lPID;
			pWH->hWnd = NULL;
			pWH->Next = NULL;
			lvItem.lParam = (LPARAM)pWH;
			lvItem.iItem = ListView_GetItemCount(hList_SelWI);
			ListView_InsertItem(hList_SelWI,&lvItem);

			memset(sTip,0,1024*(sizeof(wchar_t)));
			x = 0;
			pTip = (wchar_t*)tb.iString;
			do 
			{	
				if(x >= 1023)
				{
					wcscpy(sTip,L"[ToolTip was either too long or not set]");
					break;
				}
				
				data.ReadData_Tip((wchar_t*)&Tipwchar_t, (void *)pTip++);
			}while(x < 1000 && (sTip[x++] = Tipwchar_t));

			iRet = 0;
			wcscpy(pWH->cIconTip,sTip);


			for(int j = 0;j < iW_Sel;j++)
			{
				if (lPID == sBKSel[j].lPID && wcscmp(sBKSel[j].cProcessName,L"explorer.exe")!=0 )
				{
					if (sBKSel[j].bExist == 1)
					{
						sBKSel[j].iHasTrayIcon = 1;
						continue;
					}

					sBKSel[j].trayicon = tray;
					lvItem.lParam = (LPARAM)(sBKSel +j);
					pTemp = sBKSel + j;
					while(pTemp)
					{
						wcscpy(pTemp->cIconTip,pWH->cIconTip);
						pTemp->bHide = pWH->bHide;//bHide SAVE TBBUTTON idcommand
						pTemp = pTemp->Next;
					}
					delete [] pWH->cIconTip;
					delete pWH;
					iRet = ListView_SetItem(hList_SelWI,&lvItem);
					lvItem.mask = LVIF_TEXT;
					lvItem.iSubItem = 1;

					//lvItem.pszText = sBKSel[j].cWndTitle;
					{//Use icon tip 
						int idx = 0;
						for(idx = 0;idx < iW_Sel; idx ++)
						{
							if(lPID == sBKSel[idx].lPID && 
								wcscmp(sBKSel[idx].cIconTip,L""))
							{
								lvItem.pszText = sBKSel[idx].cIconTip;
								break;
							}
						}
					}

					iRet = ListView_SetItem(hList_SelWI,&lvItem);
					sBKSel[j].iHasTrayIcon = 1;
					break;
				}
				else if(lPID == sBKSel[j].lPID && wcscmp(sBKSel[j].cProcessName,L"explorer.exe")==0)
				{
					j = iW_Sel;
					break;
				}
			}
			if(j == iW_Sel)
			{
				lvItem.pszText = pWH->cIconTip;
				lvItem.mask = LVIF_TEXT;
				lvItem.iSubItem = 1;
				ListView_SetItem(hList_SelWI,&lvItem);
			}
			
			

			lvItem.mask = LVIF_TEXT|LVIF_IMAGE;
			lvItem.lParam = NULL;
			swprintf(cPID,L"%X",lPID);
			lvItem.iSubItem = 2;
			lvItem.pszText = cPID;
			iRet = ListView_SetItem(hList_SelWI,&lvItem);
			if(iRet != TRUE)
			{
				//MessageBox(hMainDialog,"listview_setitem error","隐身专家",MB_OK);
			}
			
			arrTray[i] = tray;
			iconNum ++;
		}
	}


	
	for(j = 0;j<iW_Sel ;j++)
	{
		if( sBKSel[j].iHasTrayIcon != 1 && sBKSel[j].bExist != 1)
		{
			GetWindowThreadProcessId(sBKSel[j].hWnd,&lPID);

			if(lPID == GetCurrentProcessId())
			{
				continue;
			}
			lvItem.mask = LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE;
			lvItem.lParam = (LPARAM)(sBKSel + j);
			lvItem.iItem = ListView_GetItemCount(hList_SelWI);
			lvItem.iSubItem = 0;
			lvItem.iImage = 0;
			lvItem.pszText = L"";
			ListView_InsertItem(hList_SelWI,&lvItem);


			lvItem.mask = LVIF_TEXT;
			lvItem.iSubItem = 1;
			lvItem.pszText = sBKSel[j].cWndTitle;
			ListView_SetItem(hList_SelWI,&lvItem);

			lvItem.iSubItem = 2;
			swprintf(cPID,L"%X",sBKSel[j].lPID);
			lvItem.pszText = cPID;
			ListView_SetItem(hList_SelWI,&lvItem);
		}
	}


	return 1;
}


int AllocResourceForTrayList()
{
	g_QuickHideAllTray = (WndAndHandle*)VirtualAlloc(NULL,sizeof(WndAndHandle)*100,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	for(i = 0;i<100;i++)
	{
		g_QuickHideAllTray[i].cWndTitle = NULL;//(wchar_t *) VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		g_QuickHideAllTray[i].cIconTip = (wchar_t*)new wchar_t [1024];
		memset(g_QuickHideAllTray[i].cIconTip,0,1024 * sizeof(wchar_t));
//		memset(g_QuickHideAllTray[i].cWndTitle,0,2048 * sizeof(wchar_t));
		memset(g_QuickHideAllTray[i].cProcessName,0,30 * sizeof(wchar_t));
		g_QuickHideAllTray[i].Next = NULL;
	}

	

	return 1;
}

extern HBRUSH g_brushAllPage;
extern HBRUSH g_brushTrayPage;
extern HBRUSH g_brushBKPage;

extern HFONT hFont_New2;

int ReleaseResourceForTrayList()
{
	int i = 0;
	for(i = 0;i<100;i++)
	{
		delete [] g_QuickHideAllTray[i].cIconTip;
	}
	VirtualFree(g_QuickHideAllTray,0,MEM_RELEASE);
	DeleteObject((HGDIOBJ)g_brushAllPage);
	DeleteObject((HGDIOBJ)g_brushTrayPage);
	DeleteObject((HGDIOBJ)hFont_New2);
	DeleteObject((HGDIOBJ)g_brushBKPage);
	return 1;
}


int EnumAllVisibleTray()
{
	wchar_t * pszText = NULL;
	DWORD lPID;
	int iconIndex = 0,iconNum = 0;
	wchar_t cPID[20] = {0};
	int i = 0,j = 0;
	ICONINFO iconInfo;




	for(i = 0;i<100;i++)
	{
		if(g_QuickHideAllTray[i].trayicon.Reserved[0] == 22
			&& g_QuickHideAllTray[i].trayicon.Reserved[1] == 33)
		{
			DestroyIcon(g_QuickHideAllTray[i].trayicon.hIcon);
		}
		g_QuickHideAllTray[i].iHasTrayIcon = 0;
		g_QuickHideAllTray[i].iIconIndex = 0;
		g_QuickHideAllTray[i].iCommandId = 0;
		g_QuickHideAllTray[i].hWnd = NULL;
		g_QuickHideAllTray[i].trayicon.hIcon = NULL;
		g_QuickHideAllTray[i].trayicon.hwnd = NULL;
		g_QuickHideAllTray[i].trayicon.uID = 0;
		g_QuickHideAllTray[i].trayicon.uCallbackMessage = 0;

		g_QuickHideAllTray[i].lPID = 0;
		g_QuickHideAllTray[i].bHide = 0;
		g_QuickHideAllTray[i].bExist = 0;
		memset(g_QuickHideAllTray[i].cProcessName,0,30 * sizeof(wchar_t));
		memset(g_QuickHideAllTray[i].cIconTip,0,1024 * sizeof(wchar_t));
		//memset(g_QuickHideAllTray[i].cWndTitle,0,2048 * sizeof(wchar_t));
	}




	GetWindowThreadProcessId(hTrayWnd,&lPID);
	CProcessData<TBBUTTON> data(lPID);
	TBBUTTON tb = {0};
	TRAYDATA tray = {0};
	TrayItemInfo tifo = {0};
	wchar_t Tipwchar_t = 0;
	wchar_t sTip[1024] = L"\0";
	wchar_t* pTip = NULL;
	int x = 0,iRet = 0;
	
	j = 0;
	int iBtnCount = SendMessage(hTrayWnd,TB_BUTTONCOUNT,0,0);

	DWORD lExID = 0;
	for(i=0; i < iBtnCount;i++)
	{
		if (i <= 100)
		{
		}
		else
		{
			break;
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
			memset(&iconInfo,0,sizeof(iconInfo));
			if(GetIconInfo(tray.hIcon,&iconInfo) == 0)
			{
				DWORD dwErr = GetLastError();
				tray.hIcon = hIcon_Unknown;
				GetProcessTrayIcon(lPID,&tb,&tray);
				if(tray.hIcon == NULL)
				{
					tray.hIcon = hIcon_Unknown;
				}
			}
			if(iconInfo.hbmColor != NULL)
			{
				DeleteObject(iconInfo.hbmColor);
			}
			if(iconInfo.hbmMask != NULL)
			{
				DeleteObject(iconInfo.hbmMask);
			}
			g_QuickHideAllTray[j].bHide = tb.idCommand;//Use this member save idcommand for hide or show
			g_QuickHideAllTray[j].cWndTitle = NULL;
			//g_QuickHideAllTray[j].cIconTip = new wchar_t[1024];
			//memset(g_QuickHideAllTray[j].cIconTip,0,1024 * sizeof(wchar_t));
			memset(g_QuickHideAllTray[j].cProcessName,0,30 * sizeof(wchar_t));
			GetWindowThreadProcessId(hTrayWnd,&lExID);
			if(lExID == lPID)
			{
				wcscpy(g_QuickHideAllTray[j].cProcessName,L"explorer.exe");
			}

			//g_QuickHideAllTray[j].trayicon = tray;
			g_QuickHideAllTray[j].trayicon.hIcon = tray.hIcon;
			g_QuickHideAllTray[j].trayicon.hwnd = tray.hwnd;
			//g_QuickHideAllTray[j].trayicon.Reserved = tray.Reserved;
			g_QuickHideAllTray[j].trayicon.uCallbackMessage = tray.uCallbackMessage;
			g_QuickHideAllTray[j].trayicon.uID = tray.uID;
			g_QuickHideAllTray[j].lPID = lPID;
			g_QuickHideAllTray[j].hWnd = NULL;
			g_QuickHideAllTray[j].Next = NULL;

			memset(sTip,0,1024*(sizeof(wchar_t)));
			x = 0;
			pTip = (wchar_t*)tb.iString;
			do 
			{	
				if(x >= 1000)
				{
					wcscpy(sTip,L"[ToolTip was either too long or not set]");
					break;
				}
				
				data.ReadData_Tip((wchar_t*)&Tipwchar_t, (void *)pTip++);
			}while( x < 1000 && (sTip[x++] = Tipwchar_t));

			iRet = 0;
			wcscpy(g_QuickHideAllTray[j].cIconTip,sTip);

			j ++;
		}
	}


	return 1;
}



int HideAllTray()
{
	int i = 0;
	while(g_QuickHideAllTray[i].lPID != 0 && i < 100)
	{
		int iCommandId = g_QuickHideAllTray[i].bHide;
		SendMessageW(g_TrayWnd,TB_HIDEBUTTON,WPARAM(iCommandId),
			(LPARAM)MAKELONG(TRUE,0));
		{
			NOTIFYICONDATA nid2;
			memset(&nid2,0,sizeof(nid2));
			nid2.cbSize=sizeof(nid2);
			nid2.hWnd=g_QuickHideAllTray[i].trayicon.hwnd;
			nid2.uID=g_QuickHideAllTray[i].trayicon.uID;
			nid2.uFlags=NIF_STATE;
			nid2.dwState=NIS_HIDDEN;
			::SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)FALSE,0);
			Shell_NotifyIcon(NIM_MODIFY,&nid2);
			SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)TRUE,0);
		}
		i++;
	}
	


	return 1;
}



int ShowAllTray()
{
	NOTIFYICONDATA nid;
	int i = 0;
	while(g_QuickHideAllTray[i].lPID == 0 && i < 100)
	{
		if(g_QuickHideAllTray[i].bHide > 0)
		{
			nid.cbSize           = sizeof(NOTIFYICONDATA);
			nid.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
			nid.uID              = g_QuickHideAllTray[i].trayicon.uID;
			nid.hIcon            = g_QuickHideAllTray[i].trayicon.hIcon;
			nid.hWnd             = g_QuickHideAllTray[i].trayicon.hwnd;
			nid.uCallbackMessage = g_QuickHideAllTray[i].trayicon.uCallbackMessage;
			{
				int iCommandId = g_QuickHideAllTray[i].bHide;
				if(iCommandId == 0)
				{
					iCommandId = g_QuickHideAllTray[i].iCommandId;
				}
				SendMessageW(g_TrayWnd,TB_HIDEBUTTON,WPARAM(iCommandId),(LPARAM)MAKELONG(FALSE,0));
			}
			{
				NOTIFYICONDATA nid2;
				memset(&nid2,0,sizeof(nid2));
				nid2.cbSize=sizeof(nid2);
				nid2.hWnd=nid.hWnd;
				nid2.uID=nid.uID;
				nid2.uFlags=NIF_STATE;
				nid2.dwState=NIS_HIDDEN;
				::SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)FALSE,0);
				Shell_NotifyIcon(NIM_MODIFY,&nid2);
				SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)TRUE,0);
			}
		}
		i ++;
	}
	


	return 1;
}





BOOL CALLBACK EnumWindowsForBKProc(HWND hwnd,LPARAM lParam)
{
	//HWND hOnw;
	DWORD lPID;
	WndAndHandle * sBKSel = (WndAndHandle*)lParam;
	HANDLE hProcessSnap2_bk = NULL,hProcessSnap2 = NULL;

	if (::GetWindowLong(hwnd,GWL_STYLE) & WS_VISIBLE)
	{	
		cBKWndText = (wchar_t*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
		memset(cBKWndText,0,2048 * sizeof(wchar_t));
		GetWindowText(hwnd,cBKWndText,2048);
		GetWindowThreadProcessId(hwnd,&lPID);
		if (/*!wcscmp(cBKWndText,"")||*/!wcscmp(cBKWndText,L"Program Manager"))
		{
			VirtualFree(cBKWndText,0,MEM_RELEASE);
			return 1;
		}
		if (hwnd == hMainDialog||::GetParent(hwnd) == hMainDialog)
		{
			VirtualFree(cBKWndText,0,MEM_RELEASE);
			return 1;
		}


		hProcessSnap2 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		hProcessSnap2_bk = hProcessSnap2;
		pe322.dwSize = sizeof(PROCESSENTRY32);
		Process32First((void*)hProcessSnap2,&pe322);
		int iFoundFlag_BK = 0;
		while(Process32Next((void*)hProcessSnap2,&pe322))
		{
			if(pe322.th32ProcessID == lPID)
			{
				iFoundFlag_BK = 1;
				if(wcscmp(wcslwr(pe322.szExeFile),L"explorer.exe")==0)
				{
					if (wcscmp(cBKWndText,L"") != 0)
					{
						wcscpy(sBKSel[iW_Sel].cProcessName,pe322.szExeFile);
						sBKSel[iW_Sel].hWnd = hwnd;
						sBKSel[iW_Sel].lPID = lPID;
						sBKSel[iW_Sel].Next = NULL;
						GetWindowText(hwnd,sBKSel[iW_Sel].cWndTitle,2040);
						iW_Sel ++;
					}
				}
				else
				{
					wcscpy(sBKSel[iW_Sel].cProcessName,pe322.szExeFile);
					sBKSel[iW_Sel].hWnd = hwnd;
					sBKSel[iW_Sel].lPID = lPID;
					sBKSel[iW_Sel].Next = NULL;
					if (wcscmp(cBKWndText,L"") == 0)
					{
						wcscpy(sBKSel[iW_Sel].cWndTitle,pe322.szExeFile);
						sBKSel[iW_Sel].bReserved = 1;
					}
					else
					{
						GetWindowText(hwnd,sBKSel[iW_Sel].cWndTitle,2040);
					}
					if (IsWindow(GetParent(hwnd)))
					{
						sBKSel[iW_Sel].iWindowType = 1;
					}
					iW_Sel ++;
				}
				VirtualFree(cBKWndText,0,MEM_RELEASE);
				return 1;
			}
		}
		//hProcessSnap2 = hProcessSnap2_bk;
		CloseHandle((HANDLE)hProcessSnap2);
		CloseHandle((HANDLE)hProcessSnap2_bk);
		if(iFoundFlag_BK == 0)
		{
			//ShowProcess(sW[iForSW].lPID);
			wchar_t cProcessName[50] = L"";
			GetHidedProcessName(lPID,cProcessName);

			
			
			if(wcscmp(wcslwr(cProcessName),L"explorer.exe")==0)
			{
				if (wcscmp(cBKWndText,L"") != 0)
				{
					wcscpy(sBKSel[iW_Sel].cProcessName,cProcessName);
					sBKSel[iW_Sel].hWnd = hwnd;
					sBKSel[iW_Sel].lPID = lPID;
					sBKSel[iW_Sel].Next = NULL;
					GetWindowText(hwnd,sBKSel[iW_Sel].cWndTitle,2040);
					iW_Sel ++;
				}
			}
			else
			{
				wcscpy(sBKSel[iW_Sel].cProcessName,cProcessName);
				sBKSel[iW_Sel].hWnd = hwnd;
				sBKSel[iW_Sel].lPID = lPID;
				sBKSel[iW_Sel].Next = NULL;
				if (wcscmp(cBKWndText,L"") == 0)
				{
					wcscpy(sBKSel[iW_Sel].cWndTitle,cProcessName);
					sBKSel[iW_Sel].bReserved = 1;
				}
				else
				{
					GetWindowText(hwnd,sBKSel[iW_Sel].cWndTitle,2040);
				}
				if (IsWindow(GetParent(hwnd)))
				{
					sBKSel[iW_Sel].iWindowType = 1;
				}
				iW_Sel ++;
			}





		}




		VirtualFree(cBKWndText,0,MEM_RELEASE);
	}
	return 1;
}


int AddConfirmListHeader()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle;
	
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 70;
	wchar_t * pTrayIcon = new wchar_t [1024],*pRelatedText = new wchar_t[1024],*pProcessID = new wchar_t[1024];
	
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Tray Icon";
	}
	else
	{
		wcscpy(pTrayIcon,LoadItemStringW(L"IDS_WIZARDFINISH_TRAYICON",pTrayIcon,L"Tray Icon"));
		lvColumn.pszText = pTrayIcon;
	}
	//ListView_InsertColumn(hList_Confirm,0,&lvColumn);
	SendMessageW(hList_Confirm,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
	lvColumn.cx    = 260;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Window titles or tooltips related to the tray icons";
	}
	else
	{
		wcscpy(pRelatedText,LoadItemStringW(L"IDS_WIZARDFINISH_RELATEDTEXT",pRelatedText,
			L"Window titles or tooltips related to the tray icons"));
		lvColumn.pszText = pRelatedText;
	}
	//ListView_InsertColumn(hList_Confirm,1,&lvColumn);
	SendMessageW(hList_Confirm,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
	lvColumn.cx   = 80;
	if(g_iUseLanguage == 0)
	{
		lvColumn.pszText = L"Process ID";
	}
	else
	{
		LoadItemStringW(L"IDS_WIZARDFINISH_PROCESSID",pProcessID,
			L"Process ID");
		lvColumn.pszText = pProcessID;
	}
	//ListView_InsertColumn(hList_Confirm,2,&lvColumn);
	SendMessageW(hList_Confirm,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_SelWI);
	lvStyle = lvStyle|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_SelWI,0,lvStyle);

	delete [] pTrayIcon;
	delete [] pRelatedText;
	delete [] pProcessID;


	return 1;
}



int UpdateConfirmList()
{
	LVITEM conItem;
	int i = 0,iIconIndex = 0,iRet = 0;
	wchar_t cPID[20];
	wchar_t * pszTemp = NULL;


	ListView_DeleteAllItems(hList_Confirm);
	for( i = 0;i<iTotalBK;i++)
	{
		conItem.mask =LVIF_IMAGE|LVIF_PARAM;
		conItem.iItem = ListView_GetItemCount(hList_Confirm);
		conItem.lParam = (LPARAM)sBKWI[i];
		if(sBKWI[i]->trayicon.hIcon != NULL)
		{
			iIconIndex = ImageList_AddIcon(hImage_Confirm,sBKWI[i]->trayicon.hIcon);
		}
		else
		{
			iIconIndex = 0;
		}
		conItem.iImage = iIconIndex;
		conItem.iSubItem = 0;
		iRet = ListView_InsertItem(hList_Confirm,&conItem);

		conItem.mask = LVIF_TEXT;
		conItem.iSubItem = 1;
		if(!IsWindow(sBKWI[i]->hWnd))
		{
			conItem.pszText = sBKWI[i]->cIconTip;
		}
		else
		{
			conItem.pszText = sBKWI[i]->cWndTitle;
		}
		iRet = ListView_SetItem(hList_Confirm,&conItem);


		conItem.iSubItem = 2;
		swprintf(cPID,L"%X",sBKWI[i]->lPID);
		conItem.pszText= cPID;
		iRet = ListView_SetItem(hList_Confirm,&conItem);
	}
		
	

	return 1;
}




int IsProcessItem(WndAndHandle * pWH)
{

	if(pWH->cWndTitle == NULL && pWH->hWnd == NULL && pWH->trayicon.hIcon == NULL)
	{
		return 1;
	}


	return 0;
}



HANDLE hBKWindowHandle[20];
HWND hBKWnd[20];
long g_BKThreadCount = 0;

CRITICAL_SECTION sc_BKWnd;




//InitializeCriticalSection(&sc_BKWnd);

int HideWindowInThread(HWND * hWnd)
{
	//EnterCriticalSection(&sc_BKWnd);

	hBKWnd[g_BKThreadCount] = *hWnd;
	hBKWindowHandle[g_BKThreadCount] = (HANDLE)_beginthreadex(NULL,0,BKWindowThread,
		(void*)&(hBKWnd[g_BKThreadCount]),0,NULL);
	
	InterlockedIncrement(&g_BKThreadCount);// ++;

	//LeaveCriticalSection(&sc_BKWnd);
	

	
	
	return 1;
}



HANDLE hMonitorBKThread;

int BeginBKMonitorThread()
{
	dbview(L"enter BeginBKMonitorThread.......................................");

	
	hMonitorBKThread= (HANDLE)_beginthreadex(NULL,0,MonitorBKProc,
		hBKWindowHandle,0,NULL);


	HANDLE hMonitorBKHandle = hMonitorBKThread;

	if(hMonitorBKHandle == INVALID_HANDLE_VALUE)
	{
		hMonitorBKHandle = NULL;
	}


	dbview(L"leave BeginBKMonitorThread.......................................");
	return 1;
}





 

unsigned __stdcall BKWindowThread(void * pParam)
{
	dbview(L"enter BKWindowThread.......................................");

	HWND * pWnd = (HWND *)pParam;
	
	HWND hWnd = (*pWnd);
	ShowWindow(hWnd,SW_HIDE);

	//g_BKThreadCount --;


	//CloseHandle()
	InterlockedDecrement(& g_BKThreadCount);
	dbview(L"leave BKWindowThread.......................................");
	
	
	return 1;
}





int dbview(wchar_t * pInfo)
{
	OutputDebugString(pInfo);
	

	return 0;
}

unsigned __stdcall MonitorBKProc(void * pParam)
{
	DWORD dwErr = 0;
	BOOL bRet = FALSE;
	
	dbview(L"enter MonitorBKProc.......................................");
	int i = 0;
	
	Sleep(1000*1);
	DWORD dwCode = 0;
	while(i < g_BKThreadCount)
	{
		dbview(L"enter bkmonitor thread......i < g_BKThreadCount.........");
		GetExitCodeThread(hBKWindowHandle[i],&dwCode);
		if(dwCode == STILL_ACTIVE)
		{
			TerminateThread(hBKWindowHandle[i],101);
			dbview(L"termimate bk thread.");
		}

		CloseHandle(hBKWindowHandle[i]);
				
		i++;
	}
	

	{
		CloseHandle(hMonitorBKThread);
	}

	g_BKThreadCount = 0;

	dbview(L"leave MonitorBKProc.......................................");
	
	return 1;
}


int g_iBmpIdx = 0;

int HideSelected(WndAndHandle **pWH,int iSize,int iAutoHide /* == 0*/ )
{
	LVITEM bkItem;
	int i = 0,iIconIndex = 0;
	NOTIFYICONDATA nid;
	wchar_t cPID[20];

	WndAndHandle * pNext = NULL, * pFirst = NULL,*pBKSel = NULL,*pTemp = NULL;

	
	for(i = 0;i<iSize;i++)
	{
		sBK_All = new WndAndHandle;
		memset(sBK_All,0,sizeof(WndAndHandle));
		sBK_All->hWnd = pWH[i]->hWnd;
		sBK_All->iHasTrayIcon = pWH[i]->iHasTrayIcon;
		sBK_All->iCommandId = pWH[i]->iCommandId;
		sBK_All->lPID = pWH[i]->lPID;
		sBK_All->trayicon = pWH[i]->trayicon;
		sBK_All->bHide = pWH[i]->bHide;
		if(pWH[i]->cWndTitle != NULL)
		{
			sBK_All->cWndTitle = (wchar_t*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
			memset(sBK_All->cWndTitle,0,2048 * sizeof(wchar_t));
		}
		else
		{
			sBK_All->cWndTitle = NULL;
		}

		if(IsProcessItem(pWH[i]) == 0)
		{
			sBK_All->cIconTip = new wchar_t[1024];
			memset(sBK_All->cIconTip,0,1024 * sizeof(wchar_t));
		}
		if(pWH[i]->cWndTitle != NULL)
		{
			wcscpy(sBK_All->cWndTitle,pWH[i]->cWndTitle);
		}
		if(wcscmp(pWH[i]->cProcessName,L"") != 0)
		{
			wcscpy(sBK_All->cProcessName,pWH[i]->cProcessName);
		}
		if(IsProcessItem(pWH[i]) == 0)
		{
			wcscpy(sBK_All->cIconTip,pWH[i]->cIconTip);
		}

		if(IsWindow(pWH[i]->hWnd))
		{
			//ShowWindow(pWH[i]->hWnd,SW_HIDE);
			HideWindowInThread(&(pWH[i]->hWnd));
		}
		if(pWH[i]->trayicon.hIcon != NULL )
		{
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = pWH[i]->trayicon.hwnd;
			nid.uID = pWH[i]->trayicon.uID;
//			Shell_NotifyIcon(NIM_DELETE,&nid);
			int iCommandId = pWH[i]->bHide;//Use this member to save TBBUTTON idCommand
			if(iCommandId == 0)
			{
				iCommandId = pWH[i]->iCommandId;//Use menu bk now the commandid is saved in icommandid
				pWH[i]->bHide = iCommandId;
			}
			SendMessageW(g_TrayWnd,TB_HIDEBUTTON,WPARAM(iCommandId),
				(LPARAM)MAKELONG(TRUE,0));
// 			if(g_iBmpIdx != 0)
// 			{
// 				
// 				SendMessage(g_TrayWnd,
// 					TB_CHANGEBITMAP,iCommandId,MAKELPARAM(g_iBmpIdx,   0));				
// 			}
// 			else
// 			{
// 				g_iBmpIdx = SendMessage(g_TrayWnd,
// 					TB_GETBITMAP,iCommandId,0);
// 
// 				SendMessage(g_TrayWnd,
// 					TB_CHANGEBITMAP,iCommandId,MAKELPARAM(g_iBmpIdx + 1,   0));
// 			}


			{
				NOTIFYICONDATA nid2;
				memset(&nid2,0,sizeof(nid2));
				nid2.cbSize=sizeof(nid2);
				nid2.hWnd=nid.hWnd;
				nid2.uID=nid.uID;
				nid2.uFlags=NIF_STATE;
				nid2.dwState=NIS_HIDDEN;
				::SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)FALSE,0);
				Shell_NotifyIcon(NIM_MODIFY,&nid2);
				SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)TRUE,0);
			}
		}
		pFirst = pWH[i];
		while(pWH[i]->Next)
		{
			pNext = pWH[i]->Next;
			pWH[i] = pNext;
			//ShowWindow(pWH[i]->hWnd,SW_HIDE);
			HideWindowInThread(&(pWH[i]->hWnd));
		}

		BeginBKMonitorThread();

		dbview(L"start WaitForSingleObject(hMonitorBKThread,100000*2);.......................................");
		//WaitForSingleObject(hMonitorBKThread,100000*2);

		dbview(L"leave WaitForSingleObject(hMonitorBKThread,100000*2);.......................................");

		if(pWH[i]->hWnd && IsWindow(pWH[i]->hWnd) == FALSE && 
			iSize == 1 && pWH[i]->cWndTitle == NULL)
		{
			
		}
		else
		{
			if(sOP.bHideProcess == 1 && IsProcessItem(pWH[i]) != TRUE)
			{
				HideProcess(pWH[i]->lPID);
			}
		}
		pWH[i] = pFirst;
		

		if(IsProcessItem(pWH[i]))
		{
			HideProcess(pWH[i]->lPID);
		}



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
			//wcscpy(bkItem.pszText,"");
			bkItem.pszText = L"";
		}
		else
		{
			bkItem.pszText = L"";
			
		}
		//pTemp = ((WndAndHandle*)lvItem.lParam);
		pTemp = sBK_All;
		pBKSel = pWH[i];
		do
		{
			pTemp->lPID =pBKSel->lPID;
			if(pBKSel->cWndTitle != NULL)
			{
				wcscpy(pTemp->cWndTitle,pBKSel->cWndTitle);
			}
			if(pBKSel->cIconTip != NULL && wcscmp(pBKSel->cIconTip,L""))
			{
				wcscpy(pTemp->cIconTip,pBKSel->cIconTip);
			}
			else
			{
				//pTemp->cIconTip = NULL;
				if(IsProcessItem(pWH[i]) == 0)
				{
					memset(pTemp->cIconTip,0,1024 * sizeof(wchar_t));
				}
			}
			pTemp->trayicon = pBKSel->trayicon;
			pTemp->bReserved = pBKSel->bReserved;
			pTemp->hWnd = pBKSel->hWnd;
			pTemp->bExist = pBKSel->bExist;
			wcscpy(pTemp->cProcessName,pBKSel->cProcessName);
			pTemp->iHasTrayIcon = pBKSel->iHasTrayIcon;
			pTemp->iCommandId = pBKSel->iCommandId;
			pTemp->iWindowType = pBKSel->iWindowType;
			pTemp->iLink = pBKSel->iLink;
			pTemp->iCommandId = pBKSel->iCommandId;
			pTemp->bHide = pBKSel->bHide;
			if(pBKSel->Next != NULL)
			{
				pNext = new WndAndHandle;
				memset(pNext,0,sizeof(WndAndHandle));
				if((pBKSel->Next)->cWndTitle != NULL)
				{
					pNext->cWndTitle = (wchar_t*)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
					memset(pNext->cWndTitle,0,2048 * sizeof(wchar_t));
				}
				pNext->cIconTip = new wchar_t[1024];
				memset(pNext->cIconTip,0,1024 * sizeof(wchar_t));
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
		if(pWH[i]->hWnd != NULL && IsWindow(pWH[i]->hWnd))
		{
			//GetWindowText(pWH[i]->hWnd,pWH[i]->cWndTitle,2022);
			//bkItem.pszText=pWH[i]->cWndTitle;//pWH可能在下面释放了，所以不用这个指针
			bkItem.pszText=pTemp->cWndTitle;//pTemp是新分配的，又设置成了Item的lParam，使用这个指针
		}
		else
		{
			//bkItem.pszText= pWH[i]->cIconTip;//pWH可能在下面释放了，所以不用这个指针
			bkItem.pszText= pTemp->cIconTip;//pTemp是新分配的，又设置成了Item的lParam，使用这个指针
		}
		if(IsProcessItem(pWH[i]))
		{
			bkItem.pszText= pTemp->cProcessName;
		}
		BOOL bRet = ListView_SetItem(hList_BK,&bkItem);
		if(bRet == FALSE)
		{
			DWORD dwErr = GetLastError();

		}

		bkItem.iSubItem = 2;
		swprintf(cPID,L"%X",pWH[i]->lPID);
		bkItem.pszText=cPID;
		ListView_SetItem(hList_BK,&bkItem);

		if(pWH[i]->hWnd == NULL && pWH[i]->cIconTip != NULL && pWH[i]->Next == NULL)
		{
			delete [] pWH[i]->cIconTip;
			delete pWH[i];
		}
	}
	iBKItemCount = 1;

	//Free memory
	if(sBKSel != NULL && iAutoHide == 0)
	{
		for(i = 0;i < 100;i++)
		{
			if(sBKSel[i].cWndTitle != NULL)
			{
				VirtualFree(sBKSel[i].cWndTitle,0,MEM_RELEASE);
			}
			if(sBKSel[i].cIconTip != NULL)
			{
				delete [] sBKSel[i].cIconTip;
			}
		}
		VirtualFree(sBKSel,0,MEM_RELEASE);
		sBKSel = NULL;
	}

	if(sBKSel_con != NULL)
	{
		for(i = 0;i < 100;i++)
		{
			if(sBKSel_con[i].cWndTitle != NULL)
			{
				VirtualFree(sBKSel_con[i].cWndTitle,0,MEM_RELEASE);
			}
			if(sBKSel_con[i].cIconTip != NULL)
			{
				delete [] sBKSel_con[i].cIconTip;
			}
		}
		VirtualFree(sBKSel_con,0,MEM_RELEASE);
	}
	sBKSel_con = NULL;


	if(sBKWI != NULL)
	{
		delete [] sBKWI;
		sBKWI = NULL;
	}
	EnableWindow(hButton_ShowBK,TRUE);
	return 1;
}


int ShowSelected(WndAndHandle * pWH,int iSize)
{
	int i = 0,j = 0;
	NOTIFYICONDATA nid;
	WndAndHandle * pTemp = NULL, * pFirst = NULL, * pNext = NULL,*pAddr = NULL;

	if(IsProcessItem(pWH))
	{
		ShowProcess(pWH->lPID);
	}
	if(pWH->hWnd != NULL && IsWindow(pWH->hWnd))
	{
		if(sOP.bHideProcess == 1)
		{
			ShowProcess(pWH->lPID);
		}
		ShowWindow(pWH->hWnd,SW_SHOW);
	}
	if(pWH->bHide > 0)//trayicon.uID != NULL)
	{
		nid.cbSize           = sizeof(NOTIFYICONDATA);
		nid.uFlags           = NIF_ICON|NIF_TIP|NIF_MESSAGE;
		nid.uID              = pWH->trayicon.uID;
		nid.hIcon            = pWH->trayicon.hIcon;
		nid.hWnd             = pWH->trayicon.hwnd;
		//wcscpy(nid.szTip,(const wchar_t *)sW_dll[i].cIconTip);
//		if(IsBadReadPtr(pWH->cIconTip,1) == 0)
//		{
//			wcscpy(nid.szTip,pWH->cIconTip);
//		}
		nid.uCallbackMessage = pWH->trayicon.uCallbackMessage;
		//Shell_NotifyIcon(NIM_ADD,&nid);
		{
			int iCommandId = pWH->bHide;
			if(iCommandId == 0)
			{
				iCommandId = pWH->iCommandId;
			}
			SendMessageW(g_TrayWnd,TB_HIDEBUTTON,WPARAM(iCommandId),(LPARAM)MAKELONG(FALSE,0));
		}
		{
			NOTIFYICONDATA nid2;
			memset(&nid2,0,sizeof(nid2));
			nid2.cbSize=sizeof(nid2);
			nid2.hWnd=nid.hWnd;
			nid2.uID=nid.uID;
			nid2.uFlags=NIF_STATE;
			nid2.dwState=NIS_HIDDEN;
			::SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)FALSE,0);
			Shell_NotifyIcon(NIM_MODIFY,&nid2);
			SendMessage(g_TrayWnd,WM_SETREDRAW,(WPARAM)TRUE,0);
		}
	}
	pTemp = pWH;
	pNext = pWH->Next;

	if(pTemp->cIconTip != NULL && IsBadReadPtr(pTemp->cIconTip,1024) == 0)
	{
		delete [] pTemp->cIconTip;
	}
	if(pTemp->cWndTitle != NULL)
	{
		VirtualFree(pTemp->cWndTitle,0,MEM_RELEASE);
	}
	delete pTemp;



	while(pNext)
	{
		pTemp = pNext->Next;
		ShowWindow(pNext->hWnd,SW_SHOW);
		if(IsBadReadPtr(pNext->cIconTip,1) == 0)
		{
			delete [] pNext->cIconTip;
		}
		if(pNext->cWndTitle != NULL)
		{
			VirtualFree(pNext->cWndTitle,0,MEM_RELEASE);
		}
		delete pNext;
		pNext = pTemp;
	}
	
	int iItemCount = 0;
	iItemCount = ListView_GetItemCount(hList_BK);
	if(iItemCount == 1)
	{
		EnableWindow(hButton_ShowBK,FALSE);
		//ShowWindowNormal(hMainDialog);
		ImageList_RemoveAll(hImage_BK);
		ImageList_AddIcon(hImage_BK,hIcon_NoIcon);
		iBKItemCount = 0;
	}


	return 1;
}








int AddBKListHeader()
{
	LVCOLUMNW lvColumn;
	DWORD lvStyle = 0;
	wchar_t pTrayIcon[100],pRelatedTitle[512],pProcessID[100];
	//lvColumn.pszText = new wchar_t [200];
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt  = LVCFMT_LEFT;
	lvColumn.cx   = 80;
	//wcscpy(lvColumn.pszText,"托盘图标");
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
	//ListView_InsertColumn(hList_BK,0,&lvColumn);
	SendMessageW(hList_BK,LVM_INSERTCOLUMNW,0,(LPARAM)&lvColumn);
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
	//ListView_InsertColumn(hList_BK,1,&lvColumn);
	SendMessageW(hList_BK,LVM_INSERTCOLUMNW,1,(LPARAM)&lvColumn);
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
	//ListView_InsertColumn(hList_BK,2,&lvColumn);
	SendMessageW(hList_BK,LVM_INSERTCOLUMNW,2,(LPARAM)&lvColumn);
	lvStyle = ListView_GetExtendedListViewStyle(hList_BK);
	lvStyle = lvStyle|LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyleEx(hList_BK,0,lvStyle);

	hImage_BK = ImageList_Create(16,16,ILC_COLOR24,20,10);
	ImageList_AddIcon(hImage_BK,hIcon_NoIcon);
	ListView_SetImageList(hList_BK,hImage_BK,LVSIL_SMALL);
	//delete [] lvColumn.pszText;
	_CrtDumpMemoryLeaks();
	return 1;
}


int RestoreAllBK(int iKorS)
{
	int i = 0,iCount = ListView_GetItemCount(hList_BK);
	LVITEM lvItem;
	WndAndHandle * pData = NULL;
	if(iKorS == 'S')
	{
		for(i = 0;i<iCount;i++)
		{
			lvItem.iItem = 0;
			lvItem.mask = LVIF_PARAM;
			ListView_GetItem(hList_BK,&lvItem);
			ShowSelected((WndAndHandle*)(lvItem.lParam),1);
			ListView_DeleteItem(hList_BK,0);
		}
	}
	else
	{
		for (int i = 0;i<iCount;i++)
		{
			lvItem.iItem = i;
			lvItem.mask = LVIF_PARAM;
			ListView_GetItem(hList_BK,&lvItem);
			pData = (WndAndHandle *)(lvItem.lParam);
			if(wcscmp((wcslwr(pData->cProcessName)),L"explorer.exe")!= 0)
			{
				TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS,FALSE,pData->lPID),0);
			}
		}
	}
	return 1;
}




INT_PTR CALLBACK QuickHideProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	OPENFILENAME OFN;
	wchar_t * szFile;
	wchar_t * pFileType = NULL, * pDialogTitle = NULL, * pT1 = NULL,
		* pT2 = NULL, * pT3 = NULL, * pT4 = NULL;

	wchar_t * cCurDir = NULL;

	HWND hStatic_SystemSetting,hStatic_Selected;
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		if(sOP.bReserveSel == 1)
		{
			CheckDlgButton(hwndDlg,IDC_RADIO_RESERVE,BST_CHECKED);
		}
		else
		{
			CheckDlgButton(hwndDlg,IDC_RADIO_HIDE,BST_CHECKED);
		}
		CheckDlgButton(hwndDlg,IDC_CHECK_NEEDSTART,sOP.bNeedStart?BST_CHECKED:BST_UNCHECKED);
		hEdit_PPath = GetDlgItem(hwndDlg,IDC_EDIT_PPATH);
		hBtn_Path = GetDlgItem(hwndDlg,IDC_BUTTON_SELPATH);
		EnableWindow(hEdit_PPath,sOP.bNeedStart?TRUE:FALSE);
		EnableWindow(hBtn_Path,sOP.bNeedStart?TRUE:FALSE);
		SetWindowText(hEdit_PPath,sOP.bNeedStart?sOP.cTrailRun:L"");
		if(IsWindowEnabled(hListView) == FALSE)
		{
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO_RESERVE),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,IDC_RADIO_HIDE),FALSE);
		}
		if(sOP.bAutoHide_QH == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_HIDEPOPUP,BST_CHECKED);
		}
		if(sOP.bReserveHabit == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_AUTOMEM,BST_CHECKED);
		}
		if(sOP.bHideAllTray == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_HIDEALLTRAY,BST_CHECKED);
		}
		if(sOP.bCleanAfterHide == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_CLEANAFTERHIDE,BST_CHECKED);
		}
		if(sOP.bHideProcess == 1)
		{
			CheckDlgButton(hwndDlg,IDC_CHECK_HIDEPROCESS,BST_CHECKED);
		}

#ifndef _HIDEPROCESS
		ShowWindow(GetDlgItem(hwndDlg,IDC_CHECK_HIDEPROCESS),SW_HIDE);
#endif



		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOGQUICKHIDE);
		}
		
		hStatic_SystemSetting = GetDlgItem(hwndDlg,IDC_STATIC_SYSTEMSETTING);
		SendMessage(hStatic_SystemSetting,WM_SETFONT,(WPARAM)hFont_New,0);
		hStatic_Selected = GetDlgItem(hwndDlg,IDC_STATIC_SELECTED);
		SendMessage(hStatic_Selected,WM_SETFONT,(WPARAM)hFont_New,0);

		return FALSE;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam) ->code)
		{
		case PSN_KILLACTIVE:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_NEEDSTART) == BST_CHECKED)
			{
				szFile = new wchar_t [MAX_PATH];
				GetDlgItemText(hwndDlg,IDC_EDIT_PPATH,szFile,MAX_PATH);
				if(wcslen(szFile) == 0)
				{
					HDMessageBoxW(L"IDS_MESSAGEBOX_NEEDSELPROGRAM",
						L"If enable run program after hide window,please specify a program path.",hwndDlg);
					SetFocus(GetDlgItem(hwndDlg,IDC_EDIT_PPATH));
					SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
					delete [] szFile;
					return TRUE;
				}
				delete [] szFile;
			}
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		case PSN_APPLY:
			if(IsDlgButtonChecked(hwndDlg,IDC_RADIO_RESERVE) == BST_CHECKED)
			{
				if(g_iTipCount >= 1)
				{
					wcscpy(g_pTipText[1][0],L"IDS_USAGETIP_SELWNDLISTCANRESERVEWND");
					wcscpy(g_pTipText[1][1],L"Check below window list item can to reserve or hide:");
					if(IsWindowEnabled(hListView) == TRUE)
					{
						SetTipText(L"IDS_USAGETIP_SELWNDLISTCANRESERVEWND",
							L"Check below window list item can to reserve or hide:");
					}
				}
				sOP.bReserveSel = 1;
			}
			else if(IsDlgButtonChecked(hwndDlg,IDC_RADIO_HIDE) == BST_CHECKED)
			{
				if(g_iTipCount >= 1)
				{
					wcscpy(g_pTipText[1][0],L"IDS_USAGETIP_HIDESELITEM");
					wcscpy(g_pTipText[1][1],L"In Window List selected item will hide.");
					if(IsWindowEnabled(hListView) == TRUE)
					{
						SetTipText(L"IDS_USAGETIP_HIDESELITEM",
							L"In Window List selected item will hide.");
					}
				}
				
				sOP.bReserveSel = 0;
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_NEEDSTART) == BST_CHECKED)
			{
				sOP.bNeedStart = 1;
				GetDlgItemText(hwndDlg,IDC_EDIT_PPATH,sOP.cTrailRun,MAX_PATH);
				if(wcslen(sOP.cTrailRun) == 0)
				{
					HDMessageBoxW(L"IDS_MESSAGEBOX_NEEDSELPROGRAM",
						L"If enable run program after hide window,please specify a program path.",hwndDlg);
					SetFocus(GetDlgItem(hwndDlg,IDC_EDIT_PPATH));
					SetWindowLong(hwndDlg,DWL_MSGRESULT,TRUE);
					return TRUE;
				}
			}
			else
			{
				sOP.bNeedStart = 0;
				memset(sOP.cTrailRun,0,MAX_PATH * sizeof(wchar_t));
			}

			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEPOPUP) == BST_CHECKED)
			{
				sOP.bAutoHide_QH = 1;
			}
			else
			{
				sOP.bAutoHide_QH = 0;
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_AUTOMEM) == BST_CHECKED)
			{
				sOP.bReserveHabit = 1;
			}
			else
			{
				sOP.bReserveHabit = 0;
			}	
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEALLTRAY) == BST_CHECKED)
			{
				sOP.bHideAllTray = 1;
			}
			else
			{
				sOP.bHideAllTray = 0;
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_CLEANAFTERHIDE) == BST_CHECKED)
			{
				sOP.bCleanAfterHide = 1;
				CheckDlgButton(g_TabWnd[5],IDC_CLEAN_CLEANAFTER,BST_CHECKED);
			}
			else
			{
				sOP.bCleanAfterHide = 0;
				CheckDlgButton(g_TabWnd[5],IDC_CLEAN_CLEANAFTER,BST_UNCHECKED);
			}
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_HIDEPROCESS))
			{
				sOP.bHideProcess = 1;
			}
			else
			{
				sOP.bHideProcess = 0;
			}


			WriteOptionFile(sOP);
			
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
		default:
			return FALSE;
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_SELPATH:
			szFile = new wchar_t [MAX_PATH];
			memset(szFile,0,MAX_PATH * sizeof(wchar_t));
			memset(&OFN,0,sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.lpstrDefExt = L".EXE";
			OFN.hwndOwner = hwndDlg;
			OFN.hInstance = NULL;
			if(g_iUseLanguage == 0)
			{
				OFN.lpstrFilter = L"Executable Files (*.EXE)\0*.EXE\0All Files (*.*)\0*.*\0";
			}
			else
			{
				pFileType = new wchar_t[1024];
				memset(pFileType,0,1024 * sizeof(wchar_t));
				//pFileType = new wchar_t[1024];
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
				OFN.lpstrTitle = L"Please Select Executable File";
			}
			else
			{
				pDialogTitle = new wchar_t[1024];
				memset(pDialogTitle,0,1024 * sizeof(wchar_t));
				OFN.lpstrTitle = LoadItemStringW(L"IDS_QUICKHIDE_DAILOGTITLE",pDialogTitle,L"Please Select Executable File");
			}
			OFN.Flags = OFN_CREATEPROMPT|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;

			if(GetOpenFileName(&OFN))
			{
				SetDlgItemText(hwndDlg,IDC_EDIT_PPATH,szFile);
				PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
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


			break;
		case IDC_RADIO_RESERVE:
		case IDC_RADIO_HIDE:
		case IDC_CHECK_HIDEPOPUP:
		case IDC_CHECK_AUTOMEM:
		case IDC_CHECK_HIDEALLTRAY:
		case IDC_CHECK_CLEANAFTERHIDE:
		case IDC_CHECK_HIDEPROCESS:

			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_CHECK_NEEDSTART:
			if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_NEEDSTART) == BST_CHECKED)
			{
				EnableWindow(hEdit_PPath,TRUE);
				EnableWindow(hBtn_Path,TRUE);
			}
			else if(IsDlgButtonChecked(hwndDlg,IDC_CHECK_NEEDSTART) == BST_UNCHECKED)
			{
				EnableWindow(hEdit_PPath,FALSE);
				EnableWindow(hBtn_Path,FALSE);
			}
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



INT_PTR CALLBACK MouseMotionProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_INITDIALOG:
		return FALSE;
	default:
		return FALSE;
	}
}


INT_PTR CALLBACK AutoUpdateProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HWND hStatic_Update;



	switch(uMsg)
	{
	case WM_INITDIALOG:
		hRadioAuto = GetDlgItem(hwndDlg,IDC_RADIO_AUTOUPDATE);
		hRadioTip = GetDlgItem(hwndDlg,IDC_RADIO_TIP);
		if(sOP.iAutoUpdate == ID_AUTOUPDATE_AUTODONTTIP)
		{
			CheckRadioButton(hwndDlg,IDC_RADIO_AUTOUPDATE,IDC_RADIO_TIP,IDC_RADIO_AUTOUPDATE);
		}
		else
		{
			CheckRadioButton(hwndDlg,IDC_RADIO_AUTOUPDATE,IDC_RADIO_TIP,IDC_RADIO_TIP);
		}
		if(g_iUseLanguage == 1)
		{
			SetDialogTextW(&hwndDlg,IDD_DIALOG_AUTOUPDATE);
		}
		hStatic_Update = GetDlgItem(hwndDlg,IDC_STATIC_UPDATE);
		SendMessage(hStatic_Update,WM_SETFONT,(WPARAM)hFont_New,0);
		
		return FALSE;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam) ->code)
		{
		case PSN_APPLY:
			if(IsDlgButtonChecked(hwndDlg,IDC_RADIO_AUTOUPDATE) == BST_CHECKED)
			{
				sOP.iAutoUpdate = ID_AUTOUPDATE_AUTODONTTIP;
			}
			else if(IsDlgButtonChecked(hwndDlg,IDC_RADIO_TIP) == BST_CHECKED)
			{
				sOP.iAutoUpdate = ID_AUTOUPDATE_TIP;
			}
			WriteOptionFile(sOP);
			SetWindowLong(hwndDlg,DWL_MSGRESULT,FALSE);
			return FALSE;
			
		default:
			return FALSE;
		}
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_RADIO_AUTOUPDATE:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		case IDC_RADIO_TIP:
			PropSheet_Changed(::GetParent(hwndDlg),hwndDlg);
			return FALSE;
		default:
			return FALSE;
		}
		return FALSE;
	default:
		return FALSE;
	}
}


INT_PTR CALLBACK MultiLanguageProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_INITDIALOG:
		return FALSE;
	default:
		return FALSE;
	}
}





unsigned __stdcall RefreshThread(void * pParam)
{
	while(1)
	{
		if(bRef && sOP.iRefInterval > 0)
		{
			FillList();
		}
		if(sOP.iRefInterval > 0)
		{
			Sleep(sOP.iRefInterval*1000);
		}
		else
		{
			Sleep(1*1000);
		}
	}
	

	return 1;
}




int ShowWindowNormal(HWND hwnd)
{
	HWND hForeground = GetDlgItem(hwnd,IDC_STATIC_FOREGROUND);
	RECT Rect,MainRect;
	GetWindowRect(hForeground,&Rect);
	GetWindowRect(hwnd,&MainRect);

	//CAxWindow hWeb;
	//hWeb = GetDlgItem(hMainDialog,IDC_WEB);
	
	//GetWindowRect(hWeb,&Rect);
	SetWindowPos(hwnd,NULL,0,0,MainRect.right-MainRect.left,Rect.bottom-MainRect.top + 10,SWP_NOMOVE|SWP_NOZORDER);
//	IWebBrowser2*  WebBrowser;
//	VARIANT    varMyURL;

	
//	if(IsWindow(hWeb) == FALSE)
//	{
//		//MessageBox(hwndDlg,"ddddddddd","dddddddd",MB_OK);
//	}
//	else
//	{
//		hWeb.QueryControl( __uuidof(IWebBrowser2), (void**)&WebBrowser);
//		VariantInit(&varMyURL);
//		varMyURL.vt = VT_BSTR;
//		wchar_t  *buffer;
//		DWORD  size;
//
//		size = MultiByteToWideChar(CP_ACP, 0, "http://www.hidedragon.com/ads2.htm", -1, 0, 0);
//		if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size))) return FALSE;
//		MultiByteToWideChar(CP_ACP, 0, "http://www.hidedragon.com/ads2.htm", -1, buffer, size);
//		varMyURL.bstrVal = SysAllocString(buffer);
//		GlobalFree(buffer);
//
//		WebBrowser->Navigate2(&varMyURL, 0, 0, 0, 0);
//
//		VariantClear(&varMyURL);
//	}
	iExtWindow = 0;
	return 1;
}

int ExtendWindow()
{
	HWND hBKGroup = GetDlgItem(hMainDialog,IDC_STATIC_BKGROUP);
	RECT bkRect,mainRect;
	GetWindowRect(hBKGroup,&bkRect);
	GetWindowRect(hMainDialog,&mainRect);
	SetWindowPos(hMainDialog,NULL,0,0,mainRect.right-mainRect.left,bkRect.bottom-mainRect.top+10,SWP_NOMOVE|SWP_NOZORDER);
	iExtWindow = 1;
	
	return 1;
}



int ModifyAutoReserveMenu(HMENU hMenu,int iSel)
{
	iSel = ListView_GetSelectionMark(hListView);
	sFileItem * pItem = NULL;
	wchar_t * pTitle = (wchar_t *)VirtualAlloc(NULL,2048,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	memset(pTitle,0,2048 * sizeof(wchar_t));
	iSelectedItem = iSel;
	ListView_GetItemText(hListView,iSel,0,pTitle,2046);
	LVITEMW lvItem;
	memset(&lvItem,0,sizeof(LVITEMW));
	lvItem.iSubItem = 0;
	lvItem.pszText = pTitle;
	lvItem.cchTextMax = 2048;
	lvItem.iItem = iSel;

	SendMessageW(hListView,LVM_GETITEMTEXTW,iSel,(LPARAM)&lvItem);
	GetCursorPos(&CurMouPos);
	for(i = 0;i < iBaSW;i++)
	{
		if(wcscmp(sW[i].cWndTitle,pTitle) == 0)
		{
			pItem = new sFileItem;
			pItem->cFileName = sW[i].cProcessName;
			pItem->lSize = wcslen(sW[i].cProcessName) * 2;
			{
				DBClass arDB;
				
				if(arDB.IsInARFL(pItem))
				{
					if(g_iUseLanguage == 0)
					{
						ModifyMenuW(hMenu,IDM_AUTORESERVE,MF_BYCOMMAND,IDM_AUTORESERVE,L"Cance&l Automatic Check This Window");
					}
					else
					{
						wchar_t * pAutoReserve = new wchar_t[1024];
						ModifyMenuW(hMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
						IDM_AUTORESERVE,LoadItemStringW(L"IDM_CANCELAUTORESERVE",pAutoReserve,
						L"Cance&l Automatic Check This Window"));
						delete [] pAutoReserve;
					}
				}
				else
				{
					if(g_iUseLanguage == 0)
					{
						ModifyMenuW(hMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
						IDM_AUTORESERVE,L"&Automatic Check This Window");
					}
					else
					{
						wchar_t * pAutoReserve = new wchar_t[1024];
						ModifyMenuW(hMenu,IDM_AUTORESERVE,MF_BYCOMMAND,
						IDM_AUTORESERVE,LoadItemStringW(L"IDM_AUTORESERVE",pAutoReserve,
						L"&Automatic Check This Window"));
						delete [] pAutoReserve;
					}
				}
			}
			delete pItem;
		}
	}
	if(ListView_GetCheckState(hListView,iSelectedItem) == FALSE)
	{
		if(g_iUseLanguage == 0)
		{
			ModifyMenuW(hMenu,IDM_RESERVE,MF_BYCOMMAND,IDM_RESERVE,L"&Check This Window");
		}
		else
		{
			wchar_t * pReserve = new wchar_t[1024];
			ModifyMenuW(hMenu,IDM_RESERVE,MF_BYCOMMAND,
			IDM_RESERVE,LoadItemStringW(L"IDM_RESERVE",pReserve,
			L"&Check This Window"));
			delete [] pReserve;
		}
	}
	else
	{
		if(g_iUseLanguage == 0)
		{
			ModifyMenuW(hMenu,IDM_RESERVE,MF_BYCOMMAND,IDM_RESERVE,L"&Cancel Check This Window");
		}
		else
		{
			wchar_t * pReserve = new wchar_t[1024];
			ModifyMenuW(hMenu,IDM_RESERVE,MF_BYCOMMAND,
			IDM_RESERVE,LoadItemStringW(L"IDM_CANCELRESERVE",pReserve,
			L"&Cancel Check This Window"));
			delete [] pReserve;
		}
	}


	VirtualFree(pTitle,0,MEM_RELEASE);
	return 1;
}


int IsRegister()
{
	wchar_t cRegKey[100];
	BYTE cVal[50];
	DWORD dwError = 0,dwLen = 50,dwDataType = 0;
	LONG lRet = 0,lRet2 = 0;
	HKEY hRegVal;


	memset(cRegKey,0,100 * sizeof(wchar_t));
	memset(cVal,0,50 * sizeof(wchar_t));
	wcscpy(cRegKey,L"Software\\HideDragon");
	lRet = RegOpenKey(HKEY_LOCAL_MACHINE,cRegKey,&hRegVal);
// 	if(lRet == FALSE)
// 	{
// 		return FALSE;
// 	}
	lRet2 = RegQueryValueEx(hRegVal,L"Val5",NULL,&dwDataType,(LPBYTE)cVal,&dwLen);
	if (lRet == ERROR_FILE_NOT_FOUND || lRet2 == ERROR_FILE_NOT_FOUND)
	{
		lRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE,L"Software\\HideDragon",NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,
			&hRegVal,NULL);
		dwLen = strlen(cGUID1);
		lRet = RegSetValueEx(hRegVal,L"Val1",NULL,REG_BINARY,(LPBYTE)cGUID1,dwLen);
		lRet = RegSetValueEx(hRegVal,L"Val2",NULL,REG_BINARY,(LPBYTE)cGUID2,dwLen);
		lRet = RegSetValueEx(hRegVal,L"Val3",NULL,REG_BINARY,(LPBYTE)cGUID3,dwLen);
		lRet = RegSetValueEx(hRegVal,L"Val4",NULL,REG_BINARY,(LPBYTE)cGUID4,dwLen);
		lRet = RegSetValueEx(hRegVal,L"Val5",NULL,REG_BINARY,(LPBYTE)cGUID5,dwLen);
	}
	dwLen = 50;
	lRet = RegQueryValueEx(hRegVal,L"Val5",NULL,&dwDataType,(LPBYTE)cVal,&dwLen);
	int iUsedTime = 0;
	ReadTimes(&iUsedTime);
	if(memcmp(cVal,cGUID6,dwLen) == 0 /*&& iUsedTime <= iTryTime*/)
	{
		RegCloseKey(hRegVal);
		return 1;
	}
	RegCloseKey(hRegVal);

	
	return 0;
}

int WriteRegCode()
{
	HKEY hRegVal;
	wchar_t cRegKey[50];
	LONG lRet = 0;
	memset(cRegKey,0,50 * sizeof(wchar_t));
	wcscpy(cRegKey,L"Software\\HideDragon");
	RegOpenKey(HKEY_LOCAL_MACHINE,cRegKey,&hRegVal);
	DWORD dwLen = strlen(cGUID6);
	lRet = RegSetValueEx(hRegVal,L"Val5",NULL,REG_BINARY,(LPBYTE)cGUID6,dwLen);
	
	return 1;	
}

int	FillNotExistWnd(int iListFlag /* == 0 */)
{
	HWND hTaskbar = FindTaskbarWnd();

	DWORD dwExpID = 0;
	GetWindowThreadProcessId(hTaskbar,&dwExpID);
	
	BOOL ret=FALSE;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS
		|PROCESS_VM_OPERATION
		|PROCESS_VM_READ
		|PROCESS_VM_WRITE,
		FALSE,
		dwExpID);

	if(hProcess == NULL)
	{
		return 0;
	}
	
	PVOID ptb=VirtualAllocEx(hProcess,NULL,sizeof(TBBUTTON),MEM_COMMIT,PAGE_READWRITE);

	TBBUTTON pTB;
	memset(&pTB,0,sizeof(TBBUTTON));
	
	ULONG lButtons=(ULONG)::SendMessage(hTaskbar,TB_BUTTONCOUNT,0,0);


	HWND hWnd = NULL;
	for(ULONG i=0;i<lButtons;++i)
	{
		::SendMessage(hTaskbar,TB_GETBUTTON,WPARAM(i),LPARAM(ptb));
        ::ReadProcessMemory(hProcess,ptb,&pTB,sizeof(TBBUTTON),NULL);


		
		int iSize = sizeof(HWND);
		::ReadProcessMemory(hProcess,(PVOID)(pTB.dwData),&hWnd,iSize,NULL);



		//if this window is HideDragon,continue
		{
			DWORD dwPID = 0;
			GetWindowThreadProcessId(hWnd,&dwPID);
			if(dwPID == GetCurrentProcessId())
			{
				continue;
			}
		}
		
		
		if(IsWindow(hWnd) && IsInSW(hWnd) == 0)
		{
			EnumWindowsProc(hWnd,NULL);
		}
		if(iListFlag == 1 && IsWindow(hWnd) && IsInBKSW(hWnd) == 0)
		{
			EnumWindowsForBKProc(hWnd,(LPARAM)sBKSel);
		}
    }
	
	::VirtualFreeEx(hProcess,ptb,0,MEM_RELEASE);
    ::CloseHandle(hProcess);
	
	



	return 1;
}



wchar_t cPass[15];

INT_PTR CALLBACK PasswordDlgProc(HWND hwndDlg,UINT uMsg,WPARAM wParam ,LPARAM lParam)
{
	HWND hEdit_Password,hStatic_Result;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetForegroundWindow(hwndDlg);
		hEdit_Password = GetDlgItem(hwndDlg,IDC_PASSWORD_EDIT);
		SetFocus(hEdit_Password);
		
		
		SetDialogTextW(&hwndDlg,IDD_DIALOG_PASSWORD);
		SendMessage(hEdit_Password,EM_LIMITTEXT,10,0);


		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_PASSWORDDLG_OK:
			hEdit_Password = GetDlgItem(hwndDlg,IDC_PASSWORD_EDIT);
			hStatic_Result = GetDlgItem(hwndDlg,IDC_PASSWORDDLG_WRONGPASSWORD);
			
			SendMessage(hEdit_Password,EM_LIMITTEXT,10,0);


			memset(cPass,0,15 * sizeof(wchar_t));
			GetWindowText(hEdit_Password,cPass,11);
			if(wcscmp(cPass,sOP.cPassword)==0)
			{
				EndDialog(hwndDlg,1);
			}
			else
			{
				SetWindowTextW(hEdit_Password,L"");
				ShowWindow(hStatic_Result,SW_SHOW);
				SetFocus(hEdit_Password);
			}




			
			break;
		case IDC_PASSWORDDLG_CANCEL:
			EndDialog(hwndDlg,0);
			break;
		case IDCANCEL:
			EndDialog(hwndDlg,0);
		default:
			break;
		}
		break;
	default:
		return FALSE;

	}
	
	return FALSE;

}




int ShowPasswordDlg()
{
	if(sOP.bUsePassword == 0)
	{
		return 1;
	}
	if (DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),NULL,(DLGPROC)PasswordDlgProc) == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



//int SetWebControlPos()
//{
//
//	
//	return 1;
//}












