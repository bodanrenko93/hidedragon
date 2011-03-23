//All rights reserved 2003.4.7 DongDapeng
//dongdapeng8@sohu.com
//kinghome.51.net




#ifdef HOOKLIBAPI 


#else

#define HOOKLIBAPI extern "C" __declspec(dllimport)

#endif

struct WndParam
{
	char cT[80][80];
	char cP[80][80];
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
	char *  sTip;
	char *  sProcessPath;
	bool bVisible;
};








struct WndAndHandle
{
	char * cWndTitle;
	DWORD lPID;
	HWND hWnd;
	int bReserved;
	int iWindowType;
	int bExist;
	int bHide;
	char cProcessName[20];
	int iHasTrayIcon;
	int iIconIndex;
	TRAYDATA  trayicon;
	char  * cIconTip;
	int iLink;
	WndAndHandle * Next;
};

struct OptionFile
{
	UINT iHideWindows;
	UINT iShowSelf;
	int  bUseMouse;
	int iRefInterval;
	int iKorS;
	int bDisableSound;
	int bAutoRun;
	int bAddQuickLaunch;
	int bOnTop;
	int iUsedTimes;
	int bRegistered;
	char * cSerialNumber;
	char * cMiddleCode;
	int bUsePassword;
	char cPassword[15];
	int bUseFunctionKey;
	int iFunctionKey;
	int bUseBKMouse;
	int bAutoHide;
	int bHideMain;
	int bHideIcon;
};



HOOKLIBAPI int iTest;
HOOKLIBAPI int Add(int ,int);
HOOKLIBAPI int SetMouseHook(int );
HOOKLIBAPI LRESULT CALLBACK MouseHookProc(int ,WPARAM ,LPARAM);
HOOKLIBAPI HWND hCurWnd,hLV,hAll;
HOOKLIBAPI BOOL CALLBACK EnumWindowsFunc(HWND,LPARAM);
HOOKLIBAPI int  MyHideWindows();
HOOKLIBAPI int ReserveWnds(WndAndHandle * sWP,int iCount);
HOOKLIBAPI int ClearAll();
HOOKLIBAPI HHOOK hMouseHook;
HOOKLIBAPI int RestoreAllWindows(int );
HOOKLIBAPI int SetParameter(OptionFile);
HOOKLIBAPI HWND hTrayWnd_dll;

INT_PTR CALLBACK ValidateProc(HWND,UINT,WPARAM,LPARAM);
//int MoveWindow2BK();




