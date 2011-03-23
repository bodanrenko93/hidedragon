//All rights reserved 2003.4.7 DongDapeng
//dongdapeng8@sohu.com
//kinghome.51.net




#ifdef HOOKLIBAPI 


#else

#define HOOKLIBAPI extern "C" __declspec(dllimport)

#endif

struct WndParam
{
	wchar_t cT[80][80];
	wchar_t cP[80][80];
};


struct TRAYDATA
{
	HWND hwnd;				
	UINT uID;				
	UINT uCallbackMessage;	
	DWORD Reserved[2];		
	HICON hIcon;				
};


struct TrayItemInfo
{
	HWND hwnd;				
	UINT uID;				
	UINT uCallbackMessage;
	wchar_t *  sTip;
	wchar_t *  sProcessPath;
	bool bVisible;
};

 






struct WndAndHandle
{
	wchar_t * cWndTitle;
	DWORD lPID;
	HWND hWnd;
	int bReserved;
	int iWindowType;
	int bExist;
	int bHide;
	wchar_t cProcessName[30];
	int iHasTrayIcon;
	int iIconIndex;
	TRAYDATA  trayicon;
	wchar_t  * cIconTip;
	int iLink;
	int bToBk;
	int iCommandId;
	WndAndHandle * Next;
};


struct sHotKey
{
	WORD wHK_Res;
	UINT uMF;
	UINT uVK;
};


#define ID_AUTOUPDATE_AUTODONTTIP  1
#define ID_AUTOUPDATE_TIP          2






struct OptionFile
{
	int  bUseMouse;
	int iRefInterval;
	int iKorS;
	int bDisableSound;
	int bAutoRun;
	int bAddQuickLaunch;
	int bOnTop;
	int iUsedTimes;
	int bRegistered;
	int bNeedStart;
	int bReserveSel;
	wchar_t * cSerialNumber;
	wchar_t * cMiddleCode;
	wchar_t * cTrailRun;
	int bUsePassword;
	wchar_t cPassword[15];
	int bUseFunctionKey;
	int iFunctionKey;
	int bUseBKMouse;
	int bAutoHide;
	int bHideMain;
	int bHideIcon;
	int bUseResHK;
	int bAutoHide_QH;
	int bReserveHabit;
	int bShowTip;
	int bShowBKTip;
	int iAutoUpdate;
	int bRightFirst;
	int bUseMiddle;
	sHotKey sResHK;
	sHotKey sHaHK;
	sHotKey sSsHK;
	int bHideAllTray;
	int bCloseInfoPanel;
	int bAutoChangeIconTitle;
	int bBeginnerModel;
	int bHideProcess;
	int bNotShowProcess;
	int bCleanAfterHide;
	int bAutoHideProgram;
	int doubleClick;
	int bExitProcess;

};





#define WM_ADDNEWWINDOW WM_USER + 5 





typedef  int  (EXEREFRESHFUNC)();
typedef int (EXESETDLGTEXTFUNC)(HWND*,UINT);
typedef int (EXESETTIPTEXTFUNC)(wchar_t*,wchar_t*);
typedef int (EXECLEARTIPTEXT)();
typedef int (EXEAPPENDTIPTEXT)(wchar_t *,wchar_t *);
typedef int (EXECLEANHISTORY)();




struct ExeFunc
{
	EXEREFRESHFUNC * pFillList;
	EXESETDLGTEXTFUNC * pSetDialogText;
	EXESETTIPTEXTFUNC * pSetTipText;
	EXECLEARTIPTEXT * pClearTipText;
	EXEAPPENDTIPTEXT * pAppendTipText;
	EXECLEANHISTORY * pCleanHistory;
};

struct ExeData
{
	WndAndHandle * sWP;
	int iCount;
	HWND * pArr_QH;
	int iUseLanguage;
	int * pTipCount;
	wchar_t *** pTipText;
};


HOOKLIBAPI int iTest;
HOOKLIBAPI int Add(int ,int);
HOOKLIBAPI int SetMouseHook(int );
HOOKLIBAPI int SetTrayHook(int iInstall);
//HOOKLIBAPI int SetCbtHook(int);
HOOKLIBAPI LRESULT CALLBACK MouseHookProc(int ,WPARAM ,LPARAM);
LRESULT CALLBACK TrayHookProc(int nCode,WPARAM wParam ,LPARAM lParam);
LRESULT CALLBACK CbtHookProc(int ,WPARAM ,LPARAM);
HOOKLIBAPI HWND hCurWnd,hLV,hAll;
HOOKLIBAPI int iBaSW_QH;
HOOKLIBAPI BOOL CALLBACK EnumWindowsFunc(HWND,LPARAM);
HOOKLIBAPI int  MyHideWindows();
HOOKLIBAPI int ReserveWnds(ExeData * pExeData);
HOOKLIBAPI int ClearAll();
HOOKLIBAPI HHOOK hMouseHook;
HOOKLIBAPI HHOOK hTrayHook;
HOOKLIBAPI int RestoreAllWindows(int );
HOOKLIBAPI int SetParameter(OptionFile,ExeFunc pExeFunc);
/*HOOKLIBAPI*/ int SetSW_Dll(DWORD ,HWND );// Temp comment HOOKLIBAPI 
HOOKLIBAPI HWND hTrayWnd_dll;
//HOOKLIBAPI int ReadWHFile(WndAndHandle *);
//HOOKLIBAPI int WriteWHFile(WndAndHandle *);
HOOKLIBAPI int RestoreVolume();
HWND FindTrayWnd();
HWND FindTrayToolbarWindow();
int EnumAllVisibleTray_dll();
int HideAllTray();
int ShowAllTray();
int GetProcessTrayIcon(DWORD lPID,TBBUTTON * pTB,TRAYDATA * pTrayData);
int ReleaseResourceForTrayList_dll();
int AllocResourceForTrayList();
HOOKLIBAPI int HideProcess(DWORD dwPID);
HOOKLIBAPI int ShowProcess(DWORD dwPID);
HOOKLIBAPI BOOL GetHidedProcessName(DWORD dwPID,wchar_t * cProcessName);
HOOKLIBAPI int GetHidedProcessCount();
HOOKLIBAPI int GetHidedProcessByIdx(int idx,wchar_t * cProcessName);
HOOKLIBAPI HICON ReadTrayIcon_dll();



int dbview(wchar_t * pInfo);






unsigned __stdcall HideWindowProc(void * pParam);
unsigned __stdcall MonitorProc(void * pParam);



unsigned __stdcall MouseHookThread(void * pParam);


INT_PTR CALLBACK ValidateProc(HWND,UINT,WPARAM,LPARAM);
//int MoveWindow2BK();





